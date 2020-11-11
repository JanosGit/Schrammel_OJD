
#include "OJDProcessor.h"
#include "OJDAudioProcessorEditor.h"

// To avoid extremely much typing when assigning filter coefficients
using BiquadCoeffs = juce::dsp::IIR::Coefficients<float>;

OJDAudioProcessor::OJDAudioProcessor()
  : rawValueDrive  (*parameters.getRawParameterValue (OJDParameters::Sliders::Drive::id)),
    rawValueTone   (*parameters.getRawParameterValue (OJDParameters::Sliders::Tone::id)),
    rawValueVolume (*parameters.getRawParameterValue (OJDParameters::Sliders::Volume::id)),
    rawValueHpLp   (*parameters.getRawParameterValue (OJDParameters::Switches::HpLp::id))
{
    // setup always constant elements in the chain
    chain.get<preWaveshaperGain>().setGainLinear (11.0f);

    // If the drive or hp/lp mode changes, some allocating biquad coefficient recalculation is done. The parameter
    // listener callback is used for this
    parameters.addParameterListener (OJDParameters::Sliders::Drive::id, this);
    parameters.addParameterListener (OJDParameters::Switches::HpLp::id, this);

    // Add a subtree where the editor stores some states
    parameters.state.appendChild (OJDAudioProcessorEditor::createUIStateSubtree(), nullptr);

    // Try to reach the schrammel server to find out if there is e.g. an update message to display
    checkForMessageOfTheDay();
}

void OJDAudioProcessor::prepareResources (bool sampleRateChanged, bool maxBlockSizeChanged, bool numChannelsChanged)
{
    if (numChannelsChanged)
    {
        jassert (getTotalNumOutputChannels() == numChannels);
        jassert (getTotalNumInputChannels()  == numChannels);
        return;
    }

    juce::ignoreUnused (sampleRateChanged, maxBlockSizeChanged);

    auto spec = createProcessSpec (numChannels);

    chain.prepare (spec);
    recalculateFilters();

    // Some fixed coefficients
    *chain.get<hpf30>()  .coefficients = *BiquadCoeffs::makeFirstOrderHighPass (spec.sampleRate, 30.0f);
    *chain.get<lpf6_3k>().coefficients = *BiquadCoeffs::makeFirstOrderLowPass  (spec.sampleRate, 6.3e3f);

    // Computing the chains latency. The oversampling in the waveshaper might introduce fractional sample delay
    const auto waveshaperLatency = chain.get<waveshaper>().getLatencyInSamples();
    setLatencySamples (static_cast<int> (waveshaperLatency));
}


bool OJDAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    auto mono = juce::AudioChannelSet::mono();
    return layouts.getMainInputChannelSet() == mono && layouts.getMainOutputChannelSet() == mono;
}

void OJDAudioProcessor::processBlock (juce::dsp::AudioBlock<float>& block)
{
    juce::ScopedNoDenormals noDenormals;
    juce::dsp::ProcessContextReplacing<float> context (block);

    updateParametersForProcessorChain();

    chain.process (context);
}


juce::AudioProcessorEditor* OJDAudioProcessor::createEditor() { return new OJDAudioProcessorEditor (*this); }

void OJDAudioProcessor::checkForMessageOfTheDay()
{
    auto& settingsManager = *jb::SettingsManager::getInstance();
    auto lastVersion = settingsManager.getInt64Setting ("LastMOTDVersionDisplayed", -1);

    infoAndUpdateMessage = messageOfTheDay.checkForNewMessages (lastVersion);
}

std::unique_ptr<jb::MessageOfTheDay::InfoAndUpdate> OJDAudioProcessor::getMessageOfTheDay (int timeoutMilliseconds)
{
    // The message has already been read
    if (!infoAndUpdateMessage.valid())
        return nullptr;

    auto waitDuration = std::chrono::milliseconds (timeoutMilliseconds);

    if (infoAndUpdateMessage.wait_for (waitDuration) == std::future_status::ready)
        return std::make_unique<jb::MessageOfTheDay::InfoAndUpdate> (infoAndUpdateMessage.get());

    // Timeout has been reached
    return nullptr;
}

void OJDAudioProcessor::updateParametersForProcessorChain()
{
    // Drive – coefficients are computed in the parameterChanged callback, therefore we need the lock
    if (biquadParametersUpdated.load() && biquadParameterLock.tryEnter())
    {
        std::swap (chain.get<biquadPreDriveBoost>().coefficients,   biquadPreDriveBoostCoeffs);
        std::swap (chain.get<biquadPreDriveNotch>().coefficients,   biquadPreDriveNotchCoeffs);
        std::swap (chain.get<biquadPostDriveBoost1>().coefficients, biquadPostDriveBoost1Coeffs);
        std::swap (chain.get<biquadPostDriveBoost2>().coefficients, biquadPostDriveBoost2Coeffs);
        std::swap (chain.get<biquadPostDriveBoost3>().coefficients, biquadPostDriveBoost3Coeffs);

        biquadParametersUpdated.store (false);
        biquadParameterLock.exit();
    }

    // Tone
    chain.get<tone>().setHpLpMode (OJDParameters::Switches::HpLp::getModeFromRaw (rawValueHpLp));
    chain.get<tone>().setTone     (OJDParameters::Sliders::normaliseRawValue (rawValueTone));

    // Volume
    chain.get<volume>().setGainDecibels (OJDParameters::Sliders::Volume::dBValueFromRawValue (rawValueVolume));
}

void OJDAudioProcessor::recalculateFilters()
{
    const auto sr = getSampleRate();
    if (sr == 0.0)
        return;

    const auto mode            = OJDParameters::Switches::HpLp::getModeFromRaw(rawValueHpLp);
    const auto driveNormalised = OJDParameters::Sliders::normaliseRawValue (rawValueDrive);
    const auto driveSquared = driveNormalised * driveNormalised;

    const auto biquadPreDriveBoostFreq = -1400.0f * driveSquared + 500.0f * driveNormalised + 1600.0f;
    const auto biquadPreDriveBoostQ = -0.1f * driveNormalised + 0.15f;
    const auto biquadPreDriveBoostGain = 32 * driveNormalised + 4;

    const auto biquadPreDriveNotchFreq = 8e3f;
    const auto biquadPreDriveNotchQ = 0.8f;
    const auto biquadPreDriveNotchGain = -5.0f * driveSquared;

    const auto biquadPostDriveBoost1Freq = mode == ToneStack::Mode::hp ? 2052.0f : 2781.0f;
    const auto biquadPostDriveBoost1Q = 0.5f;
    const auto biquadPostDriveBoost1Gain = mode == ToneStack::Mode::hp ? 4.6f : 4.38f;

    const auto biquadPostDriveBoost2Freq = 74.0f;
    const auto biquadPostDriveBoost2Q = 0.2f;
    const auto biquadPostDriveBoost2Gain = 7.38f * driveNormalised + 8.12f;

    const auto biquadPostDriveBoost3Freq = 2935.0f;
    const auto biquadPostDriveBoost3Q = 0.1f;
    const auto biquadPostDriveBoost3Gain = mode == ToneStack::Mode::hp ? 10.0f : 16.9f;

    // Entering the lock just before the allocating operation
    {
        juce::SpinLock::ScopedLockType scopedLock(biquadParameterLock);

        // to avoid extremely long lines below
#define CREATE_BIQUAD_COEFFICIENTS(stage) stage##Coeffs = BiquadCoeffs::makePeakFilter (sr, stage##Freq, stage##Q, juce::Decibels::decibelsToGain (stage##Gain))

        CREATE_BIQUAD_COEFFICIENTS(biquadPreDriveBoost);
        CREATE_BIQUAD_COEFFICIENTS(biquadPreDriveNotch);
        CREATE_BIQUAD_COEFFICIENTS(biquadPostDriveBoost1);
        CREATE_BIQUAD_COEFFICIENTS(biquadPostDriveBoost2);
        CREATE_BIQUAD_COEFFICIENTS(biquadPostDriveBoost3);
    }

    biquadParametersUpdated.store(true);
}

void OJDAudioProcessor::parameterChanged (const juce::String& parameterID, float)
{
    jassert (parameterID == OJDParameters::Sliders::Drive::id || parameterID == OJDParameters::Switches::HpLp::id);
    juce::ignoreUnused (parameterID);

    recalculateFilters();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new OJDAudioProcessor(); }
