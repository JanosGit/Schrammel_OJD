//
// Created by Janos Buttgereit on 25.09.20.
//

#pragma once

extern juce::Rectangle<int> scaledKeepingCentre (const juce::Rectangle<int>& src, float scale);

/**
 * A slightly hacky class to paint the slide switches from the Adobe XD based GUI design draft.
 * Faced quite a lot of trouble rendering the layers correctly so I went with a mix of vector graphics
 * and rasterised shadow, combined with a clipping mask path and plenty of magic numbers for alignment.
 * To allow the knobs shadow to go over the bottom and right edge of the component, it leaves some margin
 * at those sides
 */
class SlideSwitch : public juce::Button
{
public:
    SlideSwitch (const char* backgroundImageData, int backgroundImageSize,
                 juce::Rectangle<float> backroundPlacementRelative,
                 const juce::String& name = juce::String())
      : juce::Button (name),
        bgBoundsRelative (backroundPlacementRelative)
    {
        knob.loadFromBinaryData (BinaryData::slideKnob_svg, BinaryData::slideKnob_svgSize);
        background.loadFromBinaryData (backgroundImageData, backgroundImageSize);
    }

    void resized() override
    {
        // All rasterized items should be rendered at a higher scale.
        auto scale = juce::Desktop::getInstance().getDisplays().findDisplayForPoint (getBounds().getCentre()).scale;

        constexpr auto insetFactor = 0.9f;
        auto newImageBounds = getLocalBounds().toFloat() * scale * insetFactor;

        if (newImageBounds == cachedComponentBounds)
            return;

        cachedComponentBounds = newImageBounds;

        mask = createMaskPath (newImageBounds);

        // The knob needs slightly greater bounds as it has a shadow that needs to be clipped
        constexpr auto knobScaling = 1.3f;
        knobImage = knob.render (cachedComponentBounds * (scale * knobScaling));

        // Render the background
        bgBounds = newImageBounds.getProportion (bgBoundsRelative);
        bgImage = background.render (bgBounds);

        // Scale things back down to component coordinate space
        mask.applyTransform (juce::AffineTransform::scale (1 / static_cast<float> (scale)));
        knobBounds = knobImage.getBounds().toFloat() / scale;
        bgBounds /= scale;

        // Shift knob bounds a bit to the upper left
        knobBounds = getLocalBounds().toFloat() * knobScaling;
        auto knobHeight = knobBounds.getHeight();
        knobBounds.setWidth (knobHeight);
        knobBounds.translate (knobHeight * -0.2f, knobHeight * -0.22f);
    }

private:
    juce::Path mask;

    // The individual assets
    juce::Image shadow { juce::ImageFileFormat::loadFrom (BinaryData::slideSwitchShadow_png, BinaryData::slideSwitchShadow_pngSize) };
    jb::Resvg::RenderTree knob;
    jb::Resvg::RenderTree background;

    // The rendered images
    juce::Image knobImage;
    juce::Image bgImage;

    juce::Rectangle<float> cachedComponentBounds;
    juce::Rectangle<float> knobBounds;
    juce::Rectangle<float> bgBoundsRelative;
    juce::Rectangle<float> bgBounds;

    void paintButton (juce::Graphics& g, bool, bool) override
    {
        // Use a Graphics::ScopedSaveState to restore the clipping mask later
        {
            juce::Graphics::ScopedSaveState saveState (g);

            g.reduceClipRegion (mask);

            auto shadowBounds = (getLocalBounds().toFloat() * 1.9f).translated (getWidth() * -0.46f, getHeight() * -0.6f);
            g.drawImage (shadow, shadowBounds, juce::RectanglePlacement::centred);

            g.drawImage (bgImage, bgBounds, juce::RectanglePlacement::centred);
        }

        const auto& bounds = getToggleState() ? knobBounds.translated (getWidth() * 0.43f, 0.0f) : knobBounds;
        g.drawImage (knobImage, bounds, juce::RectanglePlacement::centred);
    }

    static juce::Path createMaskPath (const juce::Rectangle<float>& bounds)
    {
        juce::Path p;
        p.addRoundedRectangle (bounds, bounds.getHeight() / 2);

        return p;
    }
};
