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

class ToneStack : public juce::dsp::ProcessorBase
{
public:
    enum Mode
    {
        hp,
        lp
    };

    ToneStack() = default;

    void prepare (const juce::dsp::ProcessSpec& spec) override
    {
        hpfTempBuffer.setSize (static_cast<int> (spec.numChannels), static_cast<int> (spec.maximumBlockSize));
        hpfTempBlock = std::make_unique<juce::dsp::AudioBlock<float>> (hpfTempBuffer);

        constexpr auto hpModeFreq = 358.0f;
        constexpr auto lpModeFreq = 160.0f;

        hpfCoeffsHPMode = juce::dsp::IIR::ArrayCoefficients<float>::makeFirstOrderHighPass (spec.sampleRate, hpModeFreq);
        hpfCoeffsLPMode = juce::dsp::IIR::ArrayCoefficients<float>::makeFirstOrderHighPass (spec.sampleRate, lpModeFreq);

        lpfCoeffsHPMode = juce::dsp::IIR::ArrayCoefficients<float>::makeFirstOrderLowPass (spec.sampleRate, hpModeFreq);
        lpfCoeffsLPMode = juce::dsp::IIR::ArrayCoefficients<float>::makeFirstOrderLowPass (spec.sampleRate, lpModeFreq);

        *hpf.state = currentMode == hp ? hpfCoeffsHPMode : hpfCoeffsLPMode;
        *lpf.state = currentMode == hp ? lpfCoeffsHPMode : lpfCoeffsLPMode;

        hpf.prepare (spec);
        lpf.prepare (spec);
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

            *hpf.state = newMode == hp ? hpfCoeffsHPMode : hpfCoeffsLPMode;
            *lpf.state = newMode == hp ? lpfCoeffsHPMode : lpfCoeffsLPMode;
        }
    }

    /** Takes the normalised 0-1 Tone value */
    void setTone (float newTone)    { tone = newTone; }

private:
    Mode currentMode = lp;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> hpf, lpf;

    std::array<float, 4> hpfCoeffsHPMode, hpfCoeffsLPMode;
    std::array<float, 4> lpfCoeffsHPMode, lpfCoeffsLPMode;

    juce::dsp::Gain<float> hpfGain;
    float tone = 1.0f;

    juce::AudioBuffer<float> hpfTempBuffer;
    std::unique_ptr<juce::dsp::AudioBlock<float>> hpfTempBlock;
};