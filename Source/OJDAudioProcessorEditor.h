
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OJDProcessor.h"
#include "OJDLookAndFeel.h"
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

    void checkMessageOfTheDay (OJDAudioProcessor& processor);
    void setMessage (const juce::String& text, const juce::URL url);

    jb::SVGComponent background;

    OJDPedalComponent pedal;

    bool isInMessageState;
    juce::TextEditor messageEditor;
    juce::TextButton messageOkButton;
    juce::TextButton messageLearnMoreButton;

    const std::unique_ptr<juce::Drawable> knobDrawable;

    OJDLookAndFeel ojdLookAndFeel;

    std::unique_ptr<jb::PresetManagerComponent> presetManagerComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OJDAudioProcessorEditor)
};
