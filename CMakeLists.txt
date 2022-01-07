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
if (WIN32)
    set (CMAKE_GENERATOR_TOOLSET ClangCL)
    set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# If we are compiling for Mac OS we want to target OS versions down to 10.9
if (APPLE)
    set (CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE INTERNAL "")
    set (CMAKE_OSX_ARCHITECTURES x86_64 arm64)
endif()

project (SCHRAMMEL_OJD VERSION 0.9.8)

# Adding JUCE
add_subdirectory (Ext/JUCE)

# The subdirectory contains its own CMakeLists that creates BinaryData targets containing graphics and fonts
add_subdirectory (BinaryResources)

# Adding JSON
set (JSON_BuildTests OFF CACHE INTERNAL "")
set (JSON_Install OFF CACHE INTERNAL "")
add_subdirectory (Ext/JBPluginBase)
add_subdirectory (Ext/Resvg4JUCE)

if (NOT DEFINED COPY_PLUGIN_AFTER_BUILD)
    set (COPY_PLUGIN_AFTER_BUILD TRUE)
endif()

function (add_ojd_version format)

# Add the plugin target itself
juce_add_plugin (OJD-${format}

        COMPANY_NAME Schrammel
        PRODUCT_NAME "OJD"

        COPY_PLUGIN_AFTER_BUILD ${COPY_PLUGIN_AFTER_BUILD}
        FORMATS ${format}

        ${ARGN})

# We want to compile the plugin with C++14
target_compile_features (OJD-${format} PRIVATE cxx_std_14)

# Gather some info regarding our git commit to inject them into a header file.
jb_add_git_version_info (OJD-${format})

# Adding all source files to the plugin target
target_sources (OJD-${format} PRIVATE
        Source/OJDPedalComponent.cpp
        Source/OJDAudioProcessorEditor.cpp
        Source/OJDProcessor.cpp
        Source/OJDParameters.cpp)

target_compile_definitions (OJD-${format}
        PUBLIC
        DONT_SET_USING_JUCE_NAMESPACE=1
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_STRICT_REFCOUNTEDPTR=1
        JUCE_VST3_CAN_REPLACE_VST2=0
        JB_INCLUDE_JSON=1)

target_link_libraries (OJD-${format} PRIVATE
        # JUCE Modules
        juce::juce_audio_utils
        juce::juce_dsp

        # Custom modules
        jb_plugin_base
        jb::Resvg4JUCE

        # JSON dependncy in plugin base
        nlohmann_json::nlohmann_json

        # Binary Data
        EmbeddedBinaryData

        # Recommended flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        juce::juce_recommended_config_flags)

endfunction()

add_ojd_version (VST3
        # A four-character manufacturer id with at least one upper-case character
        PLUGIN_MANUFACTURER_CODE Srml
        # A unique four-character plugin id with at least one upper-case character
        PLUGIN_CODE SOJD

        # When the OJD was released, there was a bug in JUCE that wrongly reported the plugin code "proj". Applying this
        # to not break existing sessions
        USE_LEGACY_COMPATIBILITY_PLUGIN_CODE TRUE

        VST3_CATEGORIES Fx Distortion)

        target_compile_definitions (OJD-VST3 PRIVATE JUCE_DISPLAY_SPLASH_SCREEN=1)

if (APPLE)
add_ojd_version (AU
        # A four-character manufacturer id with at least one upper-case character
        PLUGIN_MANUFACTURER_CODE Srml
        # A unique four-character plugin id with at least one upper-case character
        PLUGIN_CODE Sojd

        AU_MAIN_TYPE kAudioUnitType_Effect)
        target_compile_definitions (OJD-AU PRIVATE JUCE_DISPLAY_SPLASH_SCREEN=1)
endif()

if (AAX_SDK_PATH)

    if (WIN32)
        # Extra escaping is needed to not mess up the macro inside the AAX wrapper with the path created by the
        # GitHub Actions runner
        string (REPLACE "\\" "\\\\" AAX_SDK_PATH ${AAX_SDK_PATH})
    endif()

    juce_set_aax_sdk_path ("${AAX_SDK_PATH}")
    add_ojd_version (AAX
            # A four-character manufacturer id with at least one upper-case character
            PLUGIN_MANUFACTURER_CODE Srml
            # A unique four-character plugin id with at least one upper-case character
            PLUGIN_CODE Sojd

            AAX_IDENTIFIER io.schrammel.ojd
            AAX_CATEGORY AAX_ePlugInCategory_Harmonic)

    set_target_properties(OJD-AAX_AAX PROPERTIES OSX_ARCHITECTURES x86_64)
    
    target_compile_definitions (OJD-AAX
        PRIVATE
            JUCE_DISPLAY_SPLASH_SCREEN=1
            _CRT_SECURE_NO_WARNINGS=1)
endif()

add_library (OJD-ALL_FORMATS INTERFACE)
target_link_libraries (OJD-ALL_FORMATS
    INTERFACE
        OJD-AU_AU
        OJD-VST3_VST3
        OJD-AAX_AAX)
