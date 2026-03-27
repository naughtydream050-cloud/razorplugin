#include "RecoilReverb.h"

RecoilReverb::RecoilReverb()
{
    reverbParams.roomSize   = 0.1f;
    reverbParams.damping    = 0.9f;
    reverbParams.wetLevel   = 0.3f;
    reverbParams.dryLevel   = 0.7f;
    reverbParams.width      = 0.5f;
    reverbParams.freezeMode = 0.0f;
    reverb.setParameters(reverbParams);
}

void RecoilReverb::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    reverb.setSampleRate(spec.sampleRate);

    *metalFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        spec.sampleRate, 2000.0f, 0.5f, 8.0f);
    metalFilter.prepare(spec);

    wetGain.prepare(spec);
    dryGain.prepare(spec);
    wetGain.setGainLinear(currentAmount);
    dryGain.setGainLinear(1.0f - currentAmount);

    wetBuffer.setSize(static_cast<int>(spec.numChannels),
                      static_cast<int>(spec.maximumBlockSize));
}

void RecoilReverb::reset()
{
    reverb.reset();
    metalFilter.reset();
    wetGain.reset();
    dryGain.reset();
}

void RecoilReverb::setAmount(float amount)
{
    currentAmount = amount;

    reverbParams.roomSize = 0.05f + amount * 0.25f;
    reverbParams.damping  = 1.0f - amount * 0.4f;
    reverbParams.wetLevel = amount;
    reverbParams.dryLevel = 1.0f;
    reverbParams.width    = 0.3f + amount * 0.4f;
    reverb.setParameters(reverbParams);

    float metalGainDB = 6.0f + amount * 10.0f;
    *metalFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        sampleRate, 1500.0f, 0.5f, juce::Decibels::decibelsToGain(metalGainDB));

    wetGain.setGainLinear(amount * 0.7f);
    dryGain.setGainLinear(1.0f);
}

void RecoilReverb::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (currentAmount < 0.001f)
        return;

    auto& inputBlock  = context.getInputBlock();
    auto& outputBlock = context.getOutputBlock();
    const int numChannels = static_cast<int>(outputBlock.getNumChannels());
    const int numSamples  = static_cast<int>(outputBlock.getNumSamples());

    wetBuffer.setSize(numChannels, numSamples, false, false, true);
    for (int ch = 0; ch < numChannels; ++ch)
    {
        wetBuffer.copyFrom(ch, 0, inputBlock.getChannelPointer(ch), numSamples);
    }

    if (numChannels >= 2)
    {
        reverb.processStereo(wetBuffer.getWritePointer(0),
                             wetBuffer.getWritePointer(1),
                             numSamples);
    }
    else
    {
        reverb.processMono(wetBuffer.getWritePointer(0), numSamples);
    }

    for (int ch = 0; ch < numChannels; ++ch)
    {
        float* out = outputBlock.getChannelPointer(ch);
        const float* wet = wetBuffer.getReadPointer(ch);
        for (int s = 0; s < numSamples; ++s)
        {
            out[s] = out[s] + wet[s] * currentAmount * 0.7f;
        }
    }

    juce::dsp::AudioBlock<float> wetBlock(wetBuffer.getArrayOfWritePointers(),
                                          numChannels, numSamples);
    juce::dsp::ProcessContextReplacing<float> wetCtx(wetBlock);
    metalFilter.process(wetCtx);
}
