#include "PluginEditor.h"

RazorAudioProcessorEditor::RazorAudioProcessorEditor(RazorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setLookAndFeel(&razorLnf);
    setSize(820, 480);

    gashKnob    = std::make_unique<RazorKnob>("gash",    audioProcessor.apvts, razorLnf);
    bloodKnob   = std::make_unique<RazorKnob>("blood",   audioProcessor.apvts, razorLnf);
    sharpenKnob = std::make_unique<RazorKnob>("sharpen", audioProcessor.apvts, razorLnf);
    recoilKnob  = std::make_unique<RazorKnob>("recoil",  audioProcessor.apvts, razorLnf);
    mixtapeKnob = std::make_unique<RazorKnob>("mixtape", audioProcessor.apvts, razorLnf);

    addAndMakeVisible(*gashKnob);
    addAndMakeVisible(*bloodKnob);
    addAndMakeVisible(*sharpenKnob);
    addAndMakeVisible(*recoilKnob);
    addAndMakeVisible(*mixtapeKnob);

    const auto& presets = audioProcessor.presetManager.getPresets();
    for (int i = 0; i < presets.size(); ++i)
        presetBox.addItem(presets[i].name, i + 1);

    presetBox.setSelectedId(audioProcessor.presetManager.getCurrentPresetIndex() + 1,
                            juce::dontSendNotification);
    presetBox.addListener(this);
    presetBox.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF111111));
    presetBox.setColour(juce::ComboBox::textColourId, razorLCDGreen());
    presetBox.setColour(juce::ComboBox::outlineColourId, razorRed());
    addAndMakeVisible(presetBox);

    presetDisplay.setText(audioProcessor.presetManager.getPresetName(
                              audioProcessor.presetManager.getCurrentPresetIndex()),
                          juce::dontSendNotification);
    presetDisplay.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 22.0f, juce::Font::bold));
    presetDisplay.setColour(juce::Label::textColourId, razorLCDGreen());
    presetDisplay.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(presetDisplay);
}

RazorAudioProcessorEditor::~RazorAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void RazorAudioProcessorEditor::comboBoxChanged(juce::ComboBox* box)
{
    if (box == &presetBox)
    {
        int idx = presetBox.getSelectedId() - 1;
        audioProcessor.presetManager.loadPreset(idx);
        presetDisplay.setText(audioProcessor.presetManager.getPresetName(idx),
                              juce::dontSendNotification);
    }
}

void RazorAudioProcessorEditor::paintBackground(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    juce::ColourGradient bgGrad(
        juce::Colour(0xFF0D0D0D), 0.0f, 0.0f,
        juce::Colour(0xFF050505), bounds.getWidth(), bounds.getHeight(), false);
    g.setGradientFill(bgGrad);
    g.fillAll();

    g.setColour(juce::Colour(0xFF1A1A1A).withAlpha(0.4f));
    for (float y = 0.0f; y < bounds.getHeight(); y += 8.0f)
    {
        g.drawHorizontalLine(static_cast<int>(y), 0, bounds.getWidth());
    }
    for (float x = 0.0f; x < bounds.getWidth(); x += 8.0f)
    {
        g.drawVerticalLine(static_cast<int>(x), 0, bounds.getHeight());
    }

    g.setColour(razorRed().withAlpha(0.15f));
    g.fillRect(0, 0, 4, getHeight());
    g.fillRect(getWidth() - 4, 0, 4, getHeight());
    g.fillRect(0, 0, getWidth(), 4);
    g.fillRect(0, getHeight() - 4, getWidth(), 4);

    juce::ColourGradient vigGrad(
        juce::Colours::transparentBlack, bounds.getCentreX(), bounds.getCentreY(),
        juce::Colours::black.withAlpha(0.8f), 0.0f, 0.0f, true);
    g.setGradientFill(vigGrad);
    g.fillAll();
}

void RazorAudioProcessorEditor::paintTitle(juce::Graphics& g)
{
    auto w = getWidth();

    juce::String title = "WELCOME TO RAZOR";
    juce::Font titleFont(juce::Font::getDefaultSansSerifFontName(), 52.0f,
                         juce::Font::bold | juce::Font::italic);

    float tx = 190.0f;
    float ty = 12.0f;
    float tw = w - 220.0f;
    float th = 72.0f;

    for (int i = 4; i >= 1; --i)
    {
        float alpha = 0.15f / i;
        float expand = static_cast<float>(i) * 3.0f;
        g.setColour(razorBloodRed().withAlpha(alpha));
        g.setFont(titleFont);
        g.drawText(title, static_cast<int>(tx - expand),
                   static_cast<int>(ty - expand / 2),
                   static_cast<int>(tw + expand * 2),
                   static_cast<int>(th + expand),
                   juce::Justification::centred, false);
    }

    juce::ColourGradient titleGrad(
        razorBrightRed(), tx, ty,
        razorBloodRed(), tx + tw, ty + th, false);
    g.setGradientFill(titleGrad);
    g.setFont(titleFont);
    g.drawText(title, static_cast<int>(tx), static_cast<int>(ty),
               static_cast<int>(tw), static_cast<int>(th),
               juce::Justification::centred, false);

    g.setColour(juce::Colours::white.withAlpha(0.08f));
    g.drawText(title, static_cast<int>(tx) + 1, static_cast<int>(ty) + 1,
               static_cast<int>(tw), static_cast<int>(th),
               juce::Justification::centred, false);

    g.setColour(razorBloodRed().withAlpha(0.5f));
    for (int i = 0; i < 5; ++i)
    {
        float drip_x = 200.0f + i * 90.0f;
        float len    = 10.0f + (i % 3) * 8.0f;
        float width  = 1.5f + (i % 2) * 1.0f;
        g.drawLine(drip_x, ty + th - 5, drip_x + 2.0f, ty + th + len, width);
        g.fillEllipse(drip_x + 1.0f, ty + th + len - 2.0f, width * 2.5f, width * 2.5f);
    }
}

void RazorAudioProcessorEditor::paintDevilIllustration(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    float cx = bounds.getCentreX();
    float cy = bounds.getCentreY() + 10.0f;
    float r  = bounds.getWidth() * 0.38f;

    juce::ColourGradient auraGrad(
        razorRed().withAlpha(0.3f), cx, cy,
        juce::Colours::transparentBlack, cx, cy - r * 1.5f, true);
    g.setGradientFill(auraGrad);
    g.fillEllipse(cx - r * 1.4f, cy - r * 1.4f, r * 2.8f, r * 2.8f);

    juce::ColourGradient faceGrad(
        juce::Colour(0xFF1A1000), cx - r * 0.2f, cy - r * 0.3f,
        juce::Colour(0xFF0A0800), cx + r * 0.3f, cy + r * 0.5f, false);
    g.setGradientFill(faceGrad);
    juce::Path faceShape;
    faceShape.addEllipse(cx - r * 0.65f, cy - r * 0.7f, r * 1.3f, r * 1.4f);
    g.fillPath(faceShape);

    g.setColour(juce::Colour(0xFF2A1500).withAlpha(0.8f));
    g.strokePath(faceShape, juce::PathStrokeType(2.0f));

    auto drawHorn = [&](bool isLeft)
    {
        float sign = isLeft ? -1.0f : 1.0f;
        float hbx  = cx + sign * r * 0.35f;
        float hby  = cy - r * 0.55f;

        juce::Path horn;
        horn.startNewSubPath(hbx, hby);
        horn.cubicTo(hbx + sign * r * 0.15f, hby - r * 0.6f,
                     hbx + sign * r * 0.3f,  hby - r * 0.4f,
                     hbx + sign * r * 0.1f,  hby - r * 0.85f);

        juce::ColourGradient hornGrad(
            juce::Colour(0xFF3A1500), hbx, hby,
            juce::Colour(0xFF1A0500), hbx + sign * r * 0.1f, hby - r * 0.85f, false);
        g.setGradientFill(hornGrad);
        g.strokePath(horn, juce::PathStrokeType(8.0f,
                                                juce::PathStrokeType::curved,
                                                juce::PathStrokeType::rounded));

        g.setColour(juce::Colour(0xFF5A2000).withAlpha(0.5f));
        g.strokePath(horn, juce::PathStrokeType(5.0f));
    };
    drawHorn(true);
    drawHorn(false);

    g.setColour(juce::Colour(0xFFFF6600));
    float eyeRadius = r * 0.10f;
    float eyeGlowR  = r * 0.18f;

    auto drawEye = [&](float ex, float ey)
    {
        juce::ColourGradient eyeGlow(
            juce::Colour(0xFFFF4400).withAlpha(0.6f), ex, ey,
            juce::Colours::transparentBlack, ex + eyeGlowR, ey, true);
        g.setGradientFill(eyeGlow);
        g.fillEllipse(ex - eyeGlowR, ey - eyeGlowR, eyeGlowR * 2.0f, eyeGlowR * 2.0f);

        g.setColour(juce::Colour(0xFFFF6600));
        g.fillEllipse(ex - eyeRadius, ey - eyeRadius, eyeRadius * 2.0f, eyeRadius * 2.0f);

        g.setColour(juce::Colour(0xFFFF9900));
        g.fillEllipse(ex - eyeRadius * 0.4f, ey - eyeRadius * 0.4f,
                      eyeRadius * 0.8f, eyeRadius * 0.8f);
    };

    drawEye(cx - r * 0.25f, cy - r * 0.15f);
    drawEye(cx + r * 0.25f, cy - r * 0.15f);

    g.setColour(juce::Colour(0xFF2A0000));
    juce::Path mouthPath;
    mouthPath.startNewSubPath(cx - r * 0.3f, cy + r * 0.2f);
    mouthPath.cubicTo(cx - r * 0.1f, cy + r * 0.35f,
                      cx + r * 0.1f, cy + r * 0.35f,
                      cx + r * 0.3f, cy + r * 0.2f);
    g.strokePath(mouthPath, juce::PathStrokeType(3.0f));

    float toothW = r * 0.07f;
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    for (int i = -2; i <= 2; ++i)
    {
        float tx2 = cx + i * toothW * 1.4f;
        float ty2 = cy + r * 0.22f;
        juce::Path tooth;
        tooth.addTriangle(tx2 - toothW * 0.5f, ty2,
                          tx2 + toothW * 0.5f, ty2,
                          tx2, ty2 + toothW);
        g.fillPath(tooth);
    }

    juce::Path nosePath;
    nosePath.addEllipse(cx - r * 0.1f, cy + r * 0.02f, r * 0.2f, r * 0.12f);
    g.setColour(juce::Colour(0xFF1A0800).withAlpha(0.8f));
    g.fillPath(nosePath);

    float runeSize = r * 0.15f;
    g.setColour(razorBloodRed().withAlpha(0.7f));
    g.setFont(juce::Font(runeSize));
    g.drawText(juce::CharPointer_UTF8("\xe2\x9d\x96"), static_cast<int>(cx - r * 0.55f),
               static_cast<int>(cy - r * 0.4f), static_cast<int>(runeSize * 1.5f),
               static_cast<int>(runeSize * 1.5f), juce::Justification::centred);
}

void RazorAudioProcessorEditor::paintKrumpGraffiti(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    float cx = bounds.getCentreX() - 10.0f;
    float cy = bounds.getBottom() - 30.0f;

    juce::Font grafFont(juce::Font::getDefaultSansSerifFontName(), 32.0f,
                        juce::Font::bold | juce::Font::italic);
    g.setFont(grafFont);

    for (int i = 3; i >= 1; --i)
    {
        float alpha = 0.12f / i;
        g.setColour(razorBloodRed().withAlpha(alpha));
        g.drawText("KRUMP", static_cast<int>(cx - 60 - i),
                   static_cast<int>(cy - i), 140, 40,
                   juce::Justification::centred, false);
    }

    juce::ColourGradient kGrad(
        razorBrightRed(), cx - 50.0f, cy,
        razorBloodRed().darker(0.5f), cx + 50.0f, cy + 30.0f, false);
    g.setGradientFill(kGrad);
    g.drawText("KRUMP", static_cast<int>(cx - 60), static_cast<int>(cy),
               140, 40, juce::Justification::centred, false);

    g.setColour(razorRed().withAlpha(0.3f));
    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 11.0f, juce::Font::plain));
    g.drawText("$", static_cast<int>(cx + 55), static_cast<int>(cy + 5), 20, 20,
               juce::Justification::centred);
}

void RazorAudioProcessorEditor::paintRuneDecorations(juce::Graphics& g)
{
    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 13.0f, juce::Font::plain));

    struct RunePos { float x, y; juce::String rune; float alpha; };
    std::vector<RunePos> runes = {
        { 10.0f,  100.0f, juce::CharPointer_UTF8("\xe2\x9c\x9e"), 0.4f },
        { 10.0f,  120.0f, juce::CharPointer_UTF8("\xe2\x9c\xa1"), 0.35f },
        { 10.0f,  140.0f, juce::CharPointer_UTF8("\xe2\x98\x87"), 0.3f },
        { 10.0f,  160.0f, juce::CharPointer_UTF8("\xe2\x9c\xb4"), 0.4f },
        { 10.0f,  180.0f, juce::CharPointer_UTF8("\xe2\x9c\xb3"), 0.35f },
        { 10.0f,  200.0f, juce::CharPointer_UTF8("\xe2\x9d\x96"), 0.3f },
        { 775.0f, 100.0f, juce::CharPointer_UTF8("\xe2\x9c\x9e"), 0.4f },
        { 775.0f, 120.0f, juce::CharPointer_UTF8("\xe2\x98\x87"), 0.35f },
        { 775.0f, 140.0f, juce::CharPointer_UTF8("\xe2\x9c\xa1"), 0.3f },
        { 775.0f, 160.0f, juce::CharPointer_UTF8("\xe2\x9d\x96"), 0.4f },
        { 775.0f, 180.0f, juce::CharPointer_UTF8("\xe2\x9c\xb4"), 0.35f },
        { 775.0f, 200.0f, juce::CharPointer_UTF8("\xe2\x9c\xb3"), 0.3f },
    };

    for (const auto& r : runes)
    {
        g.setColour(razorBloodRed().withAlpha(r.alpha));
        g.drawText(r.rune, static_cast<int>(r.x), static_cast<int>(r.y), 16, 16,
                   juce::Justification::centred);
    }

    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 9.0f, juce::Font::plain));
    g.setColour(razorBloodRed().withAlpha(0.3f));

    juce::String runeStrip = "MENO KA*ER \\ PRO  RAW";
    g.drawText(runeStrip, 180, 260, 460, 20, juce::Justification::centred, false);

    g.setColour(razorRed().withAlpha(0.2f));
    g.drawText("*", 700, 110, 40, 40, juce::Justification::centred);
    g.drawText("*", 720, 150, 30, 30, juce::Justification::centred);
    g.drawText("*", 695, 200, 35, 35, juce::Justification::centred);
    g.drawText("*", 700, 250, 30, 30, juce::Justification::centred);
}

void RazorAudioProcessorEditor::paintChainDecoration(juce::Graphics& g)
{
    g.setColour(razorMetal().withAlpha(0.5f));

    float chainY = 250.0f;
    for (float x = 20.0f; x < getWidth() - 20.0f; x += 18.0f)
    {
        bool horiz = static_cast<int>(x / 18.0f) % 2 == 0;
        if (horiz)
            g.drawEllipse(x, chainY - 4.0f, 14.0f, 8.0f, 1.5f);
        else
            g.drawEllipse(x + 4.0f, chainY - 7.0f, 8.0f, 14.0f, 1.5f);
    }
}

void RazorAudioProcessorEditor::paintBloodDrips(juce::Graphics& g)
{
    g.setColour(razorBloodRed().withAlpha(0.5f));

    struct Drip { float x, topY, len, w; };
    std::vector<Drip> drips = {
        { 195.0f, 68.0f, 25.0f, 2.0f },
        { 300.0f, 72.0f, 15.0f, 1.5f },
        { 430.0f, 65.0f, 35.0f, 2.5f },
        { 550.0f, 70.0f, 20.0f, 2.0f },
        { 670.0f, 68.0f, 18.0f, 1.5f },
    };

    for (const auto& d : drips)
    {
        g.drawLine(d.x, d.topY, d.x + 1.0f, d.topY + d.len, d.w);
        g.fillEllipse(d.x - d.w * 0.5f, d.topY + d.len - 1.0f,
                      d.w * 2.5f, d.w * 2.5f);
    }
}

void RazorAudioProcessorEditor::paintLCDPanel(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    g.setColour(juce::Colour(0xFF0A0A0A));
    g.fillRoundedRectangle(bounds.expanded(4.0f), 4.0f);

    juce::ColourGradient panelGrad(
        razorLCDBg(), bounds.getX(), bounds.getY(),
        juce::Colour(0xFF0A1205), bounds.getX(), bounds.getBottom(), false);
    g.setGradientFill(panelGrad);
    g.fillRoundedRectangle(bounds, 3.0f);

    g.setColour(razorBloodRed().withAlpha(0.8f));
    g.drawRoundedRectangle(bounds.expanded(1.5f), 4.0f, 2.0f);

    g.setColour(razorLCDGreen().withAlpha(0.06f));
    for (float py = bounds.getY(); py < bounds.getBottom(); py += 3.0f)
        g.drawHorizontalLine(static_cast<int>(py), bounds.getX(), bounds.getRight());

    g.setColour(juce::Colours::white.withAlpha(0.06f));
    juce::ColourGradient reflection(
        juce::Colours::white.withAlpha(0.08f), bounds.getX(), bounds.getY(),
        juce::Colours::transparentBlack, bounds.getX(), bounds.getY() + bounds.getHeight() * 0.4f, false);
    g.setGradientFill(reflection);
    g.fillRoundedRectangle(bounds, 3.0f);
}

void RazorAudioProcessorEditor::paint(juce::Graphics& g)
{
    paintBackground(g);
    paintTitle(g);
    paintBloodDrips(g);

    auto devilBounds = juce::Rectangle<float>(8.0f, 60.0f, 170.0f, 200.0f);
    paintDevilIllustration(g, devilBounds);

    auto krumpBounds = juce::Rectangle<float>(8.0f, 200.0f, 170.0f, 80.0f);
    paintKrumpGraffiti(g, krumpBounds);

    paintRuneDecorations(g);
    paintChainDecoration(g);

    auto lcdBounds = juce::Rectangle<float>(190.0f, 100.0f, 360.0f, 58.0f);
    paintLCDPanel(g, lcdBounds);

    auto rightListBounds = juce::Rectangle<float>(572.0f, 100.0f, 230.0f, 150.0f);
    g.setColour(juce::Colour(0xFF110000).withAlpha(0.7f));
    g.fillRoundedRectangle(rightListBounds, 3.0f);
    g.setColour(razorRed().withAlpha(0.4f));
    g.drawRoundedRectangle(rightListBounds, 3.0f, 1.0f);

    const auto& presets = audioProcessor.presetManager.getPresets();
    int currentIdx = audioProcessor.presetManager.getCurrentPresetIndex();
    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 13.0f, juce::Font::plain));
    for (int i = 0; i < presets.size(); ++i)
    {
        float itemY = rightListBounds.getY() + 8.0f + i * 33.0f;
        if (i == currentIdx)
        {
            g.setColour(razorRed().withAlpha(0.3f));
            g.fillRoundedRectangle(rightListBounds.getX() + 4.0f, itemY - 2.0f,
                                   rightListBounds.getWidth() - 8.0f, 26.0f, 2.0f);
            g.setColour(razorBrightRed());
        }
        else
        {
            g.setColour(razorBloodRed().withAlpha(0.7f));
        }
        g.drawText(presets[i].name, static_cast<int>(rightListBounds.getX() + 12.0f),
                   static_cast<int>(itemY), static_cast<int>(rightListBounds.getWidth() - 16.0f),
                   24, juce::Justification::centredLeft, false);
    }
}

void RazorAudioProcessorEditor::resized()
{
    int knobW = 130;
    int knobH = 150;
    int knobY = getHeight() - knobH - 10;

    int totalKnobsW = knobW * 5;
    int startX = (getWidth() - totalKnobsW) / 2;

    gashKnob->setBounds(startX + 0 * knobW, knobY, knobW, knobH);
    bloodKnob->setBounds(startX + 1 * knobW, knobY, knobW, knobH);
    sharpenKnob->setBounds(startX + 2 * knobW, knobY, knobW, knobH);
    recoilKnob->setBounds(startX + 3 * knobW, knobY, knobW, knobH);
    mixtapeKnob->setBounds(startX + 4 * knobW, knobY, knobW, knobH);

    presetDisplay.setBounds(200, 108, 345, 42);

    presetBox.setBounds(572, 260, 230, 28);
}
