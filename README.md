# Speech Recorder 🎤

**Professional speech-to-text application for Linux**

A modern, feature-rich desktop application that records audio and transcribes it to text using state-of-the-art speech recognition models (Whisper & Vosk).

Made with ♥ by [SparklyLabz](https://sparklylabz.com)

---

## Features ✨

- **🎙️ High-Quality Audio Recording** - Uses PulseAudio for reliable audio capture
- **🤖 Multiple AI Models** - Support for Whisper (accurate) and Vosk (fast)
- **💾 Export Options** - Save as TXT, DOCX, or PDF
- **⚡ Real-Time Feedback** - Live audio level meter and recording timer
- **🎨 Modern UI** - Clean, professional dark theme
- **📦 Model Manager** - Download and manage models directly from the app
- **⚙️ Customizable** - Extensive settings for audio, models, and interface

---

## Quick Start 🚀

### 1. Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt install build-essential cmake git wget \
                 qtbase5-dev libpulse-dev libqt5svg5-dev
```

**Fedora:**
```bash
sudo dnf install gcc-c++ cmake git wget \
                 qt5-qtbase-devel pulseaudio-libs-devel
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake git wget qt5-base libpulse
```

### 2. Build

```bash
cd speech-recorder
./build.sh
```

The script will:
- Download whisper.cpp automatically
- Download the Whisper Base model (142 MB)
- Build the application

### 3. Run

```bash
./build/speech-recorder
```

---

## Usage Guide 📖

### Basic Recording

1. **Click RECORD** - Start recording your voice
2. **Speak clearly** - Watch the audio level meter
3. **Click STOP** - Transcription begins automatically
4. **Export or Copy** - Save your transcription

### Selecting Models

Use the model dropdown to switch between:
- **Whisper Tiny** (75 MB) - Fast, decent accuracy
- **Whisper Base** (142 MB) - Balanced (default)
- **Whisper Small** (466 MB) - Best accuracy
- **Vosk Small** (40 MB) - Very fast
- **Vosk Large** (1.8 GB) - Vosk with best accuracy

### Downloading Models

1. Go to **Tools → Manage Models**
2. Click **Download** next to any model
3. Wait for download to complete
4. Model is immediately available

### Exporting

- **Save TXT** - Plain text file
- **Export DOCX** - Word-compatible document (RTF format)
- **Export PDF** - Professional PDF with formatting

---

## System Requirements 💻

### Minimum
- **OS:** Linux (Kernel 4.0+)
- **RAM:** 4 GB
- **Disk:** 500 MB (+ models)
- **Audio:** PulseAudio

### Recommended
- **RAM:** 8 GB
- **CPU:** 4+ cores
- **Disk:** 2 GB for multiple models

---

## Troubleshooting 🔧

### No audio recorded

**Check if PulseAudio is running:**
```bash
pactl info
```

**List audio sources:**
```bash
pactl list sources short
```

**Test recording:**
```bash
parec --channels=1 --rate=16000 test.wav
```

### Model not loading

**Check model directory:**
```bash
ls ~/.local/share/speech-recorder/models/
```

**Re-download from Model Manager** if corrupted.

### Build fails

**Make sure all dependencies are installed:**
```bash
pkg-config --modversion Qt5Core
pkg-config --exists libpulse-simple && echo "PulseAudio OK"
```

**Clean build:**
```bash
rm -rf build external
./build.sh
```

---

## Advanced Configuration ⚙️

### Settings Location
`~/.config/SparklyLabz/SpeechRecorder.conf`

### Model Directory
`~/.local/share/speech-recorder/models/`

### Custom Model Paths

Edit settings or use **Tools → Settings** to change model directory.

### Enable Vosk Support

Install libvosk:
```bash
# Ubuntu/Debian
sudo apt install libvosk-dev

# Or build from source:
git clone https://github.com/alphacep/vosk-api
cd vosk-api/src
make
sudo make install
```

Rebuild:
```bash
rm -rf build
./build.sh
```

---

## Architecture 🏗️

```
speech-recorder/
├── src/
│   ├── main.cpp              # Entry point
│   ├── MainWindow.*          # Main UI
│   ├── AudioRecorder.*       # PulseAudio recording
│   ├── WhisperTranscriber.*  # Whisper integration
│   ├── TranscriptionWorker.* # Async threading
│   ├── gui/
│   │   ├── ModelSelector.*   # Model dropdown
│   │   ├── ModelManager.*    # Download dialog
│   │   └── SettingsDialog.*  # Settings UI
│   ├── transcription/
│   │   └── VoskEngine.*      # Vosk integration
│   └── utils/
│       ├── FileExporter.*    # Export functionality
│       └── Settings.*        # Config management
├── resources/
│   ├── icons/                # SVG icons
│   ├── styles/               # QSS themes
│   └── resources.qrc         # Qt resources
├── external/
│   └── whisper.cpp/          # Git submodule (auto-downloaded)
└── models/                   # Downloaded models
```

---

## Contributing 🤝

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### TODO List
- [ ] Real-time streaming transcription
- [ ] Multiple language support UI
- [ ] Custom model training integration
- [ ] Windows/macOS ports
- [ ] Plugins system

---

## License 📄

This project is licensed under the MIT License - see [LICENSE](LICENSE) file.

---

## Credits 👏

- **whisper.cpp** by Georgi Gerganov - https://github.com/ggerganov/whisper.cpp
- **Vosk** by Alpha Cephei - https://alphacephei.com/vosk/
- **Qt Framework** - https://www.qt.io/
- **PulseAudio** - https://www.freedesktop.org/wiki/Software/PulseAudio/

---

## Links 🔗

- **Website:** https://sparklylabz.com
- **Documentation:** [BUILDING.md](BUILDING.md) | [MODELS.md](MODELS.md)
- **Report Issues:** GitHub Issues
- **Support:** support@sparklylabz.com

---

**Made with ♥ by SparklyLabz**

*If you find this useful, consider starring the repo and sharing it!*
