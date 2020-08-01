
cmake_minimum_required (VERSION 3.16)

project (SCHRAMMEL_OJD VERSION 0.9.5)

# If we are using MSVC we want static runtime linkage
if (MSVC)
    set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Adding JUCE
add_subdirectory (Ext/JUCE)

# Adding own modules
juce_add_module (Ext/JBPluginBase/jb_plugin_base)

set (FormatsToBuild AU VST3)

# If a path to the AAX SDK is passed to CMake, an AAX version will be built too
if (AAX_SDK_PATH)
    juce_set_aax_sdk_path (${AAX_SDK_PATH})
    list (APPEND FormatsToBuild AAX)
endif()

juce_add_plugin (OJD
        COMPANY_NAME Schrammel                      # Specify the name of the plugin's author
        IS_SYNTH FALSE                              # Is this a synth or an effect?
        NEEDS_MIDI_INPUT FALSE                      # Does the plugin need midi input?
        NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
        IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
        COPY_PLUGIN_AFTER_BUILD TRUE                # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE Juce               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE Dem0                            # A unique four-character plugin id with at least one upper-case character
        FORMATS ${FormatsToBuild}                   # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        PRODUCT_NAME "OJD")                         # The name of the final executable, which can differ from the target name

target_compile_features (OJD PRIVATE cxx_std_14)

juce_generate_juce_header (OJD)

target_sources (OJD PRIVATE
        Source/OJDAudioProcessorEditor.cpp
        Source/OJDProcessor.cpp
        Source/OJDParameters.cpp)

add_subdirectory (BinaryResources/SVGs)

target_compile_definitions (OJD
        PUBLIC
        DONT_SET_USING_JUCE_NAMESPACE=1
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_STRICT_REFCOUNTEDPTR=1
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries (OJD PRIVATE
        # JUCE Modules
        juce::juce_audio_utils
        juce::juce_dsp

        # Custom modules
        jb_plugin_base

        # Binary Data
        EmbeddedSVGs

        # Recommended flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        juce::juce_recommended_config_flags)

if (CREATE_INSTALLER)
    add_subdirectory (Deployment)
endif()
