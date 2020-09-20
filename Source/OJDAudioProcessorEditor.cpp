

#include "OJDProcessor.h"
#include "OJDAudioProcessorEditor.h"

//==============================================================================
OJDAudioProcessorEditor::OJDAudioProcessorEditor (OJDAudioProcessor& proc)
 :  jb::PluginEditorBase<contentMinWidth, overallMinHeight> (proc, IsResizable::Yes, UseConstrainer::Yes),
    layouts          (*this),
    knobDrawable     (juce::Drawable::createFromImageData (BinaryData::knob_svg, BinaryData::knob_svgSize)),
    ojdLookAndFeel   (*knobDrawable),
    driveSlider      (proc.parameters, OJDParameters::Sliders::Drive::id),
    toneSlider       (proc.parameters, OJDParameters::Sliders::Tone::id),
    volumeSlider     (proc.parameters, OJDParameters::Sliders::Volume::id),
    bypassSwitch     (proc.parameters, OJDParameters::Switches::Bypass::id, BinaryData::footSwitch_svg,    BinaryData::footSwitch_svgSize,    BinaryData::footSwitch_svg,    BinaryData::footSwitch_svgSize, "Bypass"),
    bypassLED        (proc.parameters, OJDParameters::Switches::Bypass::id, BinaryData::ledOff_svg,        BinaryData::ledOff_svgSize,        BinaryData::ledOn_svg,         BinaryData::ledOn_svgSize, "LED"),
    hpLpSwitch       (proc.parameters, OJDParameters::Switches::HpLp::id,   BinaryData::slideSwitchLP_svg, BinaryData::slideSwitchLP_svgSize, BinaryData::slideSwitchHP_svg, BinaryData::slideSwitchHP_svgSize,   "HpLp")
{
    setLookAndFeel (&ojdLookAndFeel);

    addAndMakeVisible (background);

    addSliderAndSetStyle (volumeSlider);
    addSliderAndSetStyle (driveSlider);
    addSliderAndSetStyle (toneSlider);

    addBypassElementsAndSetStyle();

    addHpLpSwitchAndSetStyle();

    addPresetManager (proc);

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

    layouts.recalculate();

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

    background.setBounds (getLocalBounds().withTrimmedTop (presetManagerComponentHeight));
}

void OJDAudioProcessorEditor::checkBounds (juce::Rectangle<int>& bounds, const juce::Rectangle<int>&, const juce::Rectangle<int>&, bool, bool, bool, bool)
{
    auto contentHeight = juce::jmax (contentMinHeight, bounds.getHeight() - presetManagerComponentHeight);
    auto contentWidth  = juce::jmax (contentMinWidth, juce::roundToInt (contentHeight * contentAspectRatio));

    bounds.setSize (contentWidth, contentHeight + presetManagerComponentHeight);
}

void OJDAudioProcessorEditor::addSliderAndSetStyle (AttachedSlider& slider)
{
    slider.setSliderStyle  (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    slider.setColour       (juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    slider.setPopupDisplayEnabled (true, true, this);

    addAndMakeVisible (slider);
    registerHighlightableWidget (slider);
}

void OJDAudioProcessorEditor::addBypassElementsAndSetStyle()
{
    bypassSwitch.setClickingTogglesState (true);

    bypassLED.setClickingTogglesState (false);

    addAndMakeVisible (bypassSwitch);
    addAndMakeVisible (bypassLED);

    registerHighlightableWidget (bypassSwitch);
}

void OJDAudioProcessorEditor::addHpLpSwitchAndSetStyle()
{
    hpLpSwitch.setClickingTogglesState (true);

    addAndMakeVisible (hpLpSwitch);

    registerHighlightableWidget (hpLpSwitch);
}

void OJDAudioProcessorEditor::addPresetManager (OJDAudioProcessor& processorToControl)
{
    presetManagerComponent = processorToControl.stateAndPresetManager.createPresetManagerComponent (*this, true);
    addAndMakeVisible (*presetManagerComponent);
}

void OJDAudioProcessorEditor::checkMessageOfTheDay (OJDAudioProcessor& proc)
{
    juce::Timer::callAfterDelay(1000, [&]()
    {
        if (auto messages = proc.getMessageOfTheDay (500))
        {
            auto updateMessage  = std::move (messages->updateMessage);
            auto generalMessage = std::move (messages->generalMessage);

            // Todo: Use something better than these modal dialogues
            if (updateMessage != nullptr)
                juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::InfoIcon, "OJD Update Available", updateMessage->text);

            if (generalMessage != nullptr)
            {
                juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::InfoIcon, "Schrammel Info", generalMessage->text);

                auto& settingsManager = *jb::SettingsManager::getInstance();
                settingsManager.writeSetting ("LastMOTDVersionDisplayed", generalMessage->version);
            }
        }
    });
}

void OJDAudioProcessorEditor::SubcomponentLayouts::recalculate()
{
    auto yOffset = editor.presetManagerComponentHeight;
    auto bounds = editor.getLocalBounds().withTrimmedTop (yOffset);

    hpLpCentre.x   = bounds.proportionOfWidth (0.498f);
    bypassCentre.x = bounds.proportionOfWidth (0.326f);
    ledCentre.x    = bounds.proportionOfWidth (0.5f);
    volumeCentre.x = bounds.proportionOfWidth (0.33f);
    driveCentre.x  = bounds.proportionOfWidth (0.67f);
    toneCentre.x   = bounds.proportionOfWidth (0.499f);

    hpLpCentre.y   = bounds.proportionOfHeight (0.301f) + yOffset;
    bypassCentre.y = bounds.proportionOfHeight (0.833f) + yOffset;
    ledCentre.y    = bounds.proportionOfHeight (0.125f) + yOffset;
    volumeCentre.y = bounds.proportionOfHeight (0.216f) + yOffset;
    driveCentre.y  = bounds.proportionOfHeight (0.216f) + yOffset;
    toneCentre.y   = bounds.proportionOfHeight (0.413f) + yOffset;

    hpLpWidth   = bounds.proportionOfWidth (0.15f);
    bypassWidth = bounds.proportionOfWidth (0.162f);
    ledWidth    = bounds.proportionOfWidth (0.155f);
    volumeWidth = bounds.proportionOfWidth (0.27f);
    driveWidth  = bounds.proportionOfWidth (0.27f);
    toneWidth   = bounds.proportionOfWidth (0.27f);

    hpLpHeight   = bounds.proportionOfHeight (0.1f);
    bypassHeight = bounds.proportionOfHeight (0.162f);
    ledHeight    = bounds.proportionOfHeight (0.155f);
    volumeHeight = bounds.proportionOfHeight (0.2f);
    driveHeight  = bounds.proportionOfHeight (0.2f);
    toneHeight   = bounds.proportionOfHeight (0.2f);
}
