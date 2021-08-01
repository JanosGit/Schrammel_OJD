/*

This file is part of the Schrammel OJD audio plugin.
Copyright (C) 2020  Janos Buttgereit

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OJDParameters.h"
#include "ToneStack.h"
#include "Waveshaper.h"

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

private:
    int numChannels = 1;

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

    using HPF    = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    using LPF    = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    using Biquad = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    using Gain   = juce::dsp::Gain<float>;

    juce::dsp::ProcessorChain<HPF, Biquad, Biquad, Gain, Waveshaper, Biquad, Biquad, Biquad, LPF, ToneStack, Gain> chain;

    // This lock is held for a short time when the drive changes & biquad coefficients are exchanged
    juce::SpinLock biquadParameterLock;
    std::atomic<bool> biquadParametersUpdated { false };

    std::array<float, 6> biquadPreDriveBoostCoeffs;
    std::array<float, 6> biquadPreDriveNotchCoeffs;
    std::array<float, 6> biquadPostDriveBoost1Coeffs;
    std::array<float, 6> biquadPostDriveBoost2Coeffs;
    std::array<float, 6> biquadPostDriveBoost3Coeffs;

    jb::MessageOfTheDay messageOfTheDay { juce::URL ("https://schrammel.io/motd/ojd.json"), ProjectInfo::versionNumber };
    std::future<jb::MessageOfTheDay::InfoAndUpdate> infoAndUpdateMessage;

    void checkForMessageOfTheDay();
    void recalculateFilters();
    void updateParametersForProcessorChain();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OJDAudioProcessor)
};
