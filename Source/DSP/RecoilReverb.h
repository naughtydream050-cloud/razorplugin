#pragma once
#include <JuceHeader.h>

class RecoilReverb
{
public:
    RecoilReverb();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void setAmount(float amount);
    void process(juce::dsp::ProcessContextReplacing<float>& context);

private:
    float currentAmount = 0.30f;
    double sampleRate = 44100.0;

    juce::Reverb reverb;
    juce::Reverb::Parameters reverbParams;

    juce::dsp::IIR::Filter<float> metalFilter;
    juce::dsp::Gain<float>        wetGain;
    juce::dsp::Gain<float>        dryGain;

    juce::AudioBuffer<float> wetBuffer;
};
