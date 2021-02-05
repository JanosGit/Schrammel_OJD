cmake_minimum_required (VERSION 3.16)

#[[

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

]]

juce_add_binary_data(EmbeddedBinaryData
    SOURCES
        Graphics/background.svg
        Graphics/pedalHousingShadow.png
        Graphics/pedalHousing.svg
        Graphics/knob.svg
        Graphics/slideSwitchShadow.png
        Graphics/slideKnob.svg
        Graphics/hpLpBackground.svg
        Graphics/bypassBackground.svg
        Graphics/shadowOverlay.png
        Graphics/ledOn.svg
        Graphics/ledOff.svg
        Graphics/grommet-icons/settings-option.svg

        Fonts/varela.regular.ttf)
