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

#include <juce_gui_basics/juce_gui_basics.h>

class OJDLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OJDLookAndFeel (const juce::Drawable& knobDrawable) : knob (knobDrawable.createCopy())
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
        auto shiftX = bounds.getX() - static_cast<float> (x);
        auto shiftY = bounds.getY() - static_cast<float> (y);

        // The true centre of the knob svgs is slighly off centre, this will correct that issue
        centre.x += -0.01739130435f * bounds.getWidth();
        centre.y += -0.00434782608f * bounds.getHeight();

        auto rotationAngle = (sliderPosProportional -0.5f) * 1.5f * juce::MathConstants<float>::pi;

        auto transform = juce::AffineTransform::scale (bounds.getWidth() / knob->getWidth())
                                               .translated (shiftX, shiftY)
                                               .rotated (rotationAngle, centre.x, centre.y);
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