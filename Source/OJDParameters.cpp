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

#include "OJDParameters.h"

//================ IDs =================================================================================================
const juce::String OJDParameters::Sliders::Drive::id  ("Drive");
const juce::String OJDParameters::Sliders::Tone::id   ("Tone");
const juce::String OJDParameters::Sliders::Volume::id ("Volume");
const juce::String OJDParameters::Switches::HpLp::id   ("HpLp");
const juce::String OJDParameters::Switches::Bypass::id ("Bypass");


//================ Ranges ==============================================================================================
constexpr float minDisplayRange = 0.0f;
constexpr float maxDisplayRange = 10.0f;
constexpr float minVolumeDb = -60.0f;
constexpr float maxVolumeDb = -20.0f;

const juce::NormalisableRange<float> OJDParameters::Sliders::displayRange (minDisplayRange, maxDisplayRange, 0.01f);

float OJDParameters::Sliders::normaliseRawValue (const std::atomic<float>& rawValue)
{
    return rawValue / maxDisplayRange;
}

float OJDParameters::Sliders::Volume::dBValueFromRawValue (const std::atomic<float>& rawValue)
{
    return juce::jmap (rawValue.load(), minDisplayRange, maxDisplayRange, minVolumeDb, maxVolumeDb);
}

//================ String <-> value conversion =========================================================================
static juce::String stringWithMaxLength (juce::String&& s, int maximumStringLength)
{
    return maximumStringLength > 0 ? s.substring (0, maximumStringLength) : s;
}

juce::String OJDParameters::Switches::HpLp::stringFromBoolConversion (bool isHp, int maximumStringLength)
{
    return stringWithMaxLength (isHp ? "HP" : "LP", maximumStringLength) ;
}

juce::String OJDParameters::Switches::Bypass::stringFromBoolConversion (bool isBypassed, int maximumStringLength)
{
    return stringWithMaxLength (isBypassed ? "Off" : "On", maximumStringLength);
}

bool OJDParameters::Switches::HpLp::boolFromStringConversion (const juce::String& text)
{
    return !text.compareIgnoreCase ("hp");
}

bool OJDParameters::Switches::Bypass::boolFromStringConversion (const juce::String& text)
{
    return !text.compareIgnoreCase ("on");
}

//================ Raw parameter to meaningful value conversion ========================================================
ToneStack::Mode OJDParameters::Switches::HpLp::getModeFromRaw (const std::atomic<float>& rawValue)
{
    return rawValue > 0.5 ? ToneStack::hp : ToneStack::lp;
}

bool OJDParameters::Switches::Bypass::isActive (const std::atomic<float>& rawValue)
{
    return rawValue > 0.5;
}


//================ Parameter layout creation ===========================================================================
juce::AudioProcessorValueTreeState::ParameterLayout OJDParameters::createParameterLayout()
{
#define MAKE_ROTARY_PARAMETER(Name, defaultValue) \
    std::make_unique<juce::AudioParameterFloat> (juce::ParameterID (Sliders::Name::id, 1), #Name, Sliders::displayRange, defaultValue)

#define MAKE_SWITCH_PARAMETER(Name, displayName)                                                                    \
    std::make_unique<juce::AudioParameterBool>  (juce::ParameterID (Switches::Name::id, 1), displayName, false, "", \
                                                 Switches::Name::stringFromBoolConversion,                          \
                                                 Switches::Name::boolFromStringConversion)

    return juce::AudioProcessorValueTreeState::ParameterLayout (
    {
        MAKE_ROTARY_PARAMETER (Drive,  1.65f),
        MAKE_ROTARY_PARAMETER (Tone,   5.0f),
        MAKE_ROTARY_PARAMETER (Volume, 8.35),

        MAKE_SWITCH_PARAMETER (HpLp,   "HP / LP"),
        MAKE_SWITCH_PARAMETER (Bypass, "Bypass")
    });
}

juce::StringArray OJDParameters::getPresetManagerParameters()
{
    return { Sliders::Drive::id, Sliders::Tone::id, Sliders::Volume::id, Switches::HpLp::id };
}