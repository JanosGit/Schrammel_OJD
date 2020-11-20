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
#include "ToneStack.h"

/**
 * A class containing all parameter-related things, e.g. parameter IDs, normalizable ranges,
 * conversion functions used by the various AudioParameter classes and conversion functions
 * to convert the atomic raw values into meaningful states.
 */
struct OJDParameters
{
    struct Sliders
    {
        /** All three rotary sliders display a typical guitar equipment value range from 0 to 10 */
        static const juce::NormalisableRange<float> displayRange;

        /** Takes the raw 0 to 10 parameter value and returns a 0 to 1 value */
        static float normaliseRawValue (const std::atomic<float>& rawValue);

        struct Drive
        {
            static const juce::String                   id;
        };

        struct Tone
        {
            static const juce::String                   id;
        };

        struct Volume
        {
            static const juce::String                   id;

            /** Takes the raw 0 to 10 parameter value and returns a -60dB to -20dB value */
            static float dBValueFromRawValue (const std::atomic<float>& rawValue);
        };
    };

    struct Switches
    {
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
    };

    /** Used to report the Bypass parameter to PluginAudioProcessorBase */
    using Bypass = Switches::Bypass;

    /** Supplies the parameter layout used to create the AudioProcessorValueTreeState */
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    /**
     * The preset manager will listen to all parameters in this array. Changing those will make the
     * preset state turn "dirty"
     */
    static juce::StringArray getPresetMangagerParameters();
};