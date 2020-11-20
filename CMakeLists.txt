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

# If we are using MSVC we want dynamic runtime linkage
if (MSVC)
    set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# If we are compiling for Mac OS we want to target OS versions down to 10.9
if (APPLE)
    set (CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE INTERNAL "")
endif()

project (SCHRAMMEL_OJD VERSION 0.9.7)

# Adding JUCE
add_subdirectory (Ext/JUCE)

# Adding JSON
set (JSON_BuildTests OFF CACHE INTERNAL "")
set (JSON_Install OFF CACHE INTERNAL "")
add_subdirectory (Ext/JBPluginBase/Ext/json)

# Adding own modules
juce_add_module (Ext/JBPluginBase/jb_plugin_base)
juce_add_module (Ext/Resvg4JUCE/jb_resvg)

# Set include paths for resvg lib. Todo: Make target based
if (WIN32)
    LINK_DIRECTORIES (Ext/Resvg4JUCE/jb_resvg/libs/VisualStudio2019/x64/MD)
else()
    LINK_DIRECTORIES (Ext/Resvg4JUCE/jb_resvg/libs/MacOSX/x86_64)
endif()

# VST3 should be built on all platforms
set (FormatsToBuild VST3)

# On Mac, a AU version will be built too
if (APPLE)
    list (APPEND FormatsToBuild AU)
endif()

# If a path to the AAX SDK is passed to CMake, an AAX version will be built too
if (AAX_SDK_PATH)
    juce_set_aax_sdk_path (${AAX_SDK_PATH})
    list (APPEND FormatsToBuild AAX)
endif()

# Add the plugin target itself
juce_add_plugin (OJD
        COMPANY_NAME Schrammel
        PRODUCT_NAME "OJD"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE                # Should the plugin be installed to a default location after building?
        FORMATS ${FormatsToBuild}                   # The formats to build. Depending on the platform and if an AAX SDK is passed in
        PLUGIN_MANUFACTURER_CODE Srml               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE SOJD                            # A unique four-character plugin id with at least one upper-case character
        AAX_IDENTIFIER de.schrammel.ojd
        VST3_CATEGORIES Fx Distortion
        AU_MAIN_TYPE kAudioUnitType_Effect
        AAX_CATEGORY AAX_ePlugInCategory_Harmonic)

# We want to compile the plugin with C++14
target_compile_features (OJD PRIVATE cxx_std_14)

# Using JUCE header as the original project was developed with Projucer - might be removed in future
juce_generate_juce_header (OJD)

# Adding all source files to the plugin target
target_sources (OJD PRIVATE
        Source/OJDAudioProcessorEditor.cpp
        Source/OJDProcessor.cpp
        Source/OJDParameters.cpp)

# The subdirectory contains its own CMakeLists that creates a BinaryData target containing all SVGs
add_subdirectory (BinaryResources/SVGs)

target_compile_definitions (OJD
        PUBLIC
        DONT_SET_USING_JUCE_NAMESPACE=1
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_STRICT_REFCOUNTEDPTR=1
        JUCE_VST3_CAN_REPLACE_VST2=0
        JB_INCLUDE_JSON=1)

target_link_libraries (OJD PRIVATE
        # JUCE Modules
        juce::juce_audio_utils
        juce::juce_dsp

        # Custom modules
        jb_plugin_base
        jb_resvg

        # JSON dependncy in plugin base
        nlohmann_json::nlohmann_json

        # Binary Data
        EmbeddedSVGs

        # Recommended flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        juce::juce_recommended_config_flags)

# The Deployment subdirectory contains its own CMakeLists that creates a CPack based installer
if (CREATE_INSTALLER)
    add_subdirectory (Deployment)
endif()
