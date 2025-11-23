# Building for Windows

Speech Recorder can be built for Windows with some modifications.

## Current Status

**Linux:** ✅ Fully working  
**Windows:** 🔨 Requires audio system changes

## What Needs to Change

### 1. Audio Recording System

**Current:** Uses PulseAudio (Linux-only)  
**Solution:** Replace with PortAudio (cross-platform)

PortAudio works on:
- ✅ Linux (PulseAudio, ALSA, JACK)
- ✅ Windows (WASAPI, DirectSound)
- ✅ macOS (CoreAudio)

### 2. Build System

**Linux:** CMake + GCC  
**Windows:** CMake + MSVC or MinGW

## Quick Windows Build (Coming in v1.1)

For now, Windows users can:

### Option 1: WSL (Windows Subsystem for Linux)

```powershell
# Install WSL2 with Ubuntu
wsl --install

# Inside WSL
cd /mnt/c/Users/YOUR_USERNAME/Downloads
git clone https://github.com/YOUR_REPO/speech-recorder
cd speech-recorder
./build.sh
./build/speech-recorder
```

Works perfectly - uses Linux PulseAudio in WSL!

### Option 2: Full Windows Port (v1.1 - Coming Soon)

We're working on native Windows support with:
- PortAudio for recording
- Qt5 GUI (already cross-platform)
- whisper.cpp (already cross-platform)
- Windows installer (.exe)

## For Developers: Adding Windows Support

If you want to contribute Windows support:

### Step 1: Replace AudioRecorder with PortAudio

```cpp
// Replace PulseAudio calls with PortAudio
// src/AudioRecorder.cpp needs rewrite

#ifdef _WIN32
    // Windows WASAPI
#elif __linux__
    // PulseAudio
#elif __APPLE__
    // CoreAudio
#endif
```

### Step 2: Update CMakeLists.txt

```cmake
if(WIN32)
    find_package(portaudio REQUIRED)
    target_link_libraries(speech-recorder portaudio)
else()
    # PulseAudio on Linux
    pkg_check_modules(PULSEAUDIO REQUIRED libpulse-simple)
endif()
```

### Step 3: Build on Windows

```cmd
# Install Qt5, CMake, Visual Studio
# Install vcpkg for dependencies

git clone https://github.com/YOUR_REPO/speech-recorder
cd speech-recorder

# Build
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release

# Run
.\Release\speech-recorder.exe
```

### Step 4: Create Windows Installer

Use NSIS or Inno Setup to create installer.

## Roadmap

- **v1.0.0:** Linux only ✅
- **v1.1.0:** Cross-platform with PortAudio (planned)
- **v1.2.0:** Windows installer, AppImage for Linux

## Why Not Included Yet?

Adding Windows support properly requires:
1. Rewriting AudioRecorder.cpp (~200 lines)
2. Testing on Windows 10/11
3. Creating Windows installer
4. Handling Windows-specific quirks
5. Supporting MSVC and MinGW compilers

This is significant work - we wanted to release Linux version first!

## Help Wanted

If you're a Windows developer and want to help:
1. Fork the repo
2. Implement PortAudio support
3. Test on Windows
4. Submit pull request

See CONTRIBUTING.md for guidelines.

---

## Quick Comparison

| Feature | Linux | Windows | macOS |
|---------|-------|---------|-------|
| Audio Recording | ✅ PulseAudio | 🔨 Needs PortAudio | 🔨 Needs PortAudio |
| Qt5 GUI | ✅ | ✅ | ✅ |
| whisper.cpp | ✅ | ✅ | ✅ |
| Vosk | ✅ | ✅ | ✅ |
| Easy Install | ✅ .deb | 🔨 Need .exe | 🔨 Need .dmg |

## Temporary Windows Solution

For Windows users who want to use it NOW:

### Use WSL2 (Recommended)

1. Install WSL2: https://aka.ms/wsl2
2. Install Ubuntu from Microsoft Store
3. Open Ubuntu terminal
4. Follow Linux build instructions
5. Works perfectly!

---

**Made by SparklyLabz**

*Windows support is our #1 priority for v1.1.0!*

