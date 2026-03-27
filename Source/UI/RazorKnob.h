#pragma once
#include <JuceHeader.h>
#include "RazorLookAndFeel.h"

class RazorKnob : public juce::Component
{
public:
    RazorKnob(const juce::String& paramName,
              juce::AudioProcessorValueTreeState& apvts,
              RazorLookAndFeel& lnf);
    ~RazorKnob() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    juce::Slider slider;

private:
    juce::Label nameLabel;
    juce::Label valueLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    RazorLookAndFeel& lookAndFeel;
    juce::String parameterName;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RazorKnob)
};
