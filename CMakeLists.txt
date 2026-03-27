cmake_minimum_required(VERSION 3.22)
project(Razor VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch JUCE
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.12
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(Razor
    COMPANY_NAME "RazorAudio"
    PLUGIN_MANUFACTURER_CODE RzAu
    PLUGIN_CODE Rzr1
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "Razor"
    PLUGIN_NAME "Welcome to Razor"
    DESCRIPTION "Krump Vocal Destruction Plugin"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES "Fx" "Distortion" "Dynamics"
    AU_MAIN_TYPE kAudioUnitType_Effect
)

target_sources(Razor
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/DSP/GashDistortion.cpp
        Source/DSP/BloodCompressor.cpp
        Source/DSP/SharpenExciter.cpp
        Source/DSP/RecoilReverb.cpp
        Source/DSP/MixtapeLo.cpp
        Source/UI/RazorLookAndFeel.cpp
        Source/UI/RazorKnob.cpp
        Source/Presets/PresetManager.cpp
)

target_compile_definitions(Razor
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(Razor
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
