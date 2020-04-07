/*
  ==============================================================================

    OJDParameters.cpp
    Created: 14 Feb 2020 7:23:47pm
    Author:  Janos Buttgereit

  ==============================================================================
*/

#include "OJDParameters.h"

//================ IDs =================================================================================================
const juce::String OJDParameters::Drive::id  ("Drive");
const juce::String OJDParameters::Tone::id   ("Tone");
const juce::String OJDParameters::Volume::id ("Volume");
const juce::String OJDParameters::HpLp::id   ("HpLp");
const juce::String OJDParameters::Bypass::id ("Bypass");


//================ Ranges ==============================================================================================
using FloatRange = juce::NormalisableRange<float>;

const FloatRange OJDParameters::Drive::range  (1.0f, 10.0f, 0.01f);
const FloatRange OJDParameters::Tone::range   (0.5f, 15.0f, 0.01f);
const FloatRange OJDParameters::Volume::range (-60.0f, -20.0f, 0.01f);


//================ String <-> value conversion =========================================================================
juce::String stringWithMaxLength (const juce::String& s, int maximumStringLength)
{
    auto len = s.length();

    if (len > maximumStringLength)
        return s.substring (0, maximumStringLength - 1);

    return s;
}

juce::String OJDParameters::HpLp::stringFromBoolConversion (bool isHp, int maximumStringLength)
{
    return stringWithMaxLength (isHp ? "HP" : "LP", maximumStringLength) ;
}

juce::String OJDParameters::Bypass::stringFromBoolConversion (bool isBypassed, int maximumStringLength)
{
    return stringWithMaxLength (isBypassed ? "Off" : "On", maximumStringLength);
}

bool OJDParameters::HpLp::boolFromStringConversion   (const juce::String& text) { return !text.compareIgnoreCase ("hp"); }
bool OJDParameters::Bypass::boolFromStringConversion (const juce::String& text) { return !text.compareIgnoreCase ("On"); }

//================ Raw parameter to meaningful value conversion ========================================================
ToneStack::Mode OJDParameters::HpLp::getModeFromRaw (const std::atomic<float>& rawValue) { return rawValue > 0.5 ? ToneStack::hp : ToneStack::lp; }
bool            OJDParameters::Bypass::isActive     (const std::atomic<float>& rawValue) { return rawValue > 0.5; }


//================ Parameter layout creation ===========================================================================
juce::AudioProcessorValueTreeState::ParameterLayout OJDParameters::createParameterLayout()
{
    return juce::AudioProcessorValueTreeState::ParameterLayout ({
            std::make_unique<juce::AudioParameterFloat> (Drive::id,  "Drive",   Drive::range,  5.5f),
            std::make_unique<juce::AudioParameterFloat> (Tone::id,   "Tone",    Tone::range,   7.75f),
            std::make_unique<juce::AudioParameterFloat> (Volume::id, "Volume",  Volume::range, -40.0f),
            std::make_unique<juce::AudioParameterBool>  (HpLp::id,   "HP / LP", false, "", HpLp::stringFromBoolConversion,   HpLp::boolFromStringConversion),
            std::make_unique<juce::AudioParameterBool>  (Bypass::id, "Bypass",  false, "", Bypass::stringFromBoolConversion, Bypass::boolFromStringConversion)
    });
}