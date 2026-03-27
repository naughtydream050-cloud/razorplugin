# Welcome to Razor 🎙️

**Krump Vocal Destruction Plugin** — A JUCE-based VST3/AU effect plugin designed for brutally crushing and shaping Krump vocal performances.

---

## Features

### DSP Chain (5 Parameters)

| Knob | Effect | Description |
|------|--------|-------------|
| **GASH** | Distortion | Extreme saturation — brings out the guttural throat sounds |
| **BLOOD** | Compressor | Ultra-fast attack compressor to push breath and tension forward |
| **SHARPEN** | Exciter | High-frequency harmonic exciter to cut through any mix |
| **RECOIL** | Metallic Reverb | Short, metallic reverb simulating a claustrophobic steel space |
| **MIXTAPE** | Lo-Fi | Cassette hiss + bit-rate degradation for gritty texture |

### Presets

1. **Devil chorus** — Default balanced Krump tone
2. **SAVAGE Hell** — Maximum aggression, heavy saturation + exciter
3. **Dark Raw** — Darker, more compressed tone
4. **Solid Buck** — Tight, punchy, high-compression focus

---

## Building

### Prerequisites

- CMake 3.22+
- JUCE 7.0.12 (fetched automatically via CMake FetchContent)
- C++17 compiler

### Windows (MSVC)

```bash
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target Razor_VST3
```

### macOS (Universal Binary — Intel + Apple Silicon)

```bash
cmake -B build -G Xcode \
  -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
  -DCMAKE_OSX_DEPLOYMENT_TARGET="10.15"
cmake --build build --config Release --target Razor_VST3
cmake --build build --config Release --target Razor_AU
```

### Linux (Ninja)

```bash
sudo apt-get install libasound2-dev libfreetype6-dev libx11-dev \
  libxcomposite-dev libxcursor-dev libxext-dev libxinerama-dev \
  libxrandr-dev libxrender-dev libwebkit2gtk-4.0-dev
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --target Razor_VST3
```

---

## Installation

| OS | Format | Path |
|----|--------|------|
| Windows | VST3 | `C:\Program Files\Common Files\VST3\` |
| macOS | VST3 | `/Library/Audio/Plug-Ins/VST3/` |
| macOS | AU | `/Library/Audio/Plug-Ins/Components/` |
| Linux | VST3 | `~/.vst3/` or `/usr/lib/vst3/` |

---

## CI/CD

GitHub Actions automatically builds on every push to `main`/`master`:

- **Windows** → VST3 (x64)
- **macOS** → VST3 + AU (Universal Binary)
- **Linux** → VST3 (x64)

Artifacts are uploaded to the Actions run. Tagged releases create a full GitHub Release with downloadable zip files.

---

## Visual Design

- **Color palette**: Pitch black (`#0A0A0A`) + Blood Red (`#CC0000`)
- **Title**: Glowing "WELCOME TO RAZOR" with blood drip effects
- **Knobs**: Razor blade / gear-tooth motif with metallic sheen and blood splatter
- **Left panel**: Devil illustration + KRUMP graffiti
- **LCD panel**: Backlit green display showing active preset name
- **Preset list**: Visible in upper right with current preset highlighted

---

## License

Copyright © 2024 RazorAudio. All rights reserved.
