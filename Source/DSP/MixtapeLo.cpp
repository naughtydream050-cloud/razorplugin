#include "MixtapeLo.h"

MixtapeLo::MixtapeLo() {}

void MixtapeLo::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    loPass.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        spec.sampleRate, 6000.0f);
    loPass.prepare(spec);

    hiCut.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        spec.sampleRate, 10000.0f);
    hiCut.prepare(spec);

    inputGain.prepare(spec);
    inputGain.setGainDecibels(0.0f);
}

void MixtapeLo::reset()
{
    loPass.reset();
    hiCut.reset();
    inputGain.reset();
    hissPhase = 0.0f;
}

void MixtapeLo::setAmount(float amount)
{
    currentAmount = amount;

    float lpFreq = 12000.0f - amount * 8000.0f;
    loPass.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        sampleRate, juce::jmax(1000.0f, lpFreq));

    float hiFreq = 14000.0f - amount * 6000.0f;
    hiCut.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        sampleRate, juce::jmax(2000.0f, hiFreq));

    float bits = 16.0f - amount * 10.0f;
    bitCrushFactor = std::pow(2.0f, juce::jmax(4.0f, bits));
}

void MixtapeLo::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (currentAmount < 0.001f)
        return;

    loPass.process(context);
    hiCut.process(context);

    auto& block = context.getOutputBlock();
    const int numChannels = static_cast<int>(block.getNumChannels());
    const int numSamples  = static_cast<int>(block.getNumSamples());

    float hissLevel = currentAmount * 0.012f;
    float crushInv  = 1.0f / bitCrushFactor;

    for (int ch = 0; ch < numChannels; ++ch)
    {
        float* data = block.getChannelPointer(ch);
        for (int s = 0; s < numSamples; ++s)
        {
            float hiss = (rng.nextFloat() * 2.0f - 1.0f) * hissLevel;
            data[s] += hiss;

            data[s] = std::round(data[s] / crushInv) * crushInv;

            float saturation = 1.0f + currentAmount * 0.3f;
            data[s] = std::tanh(data[s] * saturation);
        }
    }
}
