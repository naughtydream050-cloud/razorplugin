#pragma once
#include <JuceHeader.h>

class BloodCompressor
{
public:
    BloodCompressor();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void setAmount(float amount);
    void process(juce::dsp::ProcessContextReplacing<float>& context);

private:
    float currentAmount = 0.40f;
    double sampleRate = 44100.0;

    juce::dsp::Compressor<float> compressor;
    juce::dsp::Gain<float> makeupGain;
};
