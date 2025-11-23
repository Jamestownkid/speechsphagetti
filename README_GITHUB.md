# 🎤 Speech Recorder

**Professional speech-to-text application for Linux**

Transform your voice into text using state-of-the-art AI models. Perfect for transcription, dictation, note-taking, and accessibility.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Linux-blue.svg)](https://www.linux.org/)
[![Qt](https://img.shields.io/badge/Qt-5.12%2B-green.svg)](https://www.qt.io/)

---

## ✨ Features

- 🎙️ **High-Quality Recording** - Uses PulseAudio for reliable audio capture
- 🤖 **Multiple AI Models** - 11 Whisper models + 2 Vosk models
- 💾 **Export Options** - Save as TXT, DOCX, or PDF
- ⚡ **Real-Time Feedback** - Live audio meter and recording timer
- 🎨 **Modern UI** - Professional dark theme interface
- 📦 **Model Manager** - Download and manage models from GUI
- ⚙️ **Customizable** - Extensive settings for audio, models, and interface
- 🌍 **99 Languages** - Via Whisper multilingual models

---

## 📥 Download

### Linux (Ubuntu/Debian)

**[Download .deb Package](https://github.com/YOUR_USERNAME/speech-recorder/releases/latest)** ⬅ Click here!

Or build from source:
```bash
git clone https://github.com/YOUR_USERNAME/speech-recorder
cd speech-recorder
./build.sh
./build/speech-recorder
```

### Windows

🔨 **Windows support coming in v1.1.0!**

For now, use **WSL2** (Windows Subsystem for Linux):
1. Install WSL: `wsl --install`
2. Install Ubuntu from Microsoft Store
3. Follow Linux instructions above

[See full instructions →](DOWNLOAD_INSTRUCTIONS.md)

---

## 🚀 Quick Start

1. **Install** the .deb package
2. **Launch** from app menu or run `speech-recorder`
3. **Download a model** (Tools → Manage Models)
4. **Click RECORD** and start speaking
5. **Click STOP** - your text appears!
6. **Export** to TXT, DOCX, or PDF

---

## 📊 Model Comparison

| Model | Size | Speed | Accuracy | Best For |
|-------|------|-------|----------|----------|
| **Whisper Tiny** | 75 MB | ⚡ Fastest | Good | Quick notes, low CPU |
| **Whisper Base** ⭐ | 142 MB | ⚡ Fast | Great | General use (recommended) |
| **Whisper Small** | 466 MB | 🟡 Medium | Excellent | Long transcriptions |
| **Whisper Medium** | 1.5 GB | 🔴 Slow | Outstanding | High accuracy needs |
| **Whisper Large V3** | 2.9 GB | 🔴 Very Slow | Best | Maximum quality |

[See full model comparison →](MODELS.md)

---

## 🖼️ Screenshots

### Main Interface
```
┌─────────────────────────────────────────┐
│  Model: Whisper Base ✓      00:00      │
├─────────────────────────────────────────┤
│  ▓▓▓▓▓░░░░░░░░░░░░░░░░  [Audio Level]  │
│                                         │
│        ╔════════════════════╗          │
│        ║   ⬤  RECORD        ║          │
│        ╚════════════════════╝          │
│                                         │
│  Status: Ready                          │
├─────────────────────────────────────────┤
│  [Your transcribed text appears here]  │
│                                         │
├─────────────────────────────────────────┤
│  [Clear] [Copy] [TXT] [DOCX] [PDF]     │
└─────────────────────────────────────────┘
```

---

## 🛠️ System Requirements

### Minimum
- **OS:** Ubuntu 20.04+ / Debian 11+ / Fedora 36+
- **RAM:** 4 GB (2 GB for Tiny model)
- **Disk:** 500 MB + models
- **Audio:** PulseAudio

### Recommended
- **RAM:** 8 GB
- **CPU:** 4+ cores
- **Disk:** 2 GB for multiple models

---

## 📖 Documentation

- **[Download Instructions](DOWNLOAD_INSTRUCTIONS.md)** - Installation guide
- **[Building from Source](BUILDING.md)** - Developer guide
- **[Model Guide](MODELS.md)** - Model comparison & recommendations
- **[Windows Support](WINDOWS_BUILD.md)** - Status & roadmap
- **[Contributing](CONTRIBUTING.md)** - How to contribute

---

## 🗺️ Roadmap

### v1.0.0 (Current) ✅
- ✅ Linux support (Ubuntu, Debian, Fedora, Arch)
- ✅ 11 Whisper models + 2 Vosk models
- ✅ Export to TXT/DOCX/PDF
- ✅ Model download manager
- ✅ Professional UI

### v1.1.0 (Q1 2025)
- 🔨 Windows native support (PortAudio)
- 🔨 GPU acceleration (CUDA/ROCm)
- 🔨 Real-time streaming transcription
- 🔨 Multiple language UI

### v1.2.0 (Q2 2025)
- 📅 macOS support
- 📅 Flatpak/Snap packages
- 📅 Plugin system

---

## 🤝 Contributing

We welcome contributions! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

**Areas we need help:**
- Windows PortAudio integration
- GPU acceleration
- Translations
- Testing on different distros

---

## 📄 License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for details.

### Third-Party Licenses
- **whisper.cpp** by Georgi Gerganov (MIT)
- **Vosk** by Alpha Cephei (Apache 2.0)
- **Qt Framework** (LGPL v3)
- **PulseAudio** (LGPL v2.1+)

---

## 🙏 Credits

**Developed by:** [SparklyLabz](https://sparklylabz.com)

**Built with:**
- [whisper.cpp](https://github.com/ggerganov/whisper.cpp) - Whisper model inference
- [Vosk](https://alphacephei.com/vosk/) - Fast speech recognition
- [Qt5](https://www.qt.io/) - Cross-platform GUI framework
- [PulseAudio](https://www.freedesktop.org/wiki/Software/PulseAudio/) - Audio system

---

## 💬 Support

- **Issues:** [GitHub Issues](https://github.com/YOUR_USERNAME/speech-recorder/issues)
- **Website:** https://sparklylabz.com
- **Email:** support@sparklylabz.com

---

## ⭐ Star This Project!

If you find Speech Recorder useful, please star the repository!

---

**Made with ♥ by SparklyLabz**

*Transform your voice into text, effortlessly.*

