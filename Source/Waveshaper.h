/*
  ==============================================================================

    Waveshaper.h
    Created: 13 Feb 2020 9:37:05pm
    Author:  Janos Buttgereit

  ==============================================================================
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

    ~Waveshaper() { }

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