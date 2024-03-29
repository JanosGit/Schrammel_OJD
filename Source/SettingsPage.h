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

#include <jb_plugin_base/jb_plugin_base.h>
#include <Resvg4JUCE/Resvg4JUCE.h>
#include <BinaryData.h>

class SettingsPage : public juce::Component
{
public:
    SettingsPage()
      : housingBackside (BinaryData::backside_svg, BinaryData::backside_svgSize)
    {
        addAndMakeVisible (housingBackside);
        auto versionInfo = "Version: " + juce::String (JucePlugin_VersionString);
        auto commitInfo = "Git Commit: " + juce::String (ProjectInfo::Git::commit) + getBranchName();
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
        auto bounds = getLocalBounds();

        auto hb = jb::scaledKeepingCentre (bounds, 0.94f);
        housingBackside.setBounds (hb);

        fontHeight = static_cast<float> (getHeight ()) * 0.02f;

        versionInfoLabel.setFont (versionInfoLabel.getFont().withHeight (fontHeight));
        commitInfoLabel.setFont  (commitInfoLabel.getFont().withHeight (fontHeight));
        buildDateLabel.setFont   (buildDateLabel.getFont().withHeight (fontHeight));

        versionInfoLabel.setBoundsRelative (0.2f, 0.73f, 0.8f, 0.05f);
        commitInfoLabel.setBoundsRelative  (0.2f, 0.78f, 0.8f, 0.05f);
        buildDateLabel.setBoundsRelative   (0.2f, 0.83f, 0.8f, 0.05f);
    }

private:
    float fontHeight = 1.0f;

    juce::Label versionInfoLabel;
    juce::Label commitInfoLabel;
    juce::Label buildDateLabel;

    jb::SVGComponent housingBackside;

    static juce::String getBranchName()
    {
        if (ProjectInfo::Git::branch.empty())
            return "";

        const juce::String prefix = "\n    Branch: ";

        if (ProjectInfo::Git::branch.size() <= 25)
            return prefix + ProjectInfo::Git::branch;

        return prefix + juce::String (ProjectInfo::Git::branch).substring (0, 20) + "...";
    }
};