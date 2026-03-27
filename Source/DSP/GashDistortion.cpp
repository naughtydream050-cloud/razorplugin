#include "GashDistortion.h"

GashDistortion::GashDistortion()
{
    waveShaper.functionToUse = [](float x) -> float
    {
        float drive = 8.0f;
        float driven = x * drive;
        return std::tanh(driven) * (1.0f + 0.3f * std::abs(driven));
    };
}

void GashDistortion::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    inputGain.prepare(spec);
    inputGain.setGainDecibels(18.0f);

    outputGain.prepare(spec);
    outputGain.setGainDecibels(-12.0f);

    waveShaper.prepare(spec);

    *dcBlocker.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(
        spec.sampleRate, 20.0f);
    dcBlocker.prepare(spec);

    *lowPass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(
        spec.sampleRate, 8000.0f);
    lowPass.prepare(spec);
}

void GashDistortion::reset()
{
    waveShaper.reset();
    inputGain.reset();
    outputGain.reset();
    dcBlocker.reset();
    lowPass.reset();
}

void GashDistortion::setAmount(float amount)
{
    currentAmount = amount;
    float driveDB = amount * 30.0f;
    inputGain.setGainDecibels(driveDB);
    float outDB = -driveDB * 0.5f - 3.0f;
    outputGain.setGainDecibels(outDB);

    float lpFreq = 12000.0f - amount * 8000.0f;
    *lowPass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(
        sampleRate, juce::jmax(1000.0f, lpFreq));
}

void GashDistortion::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (currentAmount < 0.001f)
        return;

    inputGain.process(context);
    waveShaper.process(context);
    lowPass.process(context);
    dcBlocker.process(context);
    outputGain.process(context);
}
