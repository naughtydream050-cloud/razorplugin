#include "BloodCompressor.h"

BloodCompressor::BloodCompressor() {}

void BloodCompressor::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    compressor.prepare(spec);
    compressor.setThreshold(-24.0f);
    compressor.setRatio(8.0f);
    compressor.setAttack(0.1f);
    compressor.setRelease(50.0f);

    makeupGain.prepare(spec);
    makeupGain.setGainDecibels(6.0f);
}

void BloodCompressor::reset()
{
    compressor.reset();
    makeupGain.reset();
}

void BloodCompressor::setAmount(float amount)
{
    currentAmount = amount;

    float threshold = -12.0f - amount * 24.0f;
    float ratio     = 2.0f + amount * 18.0f;
    float attack    = 0.05f + (1.0f - amount) * 2.0f;
    float release   = 20.0f + (1.0f - amount) * 200.0f;
    float makeup    = amount * 12.0f;

    compressor.setThreshold(threshold);
    compressor.setRatio(ratio);
    compressor.setAttack(attack);
    compressor.setRelease(release);
    makeupGain.setGainDecibels(makeup);
}

void BloodCompressor::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (currentAmount < 0.001f)
        return;

    compressor.process(context);
    makeupGain.process(context);
}
