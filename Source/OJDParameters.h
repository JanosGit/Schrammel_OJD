/*
  ==============================================================================

    OJDParameters.h
    Created: 14 Feb 2020 7:15:11pm
    Author:  Janos Buttgereit

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ToneStack.h"

/**
 * A class containing all parameter-related things, e.g. parameter IDs, normalizable ranges,
 * conversion functions used by the various AudioParameter classes and conversion functions
 * to convert the atomic raw values into meaningful states.
 */
struct OJDParameters
{
    struct Drive
    {
        static const juce::String                   id;
        static const juce::NormalisableRange<float> range;
    };

    struct Tone
    {
        static const juce::String                   id;
        static const juce::NormalisableRange<float> range;
    };

    struct Volume
    {
        static const juce::String                   id;
        static const juce::NormalisableRange<float> range;
    };

    class HpLp
    {
    public:
        static const juce::String id;

        /** Returns if the tone stack should work in LP or HP mode */
        static ToneStack::Mode getModeFromRaw (const std::atomic<float>& rawValue);

    private:
        friend OJDParameters;
        static juce::String stringFromBoolConversion (bool isHp, int maximumStringLength);
        static bool         boolFromStringConversion (const juce::String& text);
    };

    class Bypass
    {
    public:
        static const juce::String id;

        /** Returns true if the bypass is active, e.g. if audio should be bypassed */
        static bool isActive (const std::atomic<float>& rawValue);

    private:
        friend OJDParameters;
        static juce::String stringFromBoolConversion (bool isBypassed, int maximumStringLength);
        static bool         boolFromStringConversion (const juce::String& text);
    };

    /** Supplies the parameter layout used to create the AudioProcessorValueTreeState */
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    /**
     * The preset manager will listen to all parameters in this array. Changing those will make the
     * preset state turn "dirty"
     */
    static juce::StringArray getPresetMangagerParameters();
};