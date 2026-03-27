#pragma once
#include <JuceHeader.h>
#include "DSP/GashDistortion.h"
#include "DSP/BloodCompressor.h"
#include "DSP/SharpenExciter.h"
#include "DSP/RecoilReverb.h"
#include "DSP/MixtapeLo.h"
#include "Presets/PresetManager.h"

class RazorAudioProcessor : public juce::AudioProcessor,
                             public juce::AudioProcessorValueTreeState::Listener
{
public:
    RazorAudioProcessor();
    ~RazorAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Razor"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void parameterChanged(const juce::String& parameterID, float newValue) override;

    juce::AudioProcessorValueTreeState apvts;
    PresetManager presetManager;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    GashDistortion gashDSP;
    BloodCompressor bloodDSP;
    SharpenExciter sharpenDSP;
    RecoilReverb recoilDSP;
    MixtapeLo mixtapeDSP;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RazorAudioProcessor)
};
