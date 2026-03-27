#include "RazorLookAndFeel.h"

RazorLookAndFeel::RazorLookAndFeel()
{
    setColour(juce::ResizableWindow::backgroundColourId, razorBlack());
    setColour(juce::Label::textColourId, razorBloodRed());
    setColour(juce::Slider::rotarySliderFillColourId, razorBloodRed());
    setColour(juce::Slider::rotarySliderOutlineColourId, razorMetal());
    setColour(juce::ComboBox::backgroundColourId, razorDarkGrey());
    setColour(juce::ComboBox::textColourId, razorLCDGreen());
    setColour(juce::ComboBox::outlineColourId, razorRed());
    setColour(juce::PopupMenu::backgroundColourId, razorBlack());
    setColour(juce::PopupMenu::textColourId, razorBloodRed());
    setColour(juce::PopupMenu::highlightedBackgroundColourId, razorRed());
    setColour(juce::PopupMenu::highlightedTextColourId, juce::Colours::white);
}

void RazorLookAndFeel::drawMetallicBackground(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    juce::ColourGradient radialGrad(
        razorMetal().brighter(0.3f), bounds.getCentreX(), bounds.getCentreY(),
        razorBlack(), bounds.getRight(), bounds.getBottom(), true);
    g.setGradientFill(radialGrad);
    g.fillEllipse(bounds);

    g.setColour(razorBlack().withAlpha(0.5f));
    for (int i = 0; i < 12; ++i)
    {
        float angle = juce::MathConstants<float>::twoPi * i / 12.0f;
        float r1 = bounds.getWidth() * 0.35f;
        float r2 = bounds.getWidth() * 0.48f;
        float cx = bounds.getCentreX();
        float cy = bounds.getCentreY();
        g.drawLine(cx + r1 * std::cos(angle), cy + r1 * std::sin(angle),
                   cx + r2 * std::cos(angle), cy + r2 * std::sin(angle), 1.0f);
    }

    g.setColour(razorMetal().darker(0.5f));
    g.drawEllipse(bounds, 2.0f);

    juce::ColourGradient rimGrad(
        juce::Colours::white.withAlpha(0.2f), bounds.getX(), bounds.getY(),
        juce::Colours::transparentBlack, bounds.getCentreX(), bounds.getCentreY(), false);
    g.setGradientFill(rimGrad);
    g.fillEllipse(bounds);
}

void RazorLookAndFeel::drawBloodSplatter(juce::Graphics& g, juce::Rectangle<float> bounds, float seed)
{
    juce::Random rng(static_cast<juce::int64>(seed * 10000));
    g.setColour(razorBloodRed().withAlpha(0.6f));
    for (int i = 0; i < 5; ++i)
    {
        float angle = rng.nextFloat() * juce::MathConstants<float>::twoPi;
        float dist  = rng.nextFloat() * bounds.getWidth() * 0.45f + bounds.getWidth() * 0.05f;
        float cx    = bounds.getCentreX() + dist * std::cos(angle);
        float cy    = bounds.getCentreY() + dist * std::sin(angle);
        float r     = rng.nextFloat() * 3.0f + 1.0f;
        g.fillEllipse(cx - r, cy - r, r * 2, r * 2);

        for (int j = 0; j < 3; ++j)
        {
            float da   = (rng.nextFloat() - 0.5f) * 0.8f + angle;
            float dl   = rng.nextFloat() * 8.0f + 2.0f;
            float ex   = cx + dl * std::cos(da);
            float ey   = cy + dl * std::sin(da);
            g.drawLine(cx, cy, ex, ey, rng.nextFloat() * 1.5f + 0.5f);
        }
    }
}

void RazorLookAndFeel::drawBladeKnob(juce::Graphics& g, juce::Rectangle<float> bounds,
                                      float normalizedValue, float startAngle, float endAngle)
{
    float cx    = bounds.getCentreX();
    float cy    = bounds.getCentreY();
    float radius = bounds.getWidth() * 0.5f;

    juce::Path outerRing;
    outerRing.addEllipse(bounds.reduced(2.0f));
    juce::ColourGradient outerGrad(
        razorMetal(), cx, bounds.getY(),
        razorBlack(), cx, bounds.getBottom(), false);
    g.setGradientFill(outerGrad);
    g.fillPath(outerRing);

    g.setColour(razorRed().withAlpha(0.8f));
    g.drawEllipse(bounds.reduced(2.0f), 1.5f);

    float arcEnd = startAngle + normalizedValue * (endAngle - startAngle);
    juce::Path arcPath;
    arcPath.addArc(bounds.reduced(radius * 0.12f).getX(),
                   bounds.reduced(radius * 0.12f).getY(),
                   bounds.reduced(radius * 0.12f).getWidth(),
                   bounds.reduced(radius * 0.12f).getHeight(),
                   startAngle, arcEnd, true);
    juce::PathStrokeType strokeType(3.0f);
    g.setColour(razorBloodRed());
    g.strokePath(arcPath, strokeType);

    juce::ColourGradient bodyGrad(
        razorMetal().brighter(0.2f), cx - radius * 0.3f, cy - radius * 0.3f,
        razorBlack(), cx + radius * 0.3f, cy + radius * 0.3f, false);
    g.setGradientFill(bodyGrad);
    g.fillEllipse(bounds.reduced(radius * 0.25f));

    int numBlades = 6;
    g.saveState();
    for (int i = 0; i < numBlades; ++i)
    {
        float bladeAngle = juce::MathConstants<float>::twoPi * i / numBlades;
        float bx1 = cx + radius * 0.28f * std::cos(bladeAngle);
        float by1 = cy + radius * 0.28f * std::sin(bladeAngle);
        float bx2 = cx + radius * 0.45f * std::cos(bladeAngle + 0.3f);
        float by2 = cy + radius * 0.45f * std::sin(bladeAngle + 0.3f);
        float bx3 = cx + radius * 0.45f * std::cos(bladeAngle - 0.3f);
        float by3 = cy + radius * 0.45f * std::sin(bladeAngle - 0.3f);

        juce::Path blade;
        blade.startNewSubPath(bx1, by1);
        blade.lineTo(bx2, by2);
        blade.lineTo(cx + radius * 0.38f * std::cos(bladeAngle + 0.15f),
                     cy + radius * 0.38f * std::sin(bladeAngle + 0.15f));
        blade.lineTo(bx3, by3);
        blade.closeSubPath();

        juce::ColourGradient bladeGrad(
            juce::Colours::silver, bx2, by2,
            razorMetal().darker(0.5f), bx3, by3, false);
        g.setGradientFill(bladeGrad);
        g.fillPath(blade);

        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.strokePath(blade, juce::PathStrokeType(0.5f));
    }
    g.restoreState();

    float pointerAngle = startAngle + normalizedValue * (endAngle - startAngle);
    float px1 = cx + radius * 0.15f * std::cos(pointerAngle);
    float py1 = cy + radius * 0.15f * std::sin(pointerAngle);
    float px2 = cx + radius * 0.42f * std::cos(pointerAngle);
    float py2 = cy + radius * 0.42f * std::sin(pointerAngle);

    g.setColour(juce::Colours::white.withAlpha(0.9f));
    g.drawLine(px1, py1, px2, py2, 2.5f);

    g.setColour(juce::Colours::white.withAlpha(0.4f));
    g.fillEllipse(cx - 3.0f, cy - 3.0f, 6.0f, 6.0f);

    g.setColour(razorMetal().darker(0.3f));
    g.drawEllipse(bounds.reduced(radius * 0.25f), 1.0f);
}

void RazorLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                         juce::Slider& slider)
{
    juce::Rectangle<float> bounds((float)x, (float)y, (float)width, (float)height);
    bounds = bounds.reduced(4.0f);

    float shadowAlpha = 0.5f;
    g.setColour(juce::Colours::black.withAlpha(shadowAlpha));
    g.fillEllipse(bounds.translated(3.0f, 3.0f));

    drawMetallicBackground(g, bounds);

    float seed = (float)(bounds.getCentreX() * 7 + bounds.getCentreY() * 13);
    drawBloodSplatter(g, bounds, seed);

    drawBladeKnob(g, bounds, sliderPos, rotaryStartAngle, rotaryEndAngle);

    juce::ignoreUnused(slider);
}

void RazorLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.setColour(razorBlack().withAlpha(0.0f));
    g.fillRect(label.getLocalBounds());

    if (!label.isBeingEdited())
    {
        g.setColour(label.findColour(juce::Label::textColourId));
        g.setFont(getLabelFont(label));
        g.drawFittedText(label.getText(), label.getLocalBounds(),
                         label.getJustificationType(), 1, label.getMinimumHorizontalScale());
    }
}

juce::Font RazorLookAndFeel::getLabelFont(juce::Label& /*label*/)
{
    return juce::Font(juce::Font::getDefaultMonospacedFontName(), 12.0f, juce::Font::bold);
}
