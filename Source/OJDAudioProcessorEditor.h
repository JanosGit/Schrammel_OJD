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

#include "SettingsPage.h"
#include "OJDPedalComponent.h"
#include "OJDLookAndFeel.h"

class OJDAudioProcessorEditor  : public jb::PluginEditorBase<OJDEditorConstants::contentMinWidth, OJDEditorConstants::overallMinHeight>,
                                 public OJDEditorConstants
{
public:
    OJDAudioProcessorEditor (OJDAudioProcessor&);
    ~OJDAudioProcessorEditor() override;

    //==============================================================================
    void constrainedResized() override;
    void checkBounds (juce::Rectangle<int>& bounds, const juce::Rectangle<int>&, const juce::Rectangle<int>&, bool, bool, bool, bool) override;
    void paint (juce::Graphics& g) override;

private:
    void addPresetManager (OJDAudioProcessor& processorToControl);
    void setupAndAddMessageOfTheDayComponents();
    void setupAndAddSettingsPageComponents();

    void checkMessageOfTheDay (OJDAudioProcessor& processor);
    void setMessage (const juce::String& text, const juce::URL url);

    enum class ActiveView
    {
        message,
        pedal,
        settings
    };

    jb::SVGComponent background;

    OJDPedalComponent pedal;
    SettingsPage settingsPage;

    ActiveView activeView;
    juce::TextEditor messageEditor;
    juce::TextButton messageOkButton;
    juce::TextButton messageLearnMoreButton;
    juce::DrawableButton settingsButton;

    const std::unique_ptr<juce::Drawable> knobDrawable;

    OJDLookAndFeel ojdLookAndFeel;

    std::unique_ptr<jb::PresetManagerComponent> presetManagerComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OJDAudioProcessorEditor)
};
