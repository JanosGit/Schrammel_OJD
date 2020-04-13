/*
  ==============================================================================

    ToneStack.h
    Created: 13 Feb 2020 9:37:13pm
    Author:  Janos Buttgereit

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ToneStack : public juce::dsp::ProcessorBase
{
public:
    enum Mode
    {
        hp,
        lp
    };

    ToneStack() {}

    void prepare (const juce::dsp::ProcessSpec& spec) override
    {
        hpfTempBuffer.setSize (static_cast<int> (spec.numChannels), static_cast<int> (spec.maximumBlockSize));
        hpfTempBlock = std::make_unique<juce::dsp::AudioBlock<float>> (hpfTempBuffer);

        const auto hpModeFreq = 358.0f;
        const auto lpModeFreq = 160.0f;

        hpfCoeffsHPMode = juce::dsp::IIR::Coefficients<float>::makeFirstOrderHighPass (spec.sampleRate, hpModeFreq);
        hpfCoeffsLPMode = juce::dsp::IIR::Coefficients<float>::makeFirstOrderHighPass (spec.sampleRate, lpModeFreq);

        lpfCoeffsHPMode = juce::dsp::IIR::Coefficients<float>::makeFirstOrderLowPass (spec.sampleRate, hpModeFreq);
        lpfCoeffsLPMode = juce::dsp::IIR::Coefficients<float>::makeFirstOrderLowPass (spec.sampleRate, lpModeFreq);
    }

    void process (const juce::dsp::ProcessContextReplacing<float>& context) override
    {
        // Update the gain value depending on the current mode
        hpfGain.setGainLinear (currentMode == hp ? 0.7f * tone : 0.2f * tone);

        // First process the highpass filter and store its result to a second block
        hpf.process (juce::dsp::ProcessContextNonReplacing<float> (context.getInputBlock(), *hpfTempBlock));
        // Now process the lowpass filter in place
        lpf.process (context);

        // Now process the hpf Gain and add the result to the original context, already containing the lpf result
        hpfGain.process (juce::dsp::ProcessContextReplacing<float> (*hpfTempBlock));
        context.getOutputBlock().add (*hpfTempBlock);
    }

    void reset() override
    {
        hpf.reset();
        lpf.reset();
    }

    void setHpLpMode (Mode newMode)
    {
        if (currentMode != newMode)
        {
            currentMode = newMode;

            hpf.coefficients = newMode == hp ? hpfCoeffsHPMode : hpfCoeffsLPMode;
            lpf.coefficients = newMode == hp ? lpfCoeffsHPMode : lpfCoeffsLPMode;
        }
    }

    /** Takes the normalized 0-1 Tone value */
    void setTone (float newTone)    { tone = newTone; }

private:
    Mode currentMode;

    juce::dsp::IIR::Filter<float> hpf, lpf;

    juce::dsp::IIR::Coefficients<float>::Ptr hpfCoeffsHPMode, hpfCoeffsLPMode;
    juce::dsp::IIR::Coefficients<float>::Ptr lpfCoeffsHPMode, lpfCoeffsLPMode;

    juce::dsp::Gain<float> hpfGain;
    float tone = 1.0f;

    juce::AudioBuffer<float> hpfTempBuffer;
    std::unique_ptr<juce::dsp::AudioBlock<float>> hpfTempBlock;
};