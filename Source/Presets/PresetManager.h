#pragma once
#include <JuceHeader.h>

struct PresetData
{
    juce::String name;
    float gash;
    float blood;
    float sharpen;
    float recoil;
    float mixtape;
};

class PresetManager
{
public:
    explicit PresetManager(juce::AudioProcessorValueTreeState& apvts);

    int getNumPresets() const;
    int getCurrentPresetIndex() const;
    juce::String getPresetName(int index) const;
    void loadPreset(int index);

    const juce::Array<PresetData>& getPresets() const { return presets; }

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::Array<PresetData> presets;
    int currentPresetIndex = 0;

    void initPresets();
};
