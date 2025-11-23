# 🎤 Speech Recorder

**AI-Powered Speech-to-Text Recorder with Whisper**

Transform your voice into text with the power of OpenAI's Whisper AI - completely offline and free!

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey.svg)]()
[![Made with Qt](https://img.shields.io/badge/made%20with-Qt5-green.svg)](https://www.qt.io/)

---

## 🚀 One-Click Installation

### 🐧 Linux Users

```bash
# Just double-click this file after unzipping:
CLICK_ME_IF_YOUR_ON_LINUX.sh
```

**That's it!** The app will appear in your Applications menu.

### 🪟 Windows Users

```batch
# Just double-click this file after unzipping:
CLICK_ME_IF_YOUR_ON_WINDOWS.bat
```

**That's it!** A desktop shortcut will be created.

---

## ✨ Features

- 🎙️ **Record audio** from your microphone with one click
- 🤖 **AI transcription** using OpenAI's Whisper (100% offline)
- 📝 **Export transcripts** to TXT, DOCX, or PDF
- 📋 **Copy to clipboard** instantly
- 🗂️ **11 Whisper models** - Choose from Tiny (75MB) to Large V3 (2.9GB)
- 💾 **Model manager** - Download and remove models to save space
- 🎨 **Modern Qt5 GUI** - Beautiful and easy to use
- 🔒 **100% offline** - No internet required after installation
- ⚡ **Fast and lightweight** - Runs on any modern PC
- 🆓 **Free and open source** - MIT License

---

## 📸 Screenshots

### Main Window
*Record audio and see live transcription*

### Model Manager
*Download and manage AI models*

---

## 🎯 Use Cases

- 📚 **Students**: Record lectures and get instant transcripts
- ✍️ **Writers**: Dictate your ideas instead of typing
- 💼 **Professionals**: Transcribe meetings and interviews
- 🎙️ **Podcasters**: Generate show notes automatically
- ♿ **Accessibility**: Voice-to-text for those who can't type
- 🌍 **Language learners**: Practice pronunciation with visual feedback

---

## 🔧 Technical Details

### Built With

- **Language**: C++ (C++17)
- **Framework**: Qt5
- **Audio**: PulseAudio (Linux), WSL2 (Windows)
- **AI Engine**: [Whisper.cpp](https://github.com/ggerganov/whisper.cpp)
- **Build System**: CMake
- **Models**: OpenAI Whisper (GGML format)

### System Requirements

**Linux:**
- Ubuntu 20.04+ / Debian 11+ / Fedora 35+
- Qt5 5.12 or later
- PulseAudio
- 1 GB RAM minimum (4 GB recommended)
- 200 MB - 3 GB disk space (depending on models)

**Windows:**
- Windows 10 version 2004+ (Build 19041+)
- WSL2 (installed automatically by our script)
- 2 GB RAM minimum (4 GB recommended)
- 200 MB - 3 GB disk space (depending on models)

---

## 📦 Available Models

| Model | Size | Speed | Accuracy | Best For |
|-------|------|-------|----------|----------|
| **Tiny** | 75 MB | 🟢 Fastest | Low | Real-time dictation |
| **Tiny.en** | 75 MB | 🟢 Very Fast | Medium | English-only, better than Tiny |
| **Base** | 142 MB | 🟢 Fast | Medium | ⭐ General use (recommended) |
| **Base.en** | 142 MB | 🟢 Fast | Good | English-only, better accuracy |
| **Small** | 466 MB | 🟡 Medium | High | Long transcriptions |
| **Small.en** | 466 MB | 🟡 Medium | Very High | English-only, best balance |
| **Medium** | 1.5 GB | 🔴 Slow | Very High | Professional work |
| **Medium.en** | 1.5 GB | 🔴 Slow | Ultra High | Best offline English |
| **Large V1** | 2.9 GB | 🔴 Very Slow | Ultra High | Multilingual accuracy |
| **Large V2** | 2.9 GB | 🔴 Very Slow | Ultra High | Best multilingual quality |
| **Large V3** | 2.9 GB | 🔴 Very Slow | Ultra High | Latest, best accuracy |

💡 **Tip**: Start with **Base** (142 MB) - it's the sweet spot for most users!

---

## 🎓 How It Works

1. **Click Record** → App starts recording your microphone
2. **Speak clearly** → Your voice is captured as audio
3. **Click Stop** → Recording stops
4. **AI transcribes** → Whisper converts speech to text (happens locally on your PC)
5. **Edit & Export** → Copy, edit, or save your transcript

**Everything runs on your computer** - no cloud, no internet, no data sent anywhere!

---

## 🛠️ Manual Build (Advanced)

If you want to build from source:

```bash
# Install dependencies
sudo apt install build-essential cmake git wget \
  qtbase5-dev qtbase5-dev-tools qttools5-dev-tools \
  libqt5svg5-dev libpulse-dev pkg-config

# Clone the repository
git clone https://github.com/Jamestownkid/speechsphagetti.git
cd speechsphagetti

# Build
./build.sh

# Run
./build/speech-recorder
```

See [BUILDING.md](BUILDING.md) for detailed build instructions.

---

## 📖 Documentation

- [**START_HERE.txt**](START_HERE.txt) - Quick start guide
- [**BUILDING.md**](BUILDING.md) - Build from source
- [**MODEL_MANAGEMENT_GUIDE.md**](MODEL_MANAGEMENT_GUIDE.md) - Managing AI models
- [**HOW_USERS_INSTALL_THIS.md**](HOW_USERS_INSTALL_THIS.md) - Installation walkthrough
- [**CHANGELOG.md**](CHANGELOG.md) - Version history

---

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

- 🐛 **Report bugs** - Open an issue
- 💡 **Suggest features** - Open an issue
- 🔧 **Submit PRs** - Fix bugs or add features
- 📖 **Improve docs** - Better documentation helps everyone
- ⭐ **Star this repo** - Show your support!

---

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Third-Party Licenses

- **Whisper.cpp**: MIT License
- **Qt5**: LGPL v3
- **Whisper models**: MIT License (OpenAI)

---

## 🌟 Acknowledgments

- **OpenAI** - For creating Whisper
- **Georgi Gerganov** - For [whisper.cpp](https://github.com/ggerganov/whisper.cpp)
- **Qt Project** - For the amazing Qt framework
- **All contributors** - Thank you!

---

## 📞 Support

- 🐛 **Bug reports**: [Open an issue](https://github.com/Jamestownkid/speechsphagetti/issues)
- 💬 **Questions**: [Discussions](https://github.com/Jamestownkid/speechsphagetti/discussions)
- 📧 **Email**: jamestownkid@users.noreply.github.com

---

## 🎉 Quick Links

- [**Download Latest Release**](https://github.com/Jamestownkid/speechsphagetti/releases)
- [**View Source Code**](https://github.com/Jamestownkid/speechsphagetti)
- [**Report a Bug**](https://github.com/Jamestownkid/speechsphagetti/issues)
- [**Request a Feature**](https://github.com/Jamestownkid/speechsphagetti/issues)

---

<div align="center">

**Made with ♥ by [SparklyLabz](https://github.com/Jamestownkid)**

If you find this useful, consider giving it a ⭐!

[⬆ Back to Top](#-speech-recorder)

</div>
