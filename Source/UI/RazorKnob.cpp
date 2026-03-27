#include "RazorKnob.h"

RazorKnob::RazorKnob(const juce::String& paramName,
                     juce::AudioProcessorValueTreeState& apvts,
                     RazorLookAndFeel& lnf)
    : lookAndFeel(lnf), parameterName(paramName)
{
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setLookAndFeel(&lnf);
    addAndMakeVisible(slider);

    nameLabel.setText(paramName.toUpperCase(), juce::dontSendNotification);
    nameLabel.setColour(juce::Label::textColourId, RazorLookAndFeel::razorBloodRed());
    nameLabel.setJustificationType(juce::Justification::centred);
    nameLabel.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 11.0f, juce::Font::bold));
    addAndMakeVisible(nameLabel);

    valueLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.7f));
    valueLabel.setJustificationType(juce::Justification::centred);
    valueLabel.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 10.0f, juce::Font::plain));
    addAndMakeVisible(valueLabel);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, paramName.toLowerCase(), slider);

    slider.onValueChange = [this]
    {
        int pct = static_cast<int>(std::round(slider.getValue() * 100.0));
        valueLabel.setText(juce::String(pct) + "%", juce::dontSendNotification);
    };
    slider.onValueChange();
}

RazorKnob::~RazorKnob()
{
    slider.setLookAndFeel(nullptr);
}

void RazorKnob::resized()
{
    auto area = getLocalBounds();
    nameLabel.setBounds(area.removeFromBottom(18));
    valueLabel.setBounds(area.removeFromBottom(15));
    slider.setBounds(area);
}

void RazorKnob::paint(juce::Graphics& /*g*/)
{
}
