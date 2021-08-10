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

#include <jb_plugin_base/jb_plugin_base.h>
#include "OJDProcessor.h"
#include "SlideSwitch.h"

class OJDAudioProcessorEditor;

/** Some constants derived from the GUI design */
struct OJDEditorConstants
{
    static constexpr int   presetManagerComponentHeight = 20;
    static constexpr int   contentMinHeight             = 568;
    static constexpr int   overallMinHeight             = presetManagerComponentHeight + contentMinHeight;
    static constexpr int   contentMinWidth              = 310;
    static constexpr float contentAspectRatio           = 0.5876168224f;
};

/** The pedal itself is held in a separate component to be able to move it around on the background e.g. for messages */
class OJDPedalComponent : public juce::Component
{
public:
    OJDPedalComponent (OJDAudioProcessor&, OJDAudioProcessorEditor&);

    void resized() override;

private:

    /** Determines how AAX automation highlight boxes are drawn */
    using HighlightLayout = jb::HighlightableWidget::BoxLayout;

    using AttachedSlider      = jb::AttachedWidget<juce::Slider,  HighlightLayout::SquareCenteredAboveParent>;
    using AttachedLED         = jb::AttachedWidget<jb::SVGButton, HighlightLayout::SquareCenteredAboveParent>;
    using AttachedSlideSwitch = jb::AttachedWidget<SlideSwitch,   HighlightLayout::FollowParentBounds>;

    /** Adds a slider to the component, makes it visible and performs some general styling */
    void addSliderAndSetStyle (AttachedSlider& slider);

    /** Adds the bypass switch to the component and sets its style */
    void addBypassElementsAndSetStyle();

    /** Adds the HP/LP switch to the component and sets its style */
    void addHpLpSwitchAndSetStyle();


    struct SubcomponentLayouts
    {
        void recalculate (juce::Rectangle<int> bounds);

        // Centre Positions
        juce::Point<int> hpLpCentre;
        juce::Point<int> ledCentre;
        juce::Point<int> bypassCentre;
        juce::Point<int> driveCentre;
        juce::Point<int> toneCentre;
        juce::Point<int> volumeCentre;

        // Width
        int hpLpWidth;
        int ledWidth;
        int bypassWidth;
        int driveWidth;
        int toneWidth;
        int volumeWidth;

        // Height
        int hpLpHeight;
        int ledHeight;
        int bypassHeight;
        int driveHeight;
        int toneHeight;
        int volumeHeight;
    };

    OJDAudioProcessorEditor& editor;

    SubcomponentLayouts layouts;

    juce::ImageComponent housingShadow;
    jb::SVGComponent housing;

    AttachedSlider driveSlider, toneSlider, volumeSlider;

    juce::ImageComponent shadowOverlay;

    AttachedLED bypassLED;
    AttachedSlideSwitch bypassSwitch, hpLpSwitch;
};
