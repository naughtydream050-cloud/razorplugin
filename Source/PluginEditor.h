#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/RazorLookAndFeel.h"
#include "UI/RazorKnob.h"

class RazorAudioProcessorEditor : public juce::AudioProcessorEditor,
                                   public juce::ComboBox::Listener
{
public:
    explicit RazorAudioProcessorEditor(RazorAudioProcessor&);
    ~RazorAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox* box) override;

private:
    RazorAudioProcessor& audioProcessor;
    RazorLookAndFeel razorLnf;

    std::unique_ptr<RazorKnob> gashKnob;
    std::unique_ptr<RazorKnob> bloodKnob;
    std::unique_ptr<RazorKnob> sharpenKnob;
    std::unique_ptr<RazorKnob> recoilKnob;
    std::unique_ptr<RazorKnob> mixtapeKnob;

    juce::ComboBox presetBox;
    juce::Label    presetDisplay;

    void paintBackground(juce::Graphics& g);
    void paintTitle(juce::Graphics& g);
    void paintDevilIllustration(juce::Graphics& g, juce::Rectangle<float> bounds);
    void paintKrumpGraffiti(juce::Graphics& g, juce::Rectangle<float> bounds);
    void paintRuneDecorations(juce::Graphics& g);
    void paintChainDecoration(juce::Graphics& g);
    void paintBloodDrips(juce::Graphics& g);
    void paintLCDPanel(juce::Graphics& g, juce::Rectangle<float> bounds);

    static juce::Colour razorBlack()     { return juce::Colour(0xFF0A0A0A); }
    static juce::Colour razorRed()       { return juce::Colour(0xFF8B0000); }
    static juce::Colour razorBloodRed()  { return juce::Colour(0xFFCC0000); }
    static juce::Colour razorBrightRed() { return juce::Colour(0xFFFF2200); }
    static juce::Colour razorMetal()     { return juce::Colour(0xFF2A2A2A); }
    static juce::Colour razorLCDGreen()  { return juce::Colour(0xFF88FF44); }
    static juce::Colour razorLCDBg()     { return juce::Colour(0xFF0F1A08); }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RazorAudioProcessorEditor)
};
