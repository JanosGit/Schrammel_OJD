
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OJDProcessor.h"
#include "OJDLookAndFeel.h"


class OJDAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                 public juce::ComponentBoundsConstrainer,
                                 private juce::Value::Listener
{
public:
    OJDAudioProcessorEditor (OJDAudioProcessor&);
    ~OJDAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    /** To be called by the processor to add a subtree to the plugin state that contains GUI-related information */
    static const juce::ValueTree createUIStateSubtree();

private:
    void checkBounds (juce::Rectangle<int>& bounds, const juce::Rectangle<int>&, const juce::Rectangle<int>&, bool, bool, bool, bool) override;

    void valueChanged (juce::Value&) override;

    /** Adds a slider to the component, makes it visible and performs some general styling */
    void addSliderAndSetStyle (juce::Slider& slider);

    /** Adds the bypass switch to the component and sets its style */
    void addBypassElementsAndSetStyle();

    /** Adds the HP/LP switch to the component and sets its style */
    void addHpLpSwitchAndSetStyle();

    void addPresetManager (OJDAudioProcessor& processorToControl);

    struct SubcomponentLayouts
    {
        SubcomponentLayouts (const OJDAudioProcessorEditor& e) : editor (e) {}

        void recalculate();

        juce::AffineTransform getBackgroundTransform();

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

    Drawables& drawables;
    SubcomponentLayouts layouts;

    OJDLookAndFeel ojdLookAndFeel;

    jb::AttachedWidget<juce::Slider> driveSlider, toneSlider, volumeSlider;
    jb::AttachedWidget<juce::DrawableButton> bypassSwitch, bypassLED, hpLpSwitch;

    std::unique_ptr<jb::PresetManagerComponent> presetManagerComponent;

    static const juce::Identifier uiStateTreeType, uiStateTreeWidth, uiStateTreeHeight;

    juce::Value lastUIWidth, lastUIHeight;

    // Some constants derived from the GUI design
    static constexpr int   presetManagerComponentHeight = 20;
    static constexpr int   contentMinHeight             = 568;
    static constexpr int   contentMinWitdh              = 310;
    static constexpr float contentAspectRatio           = 0.545774647887324f;
    static constexpr float widthToBackgroundScaleFactor = 6.451612903225806e-4f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OJDAudioProcessorEditor)
};
