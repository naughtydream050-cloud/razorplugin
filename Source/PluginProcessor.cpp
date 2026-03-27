#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout RazorAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "gash", "GASH",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.75f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "blood", "BLOOD",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.40f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "sharpen", "SHARPEN",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.60f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "recoil", "RECOIL",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.30f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mixtape", "MIXTAPE",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.85f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    return layout;
}

RazorAudioProcessor::RazorAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout()),
      presetManager(apvts)
{
    apvts.addParameterListener("gash", this);
    apvts.addParameterListener("blood", this);
    apvts.addParameterListener("sharpen", this);
    apvts.addParameterListener("recoil", this);
    apvts.addParameterListener("mixtape", this);
}

RazorAudioProcessor::~RazorAudioProcessor()
{
    apvts.removeParameterListener("gash", this);
    apvts.removeParameterListener("blood", this);
    apvts.removeParameterListener("sharpen", this);
    apvts.removeParameterListener("recoil", this);
    apvts.removeParameterListener("mixtape", this);
}

void RazorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    gashDSP.prepare(spec);
    bloodDSP.prepare(spec);
    sharpenDSP.prepare(spec);
    recoilDSP.prepare(spec);
    mixtapeDSP.prepare(spec);
}

void RazorAudioProcessor::releaseResources()
{
    gashDSP.reset();
    bloodDSP.reset();
    sharpenDSP.reset();
    recoilDSP.reset();
    mixtapeDSP.reset();
}

bool RazorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void RazorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;

    float gashVal    = *apvts.getRawParameterValue("gash");
    float bloodVal   = *apvts.getRawParameterValue("blood");
    float sharpenVal = *apvts.getRawParameterValue("sharpen");
    float recoilVal  = *apvts.getRawParameterValue("recoil");
    float mixtapeVal = *apvts.getRawParameterValue("mixtape");

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    gashDSP.setAmount(gashVal);
    gashDSP.process(context);

    bloodDSP.setAmount(bloodVal);
    bloodDSP.process(context);

    sharpenDSP.setAmount(sharpenVal);
    sharpenDSP.process(context);

    recoilDSP.setAmount(recoilVal);
    recoilDSP.process(context);

    mixtapeDSP.setAmount(mixtapeVal);
    mixtapeDSP.process(context);
}

void RazorAudioProcessor::parameterChanged(const juce::String& /*parameterID*/, float /*newValue*/)
{
}

juce::AudioProcessorEditor* RazorAudioProcessor::createEditor()
{
    return new RazorAudioProcessorEditor(*this);
}

int RazorAudioProcessor::getNumPrograms() { return presetManager.getNumPresets(); }
int RazorAudioProcessor::getCurrentProgram() { return presetManager.getCurrentPresetIndex(); }
void RazorAudioProcessor::setCurrentProgram(int index) { presetManager.loadPreset(index); }
const juce::String RazorAudioProcessor::getProgramName(int index) { return presetManager.getPresetName(index); }
void RazorAudioProcessor::changeProgramName(int /*index*/, const juce::String& /*newName*/) {}

void RazorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void RazorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RazorAudioProcessor();
}
