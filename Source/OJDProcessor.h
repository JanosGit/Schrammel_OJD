

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OJDParameters.h"
#include "ToneStack.h"
#include "Waveshaper.h"
#include "Drawables.h"

class OJDAudioProcessor
  : public jb::PluginAudioProcessorBase<OJDParameters>,
    public juce::AudioProcessorValueTreeState::Listener
{
public:

    //==============================================================================
    OJDAudioProcessor();

    //==============================================================================
    void prepareResources (bool sampleRateChanged, bool maxBlockSizeChanged, bool numChannelsChanged) override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::dsp::AudioBlock<float>& block) override;

    void parameterChanged (const juce::String &parameterID, float newValue) override;

    juce::AudioProcessorEditor* createEditor() override;

    /**
     * Returns a InfoAndUpdate struct if a message has received from the server before the timeout occured or
     * a nullpointer if either the server didn't respond (yet) or the message has already been displayed
     */
    std::unique_ptr<jb::MessageOfTheDay::InfoAndUpdate> getMessageOfTheDay (int timeoutMilliseconds);

    Drawables drawables;

    /** This processor will only process one channel */
    static const int numChannels = 1;
private:
    // References to all raw parameter values
    const std::atomic<float>& rawValueDrive;
    const std::atomic<float>& rawValueTone;
    const std::atomic<float>& rawValueVolume;
    const std::atomic<float>& rawValueHpLp;

    // Signal path
    enum SignalPath
    {
        hpf30,
        biquadPreDriveBoost,   // dependent on Drive setting
        biquadPreDriveNotch,   // dependent on Drive setting
        preWaveshaperGain,
        waveshaper,
        biquadPostDriveBoost1, // dependent on HP/LP
        biquadPostDriveBoost2, // dependent on Drive setting
        biquadPostDriveBoost3, // dependent on HP/LP
        lpf6_3k,
        tone,
        volume
    };

    using HPF    = juce::dsp::IIR::Filter<float>;
    using LPF    = juce::dsp::IIR::Filter<float>;
    using Biquad = juce::dsp::IIR::Filter<float>;
    using Gain   = juce::dsp::Gain<float>;

    juce::dsp::ProcessorChain<HPF, Biquad, Biquad, Gain, Waveshaper, Biquad, Biquad, Biquad, LPF, ToneStack, Gain> chain;

    // This lock is held for a short time when the drive changes & biquad coefficients are exchanged
    juce::SpinLock biquadParameterLock;
    std::atomic<bool> biquadParametersUpdated { false };

    juce::dsp::IIR::Coefficients<float>::Ptr biquadPreDriveBoostCoeffs;
    juce::dsp::IIR::Coefficients<float>::Ptr biquadPreDriveNotchCoeffs;
    juce::dsp::IIR::Coefficients<float>::Ptr biquadPostDriveBoost1Coeffs;
    juce::dsp::IIR::Coefficients<float>::Ptr biquadPostDriveBoost2Coeffs;
    juce::dsp::IIR::Coefficients<float>::Ptr biquadPostDriveBoost3Coeffs;

    jb::MessageOfTheDay messageOfTheDay { juce::URL ("https://schrammel.io/motd/ojd.json"), ProjectInfo::versionNumber };
    std::future<jb::MessageOfTheDay::InfoAndUpdate> infoAndUpdateMessage;

    void checkForMessageOfTheDay();
    void recalculateFilters();
    void updateParametersForProcessorChain();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OJDAudioProcessor)
};
