#pragma once
#include <JuceHeader.h>

class RazorLookAndFeel : public juce::LookAndFeel_V4
{
public:
    RazorLookAndFeel();

    static juce::Colour razorBlack()    { return juce::Colour(0xFF0A0A0A); }
    static juce::Colour razorDarkGrey() { return juce::Colour(0xFF1A1A1A); }
    static juce::Colour razorRed()      { return juce::Colour(0xFF8B0000); }
    static juce::Colour razorBloodRed() { return juce::Colour(0xFFCC0000); }
    static juce::Colour razorBrightRed(){ return juce::Colour(0xFFFF2200); }
    static juce::Colour razorMetal()    { return juce::Colour(0xFF3A3A3A); }
    static juce::Colour razorLCDGreen() { return juce::Colour(0xFF88FF44); }
    static juce::Colour razorLCDBg()    { return juce::Colour(0xFF0F1A08); }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override;

    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    juce::Font getLabelFont(juce::Label& label) override;

private:
    void drawBladeKnob(juce::Graphics& g, juce::Rectangle<float> bounds,
                       float normalizedValue, float startAngle, float endAngle);
    void drawMetallicBackground(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawBloodSplatter(juce::Graphics& g, juce::Rectangle<float> bounds, float seed);
};
