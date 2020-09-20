
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OJDProcessor.h"
#include "OJDLookAndFeel.h"

/** Some constants derived from the GUI design */
struct OJDEditorConstants
{
    static constexpr int   presetManagerComponentHeight = 20;
    static constexpr int   contentMinHeight             = 568;
    static constexpr int   overallMinHeight             = presetManagerComponentHeight + contentMinHeight;
    static constexpr int   contentMinWidth              = 310;
    static constexpr float contentAspectRatio           = 0.545774647887324f;
    static constexpr float widthToBackgroundScaleFactor = 6.451612903225806e-4f;
};

class OJDAudioProcessorEditor  : public jb::PluginEditorBase<OJDEditorConstants::contentMinWidth, OJDEditorConstants::overallMinHeight>,
                                 private OJDEditorConstants
{
public:
    OJDAudioProcessorEditor (OJDAudioProcessor&);
    ~OJDAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override {}
    void constrainedResized() override;
    void checkBounds (juce::Rectangle<int>& bounds, const juce::Rectangle<int>&, const juce::Rectangle<int>&, bool, bool, bool, bool) override;

private:

    /** Determines how AAX automation highlight boxes are drawn */
    using HighlightLayout = jb::HighlightableWidget::BoxLayout;

    using AttachedSlider       = jb::AttachedWidget<juce::Slider,  HighlightLayout::SquareCenteredAboveParent>;
    using AttachedBypassButton = jb::AttachedWidget<jb::SVGButton, HighlightLayout::SquareCenteredAboveParent>;
    using AttachedHpLpButton   = jb::AttachedWidget<jb::SVGButton, HighlightLayout::FollowParentBounds>;

    /** Adds a slider to the component, makes it visible and performs some general styling */
    void addSliderAndSetStyle (AttachedSlider& slider);

    /** Adds the bypass switch to the component and sets its style */
    void addBypassElementsAndSetStyle();

    /** Adds the HP/LP switch to the component and sets its style */
    void addHpLpSwitchAndSetStyle();

    void addPresetManager (OJDAudioProcessor& processorToControl);

    void checkMessageOfTheDay (OJDAudioProcessor& processor);

    struct SubcomponentLayouts
    {
        SubcomponentLayouts (const OJDAudioProcessorEditor& e) : editor (e) {}

        void recalculate();

        const OJDAudioProcessorEditor& editor;

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

    SubcomponentLayouts layouts;

    jb::SVGComponent background { BinaryData::background_svg, BinaryData::background_svgSize };

    const std::unique_ptr<juce::Drawable> knobDrawable;

    OJDLookAndFeel ojdLookAndFeel;

    AttachedSlider driveSlider, toneSlider, volumeSlider;
    AttachedBypassButton bypassSwitch, bypassLED;
    AttachedHpLpButton hpLpSwitch;

    std::unique_ptr<jb::PresetManagerComponent> presetManagerComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OJDAudioProcessorEditor)
};
