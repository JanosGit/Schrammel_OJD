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
        auto versionInfo = "Version: " + juce::String (ProjectInfo::versionString);
        auto commitInfo = "Git commit: " + juce::String (ProjectInfo::Git::commit);
        auto branch = juce::String (ProjectInfo::Git::branch);
        if (branch.isNotEmpty())
            commitInfo += ", branch " + branch;

        versionInfoLabel.setText (versionInfo, juce::dontSendNotification);
        commitInfoLabel.setText (commitInfo, juce::dontSendNotification);

        addAndMakeVisible (versionInfoLabel);
        addAndMakeVisible (commitInfoLabel);

    }

    void resized() override
    {
        versionInfoLabel.setBoundsRelative (0.1f, 0.3f, 0.8f, 0.05f);
        commitInfoLabel.setBoundsRelative (0.1f, 0.4f, 0.8f, 0.05f);
    }

private:
    juce::Label versionInfoLabel;
    juce::Label commitInfoLabel;
    juce::Label buildDateLabel;
};