# Download & Installation Instructions

Choose your platform below:

---

## 🐧 Linux (Ubuntu/Debian)

### Method 1: Install Package (Easiest)

1. **Download** the `.deb` file:
   - Go to [Releases](https://github.com/YOUR_USERNAME/speech-recorder/releases)
   - Download `speech-recorder_1.0.0_amd64.deb`

2. **Install** by double-clicking the file, or:
   ```bash
   sudo apt install ./speech-recorder_1.0.0_amd64.deb
   ```

3. **Run** from your app menu or terminal:
   ```bash
   speech-recorder
   ```

### Method 2: Build from Source

1. **Download** the source code:
   ```bash
   git clone https://github.com/YOUR_USERNAME/speech-recorder
   cd speech-recorder
   ```

2. **Build** (auto-installs dependencies):
   ```bash
   ./build.sh
   ```

3. **Run**:
   ```bash
   ./build/speech-recorder
   ```

Or use the easy launcher:
```bash
./run.sh
```

---

## 🪟 Windows

### ⚠️ Windows Support Coming in v1.1.0

Windows native support is under development. For now:

### Option 1: Use WSL2 (Recommended - Works Now!)

1. **Install WSL2**:
   - Open PowerShell as Administrator
   - Run: `wsl --install`
   - Restart computer

2. **Install Ubuntu** from Microsoft Store

3. **Open Ubuntu** terminal and run:
   ```bash
   git clone https://github.com/YOUR_USERNAME/speech-recorder
   cd speech-recorder
   ./build.sh
   ./build/speech-recorder
   ```

Works perfectly! Linux environment in Windows.

### Option 2: Wait for Native Windows Release

We're working on:
- ✅ Native Windows audio (PortAudio)
- ✅ Windows installer (.exe)
- ✅ One-click setup
- ✅ No WSL needed

**Expected:** Q1 2025

---

## 🍎 macOS

### macOS Support Planned for v1.2.0

Similar to Windows, macOS requires:
- Native audio (PortAudio/CoreAudio)
- macOS app bundle (.app)
- Homebrew installation

For now, macOS users can:
1. Build from source (requires Xcode)
2. Wait for v1.2.0 release

---

## 📦 What You Get

After installation:

✅ **Full-featured speech-to-text app**  
✅ **11 Whisper models** (download via GUI)  
✅ **Export to TXT/DOCX/PDF**  
✅ **Professional dark theme UI**  
✅ **Model download manager**  
✅ **Settings & customization**  

---

## 🚀 First-Time Setup

1. **Launch** the app
2. **Download a model**:
   - Tools → Manage Models
   - Click "Download" next to Whisper Base (142 MB)
   - Wait for download to complete
3. **Start recording!**
   - Click the big red RECORD button
   - Speak clearly
   - Click STOP
   - Your text appears!

---

## 💾 System Requirements

### Linux
- Ubuntu 20.04+ / Debian 11+
- 4 GB RAM (2 GB for Tiny model)
- 500 MB disk space (+ models)
- PulseAudio

### Windows (WSL)
- Windows 10 version 2004+ or Windows 11
- WSL2 enabled
- 4 GB RAM
- 500 MB disk space

---

## 🐛 Troubleshooting

### Linux: "PulseAudio not found"
```bash
sudo apt install pulseaudio
pulseaudio --start
```

### Linux: "Qt5 not found"
```bash
sudo apt install qtbase5-dev
```

### WSL: "No audio"
```bash
# PulseAudio works in WSL2
pactl list sources
```

---

## 📖 More Documentation

- **README.md** - Full user guide
- **BUILDING.md** - Developer build instructions
- **MODELS.md** - Model comparison table
- **WINDOWS_BUILD.md** - Windows port status

---

## 🆘 Need Help?

- **Issues:** https://github.com/YOUR_USERNAME/speech-recorder/issues
- **Website:** https://sparklylabz.com
- **Email:** support@sparklylabz.com

---

## 🎉 Thank You!

Thanks for using Speech Recorder!

**Made with ♥ by SparklyLabz**

*If you find this useful, please star the repo!* ⭐

