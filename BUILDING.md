# Building Speech Recorder

Detailed build instructions for developers and packagers.

---

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Build Methods](#build-methods)
3. [Build Options](#build-options)
4. [Platform-Specific Notes](#platform-specific-notes)
5. [Troubleshooting](#troubleshooting)

---

## Prerequisites

### Required Dependencies

| Package | Version | Purpose |
|---------|---------|---------|
| CMake | 3.16+ | Build system |
| Qt5 | 5.12+ | GUI framework |
| PulseAudio | 10.0+ | Audio capture |
| C++ Compiler | C++17 | Compilation |

### Optional Dependencies

| Package | Version | Purpose |
|---------|---------|---------|
| libvosk | 0.3.45+ | Vosk transcription |
| unzip | any | Extract model archives |

### Development Tools

```bash
# Ubuntu/Debian
sudo apt install \
  build-essential \
  cmake \
  git \
  qtbase5-dev \
  qtbase5-dev-tools \
  libqt5svg5-dev \
  libpulse-dev \
  libvosk-dev

# Fedora
sudo dnf install \
  gcc-c++ \
  cmake \
  git \
  qt5-qtbase-devel \
  qt5-qtsvg-devel \
  pulseaudio-libs-devel

# Arch Linux
sudo pacman -S \
  base-devel \
  cmake \
  git \
  qt5-base \
  qt5-svg \
  libpulse
```

---

## Build Methods

### Method 1: Quick Build (Recommended)

Use the provided build script:

```bash
./build.sh
```

This automatically:
- Checks dependencies
- Clones whisper.cpp
- Downloads Whisper Base model
- Builds the application

### Method 2: Manual CMake

```bash
# 1. Clone whisper.cpp
mkdir -p external
cd external
git clone https://github.com/ggerganov/whisper.cpp.git
cd ..

# 2. Download a model
mkdir -p models
wget https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.bin \
     -O models/ggml-base.bin

# 3. Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
cd ..
```

### Method 3: Out-of-Source Build

```bash
git clone https://github.com/your-repo/speech-recorder.git
mkdir speech-recorder-build
cd speech-recorder-build
cmake ../speech-recorder -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

---

## Build Options

### CMake Options

```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \        # Release, Debug, RelWithDebInfo
  -DCMAKE_INSTALL_PREFIX=/usr/local \ # Install location
  -DVOSK_ENABLED=ON                   # Enable Vosk support (auto-detected)
```

### Build Types

| Type | Optimization | Debug Info | Use Case |
|------|--------------|------------|----------|
| Release | O3 | No | Production |
| Debug | O0 | Full | Development |
| RelWithDebInfo | O2 | Yes | Profiling |

### Environment Variables

```bash
# Use specific Qt installation
export CMAKE_PREFIX_PATH=/opt/Qt/5.15.2/gcc_64

# Use custom compiler
export CXX=clang++

# Build with ccache
export CMAKE_CXX_COMPILER_LAUNCHER=ccache
```

---

## Platform-Specific Notes

### Ubuntu 20.04 LTS

Qt 5.12 is default. Everything works out of the box:

```bash
sudo apt install qtbase5-dev libpulse-dev
./build.sh
```

### Ubuntu 22.04 LTS

Qt 5.15 is default. Recommended for best compatibility:

```bash
sudo apt install qtbase5-dev qtbase5-dev-tools libqt5svg5-dev libpulse-dev
./build.sh
```

### Fedora 38+

```bash
sudo dnf install qt5-qtbase-devel pulseaudio-libs-devel
./build.sh
```

### Arch Linux

```bash
sudo pacman -S qt5-base libpulse
./build.sh
```

### NixOS

Create a `shell.nix`:

```nix
{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    qt5.qtbase
    qt5.qtsvg
    pulseaudio
    pkg-config
  ];
}
```

Then build:
```bash
nix-shell
./build.sh
```

---

## Installation

### User Installation (No Root)

```bash
./build/speech-recorder
```

Copy to user binaries:
```bash
mkdir -p ~/.local/bin
cp build/speech-recorder ~/.local/bin/
```

### System-Wide Installation

```bash
cd build
sudo make install
```

Installs to:
- Binary: `/usr/local/bin/speech-recorder`
- Desktop: `/usr/local/share/applications/speech-recorder.desktop`
- Icon: `/usr/local/share/icons/hicolor/256x256/apps/speech-recorder.svg`

### Uninstall

```bash
cd build
sudo make uninstall
```

---

## Packaging

### Debian/Ubuntu Package

```bash
cd packaging/debian
./build_deb.sh
```

Generates: `speech-recorder_1.0.0_amd64.deb`

### AppImage

```bash
# Install linuxdeploy
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-x86_64.AppImage

# Build AppImage
./build.sh
./linuxdeploy-x86_64.AppImage \
  --appdir AppDir \
  --executable build/speech-recorder \
  --desktop-file packaging/speech-recorder.desktop \
  --icon-file resources/icons/app_icon.svg \
  --output appimage
```

### Flatpak

See `packaging/flatpak/com.sparklylabz.SpeechRecorder.yml`

---

## Cross-Compilation

### For ARM64 (Raspberry Pi 4)

```bash
# Install cross-compiler
sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu

# Create toolchain file
cat > toolchain-arm64.cmake << EOF
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
EOF

# Build
mkdir build-arm64 && cd build-arm64
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm64.cmake
make -j$(nproc)
```

---

## Troubleshooting

### CMake Can't Find Qt5

**Solution 1:** Install Qt5 development packages
```bash
sudo apt install qtbase5-dev
```

**Solution 2:** Set Qt path manually
```bash
export CMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt5
cmake ..
```

### Linker Error: undefined reference to whisper_*

**Cause:** whisper.cpp not built

**Solution:**
```bash
cd external/whisper.cpp
mkdir build && cd build
cmake ..
make -j$(nproc)
cd ../../..
rm -rf build
mkdir build && cd build
cmake ..
make
```

### MOC Error: Parse error at "SIGNAL"

**Cause:** Old Qt version or missing Q_OBJECT macro

**Solution:**
```bash
# Update Qt
sudo apt update && sudo apt upgrade qtbase5-dev

# Clean build
rm -rf build CMakeCache.txt
mkdir build && cd build
cmake ..
make
```

### PulseAudio Not Found

**Check installation:**
```bash
pkg-config --modversion libpulse-simple
```

**Install if missing:**
```bash
sudo apt install libpulse-dev
```

### Vosk Not Detected

Vosk is optional. To enable:

```bash
# Install from source
git clone https://github.com/alphacep/vosk-api
cd vosk-api/src
make
sudo make install
sudo ldconfig

# Rebuild
cd /path/to/speech-recorder
rm -rf build
./build.sh
```

---

## Performance Tips

### Use Ninja Instead of Make

```bash
sudo apt install ninja-build
cmake .. -G Ninja
ninja
```

~30% faster builds on multi-core systems.

### Use ccache

```bash
sudo apt install ccache
export CMAKE_CXX_COMPILER_LAUNCHER=ccache
cmake ..
make -j$(nproc)
```

Subsequent builds are much faster.

### Link-Time Optimization (LTO)

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
make -j$(nproc)
```

Smaller binary (~10% reduction), slightly longer build time.

---

## Building Documentation

### Doxygen API Docs

```bash
sudo apt install doxygen graphviz
doxygen Doxyfile
firefox docs/html/index.html
```

---

## Development Workflow

### Setup Development Environment

```bash
# Clone repo
git clone https://your-repo/speech-recorder.git
cd speech-recorder

# Build debug version
mkdir build-debug
cd build-debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)

# Run with debugger
gdb ./speech-recorder
```

### Code Formatting

```bash
# Install clang-format
sudo apt install clang-format

# Format all code
find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

### Static Analysis

```bash
# Install cppcheck
sudo apt install cppcheck

# Run analysis
cppcheck --enable=all src/
```

---

## Contact

Build issues? Email: dev@sparklylabz.com

Or open an issue on GitHub.

---

**Made with ♥ by SparklyLabz**

