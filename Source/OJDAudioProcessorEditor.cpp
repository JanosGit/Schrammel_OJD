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

#include "OJDAudioProcessorEditor.h"

//==============================================================================
OJDAudioProcessorEditor::OJDAudioProcessorEditor (OJDAudioProcessor& proc)
 :  jb::PluginEditorBase<contentMinWidth, overallMinHeight> (proc, IsResizable::Yes, UseConstrainer::Yes),
    background       (BinaryData::background_svg, BinaryData::background_svgSize),
    pedal            (proc, *this),
    activeView       (ActiveView::pedal),
    messageOkButton  ("OK"),
    messageLearnMoreButton ("Learn more"),
    settingsButton   ("Settings", juce::DrawableButton::ButtonStyle::ImageFitted),
    knobDrawable     (juce::Drawable::createFromImageData (BinaryData::knob_svg, BinaryData::knob_svgSize)),
    ojdLookAndFeel   (*knobDrawable)
{
    setLookAndFeel (&ojdLookAndFeel);

    addAndMakeVisible (background);
    addAndMakeVisible (pedal);

    addPresetManager (proc);
    setupAndAddMessageOfTheDayComponents();
    setupAndAddSettingsPageComponents();

    restoreSizeFromState();

    checkMessageOfTheDay (proc);
}

OJDAudioProcessorEditor::~OJDAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void OJDAudioProcessorEditor::constrainedResized()
{
    presetManagerComponent->setBounds (getLocalBounds().removeFromTop (presetManagerComponentHeight));

    auto contentBounds = getLocalBounds().withTrimmedTop (presetManagerComponentHeight);
    background.setBounds (contentBounds);

    auto pedalBounds = jb::scaledKeepingCentre (contentBounds, 0.85f);

    if (activeView != ActiveView::pedal)
        pedalBounds.translate (proportionOfWidth (0.8f), 0);

    if (activeView == ActiveView::message)
    {
        auto messageBounds = contentBounds.reduced (proportionOfHeight (0.05f));

        messageEditor.applyFontToAllText(getHeight() * 0.025f);
        messageEditor.setBounds (messageBounds.removeFromTop (proportionOfHeight (0.8f)));

        messageBounds.removeFromTop (proportionOfHeight (0.01f));

        auto okBounds = messageBounds.removeFromLeft (messageBounds.getWidth() / 2);
        auto learnMoreBounds = messageBounds;
        messageOkButton.setBounds (okBounds.reduced (proportionOfHeight (0.01f)));
        messageLearnMoreButton.setBounds (learnMoreBounds.reduced (proportionOfHeight (0.01f)));
    }

    settingsButton.setBoundsRelative (0.9f, 0.015f, 0.08f, 0.12f);

    pedal.setBounds (pedalBounds);
}

void OJDAudioProcessorEditor::checkBounds (juce::Rectangle<int>& bounds, const juce::Rectangle<int>&, const juce::Rectangle<int>&, bool, bool, bool, bool)
{
    auto contentHeight = juce::jmax (contentMinHeight, bounds.getHeight() - presetManagerComponentHeight);
    auto contentWidth  = juce::jmax (contentMinWidth, juce::roundToInt (contentHeight * contentAspectRatio));

    bounds.setSize (contentWidth, contentHeight + presetManagerComponentHeight);
}

void OJDAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll (juce::Colours::white);
}

void OJDAudioProcessorEditor::addPresetManager (OJDAudioProcessor& processorToControl)
{
    presetManagerComponent = processorToControl.stateAndPresetManager.createPresetManagerComponent (*this, true);
    addAndMakeVisible (*presetManagerComponent);
}

void OJDAudioProcessorEditor::setupAndAddMessageOfTheDayComponents()
{
    addChildComponent(messageEditor);
    addChildComponent (messageOkButton);
    addChildComponent (messageLearnMoreButton);

    messageEditor.setReadOnly (true);
    messageEditor.setColour (juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
    messageEditor.setColour (juce::TextEditor::ColourIds::outlineColourId, juce::Colours::transparentBlack);
    messageEditor.setColour (juce::TextEditor::ColourIds::textColourId, juce::Colours::white);

    messageEditor.setJustification (juce::Justification::centred);
    messageEditor.setMultiLine (true);

    messageOkButton.setColour (juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);
    messageLearnMoreButton.setColour (juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);

    messageOkButton.onClick = [this]()
    {
        activeView = ActiveView::pedal;

        messageEditor.setVisible (false);
        messageOkButton.setVisible (false);
        messageLearnMoreButton.setVisible (false);

        constrainedResized();
        repaint();
    };
}

void OJDAudioProcessorEditor::setupAndAddSettingsPageComponents()
{
    addAndMakeVisible(settingsButton);

    auto settingsIcon = juce::Drawable::createFromImageData (BinaryData::settingsoption_svg,
                                                             BinaryData::settingsoption_svgSize);

    settingsButton.setImages (settingsIcon.get());
    settingsButton.onClick = [this]()
    {
        activeView = (activeView == ActiveView::pedal) ? ActiveView::settings : ActiveView::pedal;

        constrainedResized();
        repaint();
    };
}

void OJDAudioProcessorEditor::checkMessageOfTheDay (OJDAudioProcessor& proc)
{
    juce::Timer::callAfterDelay(500, [&]()
    {
        juce::String motdVersion = "LastMOTDVersionDisplayed";
        auto& settingsManager = *jb::SettingsManager::getInstance();

        if (!settingsManager.settingExists(motdVersion))
        {
            juce::String welcomeMessage =
R"(
Welcome!

Thanks for downloading the OJD. It's a completely free and open source distortion plugin, modeled after a well known analogue guitar effects pedal.

For best sounding results, I suggest you to put this somewhere before your guitar amp (plugin) and before reverb, delay and modulation plugins, just as you would place a real pedal on your pedal board. Being creative and using it somewhere else in your chain is possible too but be prepared that this will cause a lot of sharp high frequencies.

There are two basic sound characteristics you can select with the upper mid slide switch. The LP mode gives you a warmer tone with slightly less gain, the HP mode boost some higher frequencies, leading to a more aggressive distortion with some more gain. Just play around with the knobs until you find a setting you like. So, let's go!
)";
            setMessage (welcomeMessage, juce::URL ("https://schrammel.io"));
            settingsManager.writeSetting (motdVersion, int64_t (0));
        }
        else if (auto messages = proc.getMessageOfTheDay (500))
        {
            auto updateMessage  = std::move (messages->updateMessage);
            auto generalMessage = std::move (messages->generalMessage);

            if (updateMessage != nullptr)
            {
                setMessage ("OJD Update Available\n\n" + updateMessage->text, updateMessage->link);
                return;
            }

            if (generalMessage != nullptr)
            {
                setMessage ("Info\n\n" + generalMessage->text, generalMessage->link);
                settingsManager.writeSetting (motdVersion, generalMessage->version);
            }
        }
    });
}

void OJDAudioProcessorEditor::setMessage (const juce::String& text, const juce::URL url)
{
    messageEditor.setText (text);

    messageLearnMoreButton.onClick = [url] ()
    {
        url.launchInDefaultBrowser();
    };

    messageEditor.setVisible (true);
    messageOkButton.setVisible (true);
    messageLearnMoreButton.setVisible (true);

    activeView = ActiveView::message;

    constrainedResized();
    repaint();
}
