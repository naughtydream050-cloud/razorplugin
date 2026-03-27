#include "PresetManager.h"

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts)
    : apvts(apvts)
{
    initPresets();
}

void PresetManager::initPresets()
{
    presets.clear();

    PresetData devilChorus;
    devilChorus.name    = "Devil chorus";
    devilChorus.gash    = 0.75f;
    devilChorus.blood   = 0.40f;
    devilChorus.sharpen = 0.60f;
    devilChorus.recoil  = 0.30f;
    devilChorus.mixtape = 0.85f;
    presets.add(devilChorus);

    PresetData savageHell;
    savageHell.name    = "SAVAGE Hell";
    savageHell.gash    = 0.95f;
    savageHell.blood   = 0.80f;
    savageHell.sharpen = 0.90f;
    savageHell.recoil  = 0.15f;
    savageHell.mixtape = 0.40f;
    presets.add(savageHell);

    PresetData darkRaw;
    darkRaw.name    = "Dark Raw";
    darkRaw.gash    = 0.55f;
    darkRaw.blood   = 0.70f;
    darkRaw.sharpen = 0.35f;
    darkRaw.recoil  = 0.50f;
    darkRaw.mixtape = 0.65f;
    presets.add(darkRaw);

    PresetData solidBuck;
    solidBuck.name    = "Solid Buck";
    solidBuck.gash    = 0.40f;
    solidBuck.blood   = 0.90f;
    solidBuck.sharpen = 0.50f;
    solidBuck.recoil  = 0.20f;
    solidBuck.mixtape = 0.30f;
    presets.add(solidBuck);
}

int PresetManager::getNumPresets() const
{
    return presets.size();
}

int PresetManager::getCurrentPresetIndex() const
{
    return currentPresetIndex;
}

juce::String PresetManager::getPresetName(int index) const
{
    if (index >= 0 && index < presets.size())
        return presets[index].name;
    return "Unknown";
}

void PresetManager::loadPreset(int index)
{
    if (index < 0 || index >= presets.size())
        return;

    currentPresetIndex = index;
    const auto& preset = presets[index];

    if (auto* param = apvts.getParameter("gash"))
        param->setValueNotifyingHost(preset.gash);
    if (auto* param = apvts.getParameter("blood"))
        param->setValueNotifyingHost(preset.blood);
    if (auto* param = apvts.getParameter("sharpen"))
        param->setValueNotifyingHost(preset.sharpen);
    if (auto* param = apvts.getParameter("recoil"))
        param->setValueNotifyingHost(preset.recoil);
    if (auto* param = apvts.getParameter("mixtape"))
        param->setValueNotifyingHost(preset.mixtape);
}
