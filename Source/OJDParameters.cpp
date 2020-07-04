/*
  ==============================================================================

    OJDParameters.cpp
    Created: 14 Feb 2020 7:23:47pm
    Author:  Janos Buttgereit

  ==============================================================================
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
juce::String stringWithMaxLength (juce::String&& s, int maximumStringLength)
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
#define MAKE_ROTARY_PARAMETER(Name)                                                          \
    std::make_unique<juce::AudioParameterFloat> (Sliders::Name::id, #Name, Sliders::displayRange, 5.0f)

#define MAKE_SWITCH_PARAMETER(Name, displayName)                                             \
    std::make_unique<juce::AudioParameterBool>  (Switches::Name::id, displayName, false, "", \
                                                 Switches::Name::stringFromBoolConversion,   \
                                                 Switches::Name::boolFromStringConversion)

    return juce::AudioProcessorValueTreeState::ParameterLayout (
    {
        MAKE_ROTARY_PARAMETER (Drive),
        MAKE_ROTARY_PARAMETER (Tone),
        MAKE_ROTARY_PARAMETER (Volume),

        MAKE_SWITCH_PARAMETER (HpLp,   "HP / LP"),
        MAKE_SWITCH_PARAMETER (Bypass, "Bypass")
    });
}

juce::StringArray OJDParameters::getPresetMangagerParameters()
{
    return { Sliders::Drive::id, Sliders::Tone::id, Sliders::Volume::id, Switches::HpLp::id };
}