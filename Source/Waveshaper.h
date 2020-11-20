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

class Waveshaper : public juce::dsp::ProcessorBase
{
public:
    Waveshaper ()
    {
        // This is where the magic happens :D
        waveshaper.functionToUse = [] (float in)
        {
            float out = in;

            if (in <= -1.7f)
                out = -1.0f;
            else if ((in > -1.7f) && (in < -0.3f))
            {
                in += 0.3f;
                out = in + (in * in) / (4 * (1 - 0.3f)) - 0.3f;
            }
            else if ((in > 0.9f) && (in < 1.1f))
            {
                in -= 0.9f;
                out = in - (in * in) / (4 * (1 - 0.9f)) + 0.9f;
            }
            else if (in > 1.1f)
                out = 1.0f;

            return out;
        };
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override
    {
        oversampler.initProcessing (spec.maximumBlockSize);
    }

    void process (const juce::dsp::ProcessContextReplacing<float>& context) override
    {
        // First sample up...
        auto oversampledBlock = oversampler.processSamplesUp (context.getInputBlock());
        // Then process with the waveshaper...
        waveshaper.process (juce::dsp::ProcessContextReplacing<float> (oversampledBlock));
        // Finally sample back down
        oversampler.processSamplesDown (context.getOutputBlock());
    }

    void reset() override { oversampler.reset(); }

    float getLatencyInSamples() { return oversampler.getLatencyInSamples(); }

private:
    static const size_t numChannels = 1;
    static const size_t oversamplingOrder = 4;
    static const int    oversamplingFactor = 1 << oversamplingOrder;
    static const auto filterType = juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR;

    juce::dsp::Oversampling<float> oversampler {numChannels, oversamplingOrder, filterType};
    juce::dsp::WaveShaper<float> waveshaper;
};