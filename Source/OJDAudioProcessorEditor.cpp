

#include "OJDProcessor.h"
#include "OJDAudioProcessorEditor.h"

//==============================================================================
OJDAudioProcessorEditor::OJDAudioProcessorEditor (OJDAudioProcessor& proc)
 :  juce::AudioProcessorEditor (&proc),
    drawables        (proc.drawables),
    layouts          (*this),
    ojdLookAndFeel   (drawables),
    driveSlider      (proc.parameters, OJDParameters::Drive::id),
    toneSlider       (proc.parameters, OJDParameters::Tone::id),
    volumeSlider     (proc.parameters, OJDParameters::Volume::id),
    bypassSwitch     (proc.parameters, OJDParameters::Bypass::id, "Bypass", juce::DrawableButton::ImageFitted),
    bypassLED        (proc.parameters, OJDParameters::Bypass::id, "LED",    juce::DrawableButton::ImageFitted),
    hpLpSwitch       (proc.parameters, OJDParameters::HpLp::id,   "HpLp",   juce::DrawableButton::ImageFitted)
{
    setLookAndFeel (&ojdLookAndFeel);

    addSliderAndSetStyle (volumeSlider);
    addSliderAndSetStyle (driveSlider);
    addSliderAndSetStyle (toneSlider);

    addBypassElementsAndSetStyle();

    addHpLpSwitchAndSetStyle();

    addPresetManager (proc);

    setResizable (true, true);
    setConstrainer (this);

    // Make the UI size restorable
    auto uiStateTree = proc.parameters.state.getChildWithName (uiStateTreeType);

    lastUIWidth .referTo (uiStateTree.getPropertyAsValue (uiStateTreeWidth,  nullptr));
    lastUIHeight.referTo (uiStateTree.getPropertyAsValue (uiStateTreeHeight, nullptr));
    lastUIWidth. addListener (this);
    lastUIHeight.addListener (this);

    setSize (lastUIWidth.getValue(), lastUIHeight.getValue());
}

OJDAudioProcessorEditor::~OJDAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void OJDAudioProcessorEditor::paint (juce::Graphics& g)
{
    drawables.editorBackground->drawAt (g, 0, presetManagerComponentHeight, 1.0f);
}

void OJDAudioProcessorEditor::resized()
{
    presetManagerComponent->setBounds (getLocalBounds().removeFromTop (presetManagerComponentHeight));

    layouts.recalculate();
    drawables.editorBackground->setTransform (layouts.getBackgroundTransform());

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

    // Store size
    lastUIHeight = getHeight();
    lastUIWidth  = getWidth();
}

const juce::ValueTree OJDAudioProcessorEditor::createUIStateSubtree()
{
    return juce::ValueTree (uiStateTreeType,
                           {
                               { uiStateTreeWidth,  contentMinWitdh },
                               { uiStateTreeHeight, contentMinHeight + presetManagerComponentHeight}
                           });
}

void OJDAudioProcessorEditor::checkBounds (juce::Rectangle<int>& bounds, const juce::Rectangle<int>&, const juce::Rectangle<int>&, bool, bool, bool, bool)
{
    auto contentHeight = juce::jmax (contentMinHeight, bounds.getHeight() - presetManagerComponentHeight);
    auto contentWidth  = juce::jmax (contentMinWitdh, juce::roundToInt (contentHeight * contentAspectRatio));

    bounds.setSize (contentWidth, contentHeight + presetManagerComponentHeight);
}

void OJDAudioProcessorEditor::valueChanged (juce::Value&) { setSize (lastUIWidth.getValue(), lastUIHeight.getValue()); }

void OJDAudioProcessorEditor::addSliderAndSetStyle (juce::Slider& slider)
{
    slider.setSliderStyle  (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    slider.setColour       (juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    slider.setPopupDisplayEnabled (true, true, this);

    addAndMakeVisible (slider);
}

void OJDAudioProcessorEditor::addBypassElementsAndSetStyle()
{
    bypassSwitch.setImages (drawables.footSwitch.get());
    bypassSwitch.setColour (juce::DrawableButton::backgroundColourId,   juce::Colours::transparentBlack);
    bypassSwitch.setColour (juce::DrawableButton::backgroundOnColourId, juce::Colours::transparentBlack);
    bypassSwitch.setClickingTogglesState (true);

    bypassLED.setImages (drawables.ledOn.get(), nullptr, nullptr, nullptr, drawables.ledOff.get());
    bypassLED.setColour (juce::DrawableButton::backgroundColourId,   juce::Colours::transparentBlack);
    bypassLED.setColour (juce::DrawableButton::backgroundOnColourId, juce::Colours::transparentBlack);
    bypassLED.setClickingTogglesState (false);

    addAndMakeVisible (bypassSwitch);
    addAndMakeVisible (bypassLED);
}

void OJDAudioProcessorEditor::addHpLpSwitchAndSetStyle()
{
    hpLpSwitch.setImages (drawables.slideSwitchLP.get(), nullptr, nullptr, nullptr, drawables.slideSwitchHP.get());
    hpLpSwitch.setColour (juce::DrawableButton::backgroundColourId,   juce::Colours::transparentBlack);
    hpLpSwitch.setColour (juce::DrawableButton::backgroundOnColourId, juce::Colours::transparentBlack);
    hpLpSwitch.setClickingTogglesState (true);

    addAndMakeVisible (hpLpSwitch);
}

void OJDAudioProcessorEditor::addPresetManager (OJDAudioProcessor& processorToControl)
{
    presetManagerComponent = processorToControl.stateAndPresetManager.createPresetManagerComponent (*this, true);
    addAndMakeVisible (*presetManagerComponent);
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

juce::AffineTransform OJDAudioProcessorEditor::SubcomponentLayouts::getBackgroundTransform()
{
    return juce::AffineTransform::scale (editor.getWidth() * widthToBackgroundScaleFactor);
}

const juce::Identifier OJDAudioProcessorEditor::uiStateTreeType   ("uiState");
const juce::Identifier OJDAudioProcessorEditor::uiStateTreeWidth  ("width");
const juce::Identifier OJDAudioProcessorEditor::uiStateTreeHeight ("height");