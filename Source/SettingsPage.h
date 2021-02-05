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

#include "../JuceLibraryCode/JuceHeader.h"

class SettingsPage : public juce::Component
{
public:
    SettingsPage()
    {
        auto versionInfo = "Version number: " + juce::String (ProjectInfo::versionString);
        auto commitInfo = "Git commit: " + juce::String (ProjectInfo::Git::commit);
        if (! ProjectInfo::Git::branch.empty())
            commitInfo += juce::String (", Branch ") + ProjectInfo::Git::branch;
        auto buildDate = "Build date: " + juce::Time::getCompilationDate().toString(true, true, false);

        versionInfoLabel.setText (versionInfo, juce::dontSendNotification);
        commitInfoLabel.setText (commitInfo, juce::dontSendNotification);
        buildDateLabel.setText (buildDate, juce::dontSendNotification);

        versionInfoLabel.setMinimumHorizontalScale (1.0f);
        commitInfoLabel.setMinimumHorizontalScale (1.0f);
        buildDateLabel.setMinimumHorizontalScale (1.0f);

        addAndMakeVisible (versionInfoLabel);
        addAndMakeVisible (commitInfoLabel);
        addAndMakeVisible (buildDateLabel);

    }

    void resized() override
    {
        fontHeight = getHeight() * 0.03f;

        versionInfoLabel.setFont (versionInfoLabel.getFont().withHeight (fontHeight));
        commitInfoLabel.setFont  (commitInfoLabel.getFont().withHeight (fontHeight));
        buildDateLabel.setFont   (buildDateLabel.getFont().withHeight (fontHeight));

        versionInfoLabel.setBoundsRelative (0.1f, 0.3f,  0.8f, 0.05f);
        commitInfoLabel.setBoundsRelative  (0.1f, 0.35f, 0.8f, 0.05f);
        buildDateLabel.setBoundsRelative   (0.1f, 0.4f,  0.8f, 0.05f);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::grey.withAlpha (0.25f));
        g.setFont (fontHeight);

        g.setColour (juce::Colours::white);
        g.drawHorizontalLine (proportionOfHeight (0.2f), 0.0f, static_cast<float> (getWidth()));
        g.drawText ("Version Information", proportionOfWidth (0.1f),
                                           proportionOfHeight (0.21f),
                                           versionInfoLabel.getWidth(),
                                           proportionOfHeight(0.5f),
                                           juce::Justification::topLeft);

        g.drawHorizontalLine (proportionOfHeight (0.6f), 0.0f, static_cast<float> (getWidth()));
        g.drawText ("Settings", proportionOfWidth (0.1f),
                                proportionOfHeight (0.61f),
                                versionInfoLabel.getWidth(),
                                proportionOfHeight(0.5f),
                                juce::Justification::topLeft);
    }

private:
    float fontHeight = 1.0f;

    juce::Label versionInfoLabel;
    juce::Label commitInfoLabel;
    juce::Label buildDateLabel;
};