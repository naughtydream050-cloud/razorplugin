#pragma once
#include <JuceHeader.h>

class MixtapeLo
{
public:
    MixtapeLo();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void setAmount(float amount);
    void process(juce::dsp::ProcessContextReplacing<float>& context);

private:
    float currentAmount = 0.85f;
    double sampleRate = 44100.0;

    juce::dsp::IIR::Filter<float> loPass;
    juce::dsp::IIR::Filter<float> hiCut;
    juce::dsp::Gain<float>        inputGain;

    float hissPhase    = 0.0f;
    float bitCrushFactor = 1.0f;

    juce::Random rng;
};
