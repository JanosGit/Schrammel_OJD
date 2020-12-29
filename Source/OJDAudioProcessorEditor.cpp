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

#include "OJDProcessor.h"
#include "OJDAudioProcessorEditor.h"

juce::Rectangle<int> scaledKeepingCentre (const juce::Rectangle<int>& src, float scale)
{
    auto centre = src.getCentre().toFloat();
    auto scaled = src.toFloat() * scale;

    scaled.setCentre (centre);

    return scaled.toNearestInt();
}

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

    auto hb = scaledKeepingCentre (bounds, 0.94f);
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

//==============================================================================
OJDAudioProcessorEditor::OJDAudioProcessorEditor (OJDAudioProcessor& proc)
 :  jb::PluginEditorBase<contentMinWidth, overallMinHeight> (proc, IsResizable::Yes, UseConstrainer::Yes),
    background       (BinaryData::background_svg, BinaryData::background_svgSize),
    pedal            (proc, *this),
    isInMessageState (false),
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

    auto pedalBounds = scaledKeepingCentre (contentBounds, 0.85f);

    if (isInMessageState)
    {
        pedalBounds.translate (proportionOfWidth (0.8f), 0);

        auto messageBounds = contentBounds.reduced (proportionOfHeight (0.05f));

        messageEditor.applyFontToAllText(getHeight() * 0.025f);
        messageEditor.setBounds (messageBounds.removeFromTop (proportionOfHeight (0.8f)));

        messageBounds.removeFromTop (proportionOfHeight (0.01f));

        auto okBounds = messageBounds.removeFromLeft (messageBounds.getWidth() / 2);
        auto learnMoreBounds = messageBounds;
        messageOkButton.setBounds (okBounds.reduced (proportionOfHeight (0.01f)));
        messageLearnMoreButton.setBounds (learnMoreBounds.reduced (proportionOfHeight (0.01f)));
    }

    settingsButton.setBoundsRelative (0.9f, 0.91f, 0.08f, 0.12f);

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
        isInMessageState = false;

        messageEditor.setVisible (false);
        messageOkButton.setVisible (false);
        messageLearnMoreButton.setVisible (false);

        constrainedResized();
        repaint();
    };
}

void OJDAudioProcessorEditor::setupAndAddSettingsPageComponents()
{
    addAndMakeVisible (settingsButton);

    auto settingsIcon = juce::Drawable::createFromImageData (BinaryData::settingsoption_svg,
                                                             BinaryData::settingsoption_svgSize);

    settingsButton.setImages (settingsIcon.get());
    settingsButton.onClick = [this]()
    {
        
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

    isInMessageState = true;

    constrainedResized();
    repaint();
}
