/*
  ==============================================================================

    Drawables.h
    Created: 11 Jun 2020 1:27:50am
    Author:  Janos Buttgereit

  ==============================================================================
*/

#pragma once

/**
 * A helper class holding all Drawables needed for the Plugin Editor.
 * It loads all its members form BinaryData in the constructor and aquires
 * a message manager lock for each loading step. Its intention is to be created
 * once by the processor. The Editor just gets a reference to the instance held
 * by the processor. This way opening the Editor works quick.
 */
class Drawables
{
public:

    Drawables()
      : editorBackground (createDrawableFromImageDataLocked (BinaryData::background_svg,    BinaryData::background_svgSize)),
        knob             (createDrawableFromImageDataLocked (BinaryData::knob_svg,          BinaryData::knob_svgSize)),
        slideSwitchHP    (createDrawableFromImageDataLocked (BinaryData::slideSwitchHP_svg, BinaryData::slideSwitchHP_svgSize)),
        slideSwitchLP    (createDrawableFromImageDataLocked (BinaryData::slideSwitchLP_svg, BinaryData::slideSwitchLP_svgSize)),
        footSwitch       (createDrawableFromImageDataLocked (BinaryData::footSwitch_svg,    BinaryData::footSwitch_svgSize)),
        ledOn            (createDrawableFromImageDataLocked (BinaryData::ledOn_svg,         BinaryData::ledOn_svgSize)),
        ledOff           (createDrawableFromImageDataLocked (BinaryData::ledOff_svg,        BinaryData::ledOff_svgSize))
    {
        jassert (editorBackground != nullptr);
        jassert (knob             != nullptr);
        jassert (slideSwitchLP    != nullptr);
        jassert (slideSwitchHP    != nullptr);
        jassert (footSwitch       != nullptr);
        jassert (ledOn            != nullptr);
        jassert (ledOff           != nullptr);
    }

    const std::unique_ptr<juce::Drawable> editorBackground;
    const std::unique_ptr<juce::Drawable> knob;
    const std::unique_ptr<juce::Drawable> slideSwitchHP;
    const std::unique_ptr<juce::Drawable> slideSwitchLP;
    const std::unique_ptr<juce::Drawable> footSwitch;
    const std::unique_ptr<juce::Drawable> ledOn;
    const std::unique_ptr<juce::Drawable> ledOff;

private:
    static std::unique_ptr<juce::Drawable> createDrawableFromImageDataLocked (const void* data, size_t numBytes)
    {
        const juce::MessageManagerLock mml;
        return juce::Drawable::createFromImageData (data, numBytes);
    }
};