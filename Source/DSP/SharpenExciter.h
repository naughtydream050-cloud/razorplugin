#pragma once
#include <JuceHeader.h>

class SharpenExciter
{
public:
    SharpenExciter();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void setAmount(float amount);
    void process(juce::dsp::ProcessContextReplacing<float>& context);

private:
    float currentAmount = 0.60f;
    double sampleRate = 44100.0;

    juce::dsp::IIR::Filter<float> highPassFilter;
    juce::dsp::WaveShaper<float>  harmonicGenerator;
    juce::dsp::IIR::Filter<float> blendFilter;
    juce::dsp::Gain<float>        exciterGain;
};
