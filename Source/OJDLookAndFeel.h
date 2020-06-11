/*
  ==============================================================================

    OJDLookAndFeel.h
    Created: 21 Feb 2020 7:42:02pm
    Author:  Janos Buttgereit

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OJDLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OJDLookAndFeel (const Drawables& drawables) : knob (drawables.knob->createCopy())
    {
        setColour (juce::TextButton::ColourIds::buttonColourId,    juce::Colours::black);
        setColour (juce::PopupMenu::ColourIds::backgroundColourId, juce::Colours::black);
        setColour (juce::ComboBox::ColourIds::backgroundColourId,  juce::Colours::black);
        setColour (juce::ComboBox::ColourIds::arrowColourId,       juce::Colours::white);
        setColour (juce::ComboBox::ColourIds::outlineColourId,     juce::Colours::white);
    }

    void drawRotarySlider (juce::Graphics& g,
                           int x, int y,
                           int width, int height,
                           float sliderPosProportional,
                           float, float, juce::Slider&) override
    {

        auto bounds = getSquareCenteredInRectangle (x, y, width, height);
        auto centre = bounds.getCentre();
        auto shiftX = bounds.getX() - x;
        auto shiftY = bounds.getY() - y;

        auto rotationAngle = sliderPosProportional * 1.5f * juce::MathConstants<float>::pi;

        auto transform = juce::AffineTransform::scale (bounds.getWidth() / knob->getWidth()).translated (shiftX, shiftY).rotated (rotationAngle, centre.x, centre.y);

        knob->setTransform (transform);

        knob->drawAt (g, static_cast<float> (x), static_cast<float> (y), 1.0f);
    }

    template <typename T>
    static juce::Rectangle<float> getSquareCenteredInRectangle (const juce::Rectangle<T>& rectangle)
    {
        auto edgeLength = std::min (rectangle.getWidth(), rectangle.getHeight());
        auto square = juce::Rectangle<T> (edgeLength, edgeLength).withCentre (rectangle.getCentre());
        return square.toFloat();
    }

    template <typename T>
    static juce::Rectangle<float> getSquareCenteredInRectangle (T x, T y, T width, T height)
    {
        return getSquareCenteredInRectangle (juce::Rectangle<T> (x, y, width, height));
    }

    void drawComboBox (juce::Graphics& g, int width, int height, bool, int, int, int, int, juce::ComboBox& box) override
    {
        juce::Rectangle<int> bounds (0, 0, width, height);

        g.fillAll   (juce::Colours::black);
        g.setColour (juce::Colours::white);
        g.drawRect  (bounds);

        juce::Rectangle<int> arrowZone (width - 20, 0, 15, height);
        juce::Path path;
        path.startNewSubPath (arrowZone.getX() + 3.0f, arrowZone.getCentreY() - 2.0f);
        path.lineTo (static_cast<float> (arrowZone.getCentreX()), arrowZone.getCentreY() + 3.0f);
        path.lineTo (arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 2.0f);

        g.setColour (box.findColour (juce::ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
        g.strokePath (path, juce::PathStrokeType (1.5f));
    }

    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool, bool) override
    {
        g.fillAll   (backgroundColour);
        g.setColour (juce::Colours::white);
        g.drawRect  (button.getLocalBounds());
    }

private:
    std::unique_ptr<juce::Drawable> knob;
};