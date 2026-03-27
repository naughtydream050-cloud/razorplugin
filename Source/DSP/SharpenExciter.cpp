#include "SharpenExciter.h"

SharpenExciter::SharpenExciter()
{
    harmonicGenerator.functionToUse = [](float x) -> float
    {
        return x + 0.5f * x * x * x - 0.1f * x * x * x * x * x;
    };
}

void SharpenExciter::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    highPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(
        spec.sampleRate, 3000.0f);
    highPassFilter.prepare(spec);

    harmonicGenerator.prepare(spec);

    blendFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        spec.sampleRate, 8000.0f, 0.7f, 4.0f);
    blendFilter.prepare(spec);

    exciterGain.prepare(spec);
    exciterGain.setGainDecibels(0.0f);
}

void SharpenExciter::reset()
{
    highPassFilter.reset();
    harmonicGenerator.reset();
    blendFilter.reset();
    exciterGain.reset();
}

void SharpenExciter::setAmount(float amount)
{
    currentAmount = amount;

    float hpFreq = 8000.0f - amount * 5000.0f;
    highPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(
        sampleRate, juce::jmax(500.0f, hpFreq));

    float shelfGainDB = amount * 12.0f;
    blendFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        sampleRate, 6000.0f, 0.7f, juce::Decibels::decibelsToGain(shelfGainDB));

    exciterGain.setGainDecibels(amount * 6.0f - 3.0f);
}

void SharpenExciter::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (currentAmount < 0.001f)
        return;

    highPassFilter.process(context);
    harmonicGenerator.process(context);
    blendFilter.process(context);
    exciterGain.process(context);
}
