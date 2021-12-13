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

#include "OJDPedalComponent.h"
#include "OJDAudioProcessorEditor.h"

OJDPedalComponent::OJDPedalComponent (OJDAudioProcessor &proc, OJDAudioProcessorEditor& e)
        : editor       (e),
          housing      (BinaryData::pedalHousing_svg, BinaryData::pedalHousing_svgSize),
          driveSlider  (proc.parameters, OJDParameters::Sliders::Drive::id),
          toneSlider   (proc.parameters, OJDParameters::Sliders::Tone::id),
          volumeSlider (proc.parameters, OJDParameters::Sliders::Volume::id),
          bypassLED    (proc.parameters, OJDParameters::Switches::Bypass::id, BinaryData::ledOn_svg,            BinaryData::ledOn_svgSize,            BinaryData::ledOff_svg, BinaryData::ledOff_svgSize, "LED"),
          bypassSwitch (proc.parameters, OJDParameters::Switches::Bypass::id, BinaryData::bypassBackground_svg, BinaryData::bypassBackground_svgSize, juce::Rectangle<float> (0.3f, 0.1f, 0.9f, 0.9f)),
          hpLpSwitch   (proc.parameters, OJDParameters::Switches::HpLp::id,   BinaryData::hpLpBackground_svg,   BinaryData::hpLpBackground_svgSize,   juce::Rectangle<float> (0.05f, 0.05f, 0.9f, 0.9f))
{
    housingShadow.setImage (juce::ImageFileFormat::loadFrom (BinaryData::pedalHousingShadow_png, BinaryData::pedalHousingShadow_pngSize));
    shadowOverlay.setImage (juce::ImageFileFormat::loadFrom (BinaryData::shadowOverlay_png, BinaryData::shadowOverlay_pngSize));

    addAndMakeVisible (housingShadow);
    addAndMakeVisible (housing);

    addAndMakeVisible (shadowOverlay);

    addSliderAndSetStyle (volumeSlider);
    addSliderAndSetStyle (driveSlider);
    addSliderAndSetStyle (toneSlider);

    addBypassElementsAndSetStyle();

    addHpLpSwitchAndSetStyle();
}

void OJDPedalComponent::resized()
{
    auto bounds = getLocalBounds();

    auto hb = jb::scaledKeepingCentre (bounds, 0.94f);
    housing.setBounds (hb);
    housingShadow.setBounds (hb);
    shadowOverlay.setBounds (hb);

    layouts.recalculate (bounds);

    // set sizes
    hpLpSwitch  .setSize (layouts.hpLpWidth,   layouts.hpLpHeight);
    bypassSwitch.setSize (layouts.bypassWidth, layouts.bypassHeight);
    bypassLED   .setSize (layouts.ledWidth,    layouts.ledHeight);
    volumeSlider.setSize (layouts.volumeWidth, layouts.volumeHeight);
    driveSlider .setSize (layouts.driveWidth,  layouts.driveHeight);
    toneSlider  .setSize (layouts.toneWidth,   layouts.toneHeight);

    // setCentres
    hpLpSwitch  .setCentrePosition (layouts.hpLpCentre);
    bypassSwitch.setCentrePosition (layouts.bypassCentre);
    bypassLED   .setCentrePosition (layouts.ledCentre);
    volumeSlider.setCentrePosition (layouts.volumeCentre);
    driveSlider .setCentrePosition (layouts.driveCentre);
    toneSlider  .setCentrePosition (layouts.toneCentre);
}

void OJDPedalComponent::addSliderAndSetStyle (AttachedSlider& slider)
{
    slider.setSliderStyle  (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    slider.setColour       (juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    slider.setPopupDisplayEnabled (true, true, this);

    addAndMakeVisible (slider);
    editor.registerHighlightableWidget (slider);
}

void OJDPedalComponent::addBypassElementsAndSetStyle()
{
    bypassSwitch.setClickingTogglesState (true);

    bypassLED.setClickingTogglesState (false);

    addAndMakeVisible (bypassSwitch);
    addAndMakeVisible (bypassLED);

    editor.registerHighlightableWidget (bypassSwitch);
}

void OJDPedalComponent::addHpLpSwitchAndSetStyle()
{
    hpLpSwitch.setClickingTogglesState (true);

    addAndMakeVisible (hpLpSwitch);

    editor.registerHighlightableWidget (hpLpSwitch);
}

void OJDPedalComponent::SubcomponentLayouts::recalculate (juce::Rectangle<int> bounds)
{
    hpLpCentre.x   = bounds.proportionOfWidth (0.5f);
    bypassCentre.x = bounds.proportionOfWidth (0.57f);
    ledCentre.x    = bounds.proportionOfWidth (0.5f);
    volumeCentre.x = bounds.proportionOfWidth (0.34f);
    driveCentre.x  = bounds.proportionOfWidth (0.68f);
    toneCentre.x   = bounds.proportionOfWidth (0.507f);

    hpLpCentre.y   = bounds.proportionOfHeight (0.31f);
    bypassCentre.y = bounds.proportionOfHeight (0.833f);
    ledCentre.y    = bounds.proportionOfHeight (0.11f);
    volumeCentre.y = bounds.proportionOfHeight (0.225f);
    driveCentre.y  = bounds.proportionOfHeight (0.225f);
    toneCentre.y   = bounds.proportionOfHeight (0.433f);

    hpLpWidth   = bounds.proportionOfWidth (0.13f);
    bypassWidth = bounds.proportionOfWidth (0.13f);
    ledWidth    = bounds.proportionOfWidth (0.13f);
    volumeWidth = bounds.proportionOfWidth (0.27f);
    driveWidth  = bounds.proportionOfWidth (0.27f);
    toneWidth   = bounds.proportionOfWidth (0.27f);

    hpLpHeight   = bounds.proportionOfHeight (0.0362f);
    bypassHeight = bounds.proportionOfHeight (0.036f);
    ledHeight    = bounds.proportionOfHeight (0.155f);
    volumeHeight = bounds.proportionOfHeight (0.14f);
    driveHeight  = bounds.proportionOfHeight (0.14f);
    toneHeight   = bounds.proportionOfHeight (0.14f);
}