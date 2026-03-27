#pragma once
#include <JuceHeader.h>

class GashDistortion
{
public:
    GashDistortion();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void setAmount(float amount);
    void process(juce::dsp::ProcessContextReplacing<float>& context);

private:
    float currentAmount = 0.75f;
    double sampleRate = 44100.0;
    juce::dsp::WaveShaper<float> waveShaper;
    juce::dsp::Gain<float> inputGain;
    juce::dsp::Gain<float> outputGain;
    juce::dsp::IIR::Filter<float> dcBlocker;
    juce::dsp::IIR::Filter<float> lowPass;
};
