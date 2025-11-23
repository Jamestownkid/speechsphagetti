#!/bin/bash
# Build script for Speech Recorder
# Made by SparklyLabz - https://sparklylabz.com

set -e

echo "╔══════════════════════════════════════════╗"
echo "║   Speech Recorder Build Script v1.0     ║"
echo "║        Made by SparklyLabz              ║"
echo "╚══════════════════════════════════════════╝"
echo ""

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Functions
print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# 1. Check dependencies
print_info "Checking dependencies..."

MISSING_DEPS=()

for cmd in cmake g++ git wget; do
    if ! command -v $cmd &> /dev/null; then
        MISSING_DEPS+=("$cmd")
    fi
done

if [ ${#MISSING_DEPS[@]} -ne 0 ]; then
    print_error "Missing dependencies: ${MISSING_DEPS[*]}"
    echo ""
    echo "Install with:"
    echo "  Ubuntu/Debian: sudo apt install build-essential cmake git wget"
    echo "  Fedora:        sudo dnf install gcc-c++ cmake git wget"
    echo "  Arch:          sudo pacman -S base-devel cmake git wget"
    exit 1
fi

# Check for Qt5
if ! pkg-config --exists Qt5Core; then
    print_error "Qt5 not found"
    echo ""
    echo "Install with:"
    echo "  Ubuntu/Debian: sudo apt install qtbase5-dev qtbase5-dev-tools libqt5svg5-dev"
    echo "  Fedora:        sudo dnf install qt5-qtbase-devel"
    echo "  Arch:          sudo pacman -S qt5-base"
    exit 1
fi

print_success "Qt5 $(pkg-config --modversion Qt5Core) found"

# Check for PulseAudio
if ! pkg-config --exists libpulse-simple; then
    print_error "PulseAudio development files not found"
    echo ""
    echo "Install with:"
    echo "  Ubuntu/Debian: sudo apt install libpulse-dev"
    echo "  Fedora:        sudo dnf install pulseaudio-libs-devel"
    echo "  Arch:          sudo pacman -S libpulse"
    exit 1
fi

print_success "PulseAudio found"

# Check for Vosk (optional)
if pkg-config --exists vosk; then
    print_success "Vosk support available"
    VOSK_AVAILABLE=true
else
    print_warning "Vosk not found (optional). Vosk transcription will be disabled."
    print_info "To enable Vosk, install from: https://alphacephei.com/vosk/"
    VOSK_AVAILABLE=false
fi

# 2. Setup whisper.cpp
print_info "Setting up whisper.cpp..."

if [ ! -d "external/whisper.cpp" ]; then
    print_info "Cloning whisper.cpp..."
    mkdir -p external
    cd external
    git clone https://github.com/ggerganov/whisper.cpp.git
    cd ..
    print_success "whisper.cpp cloned"
else
    print_success "whisper.cpp already exists"
fi

# 3. Download default model
print_info "Checking for Whisper models..."

mkdir -p models

if [ ! -f "models/ggml-base.bin" ]; then
    print_info "Downloading Whisper Base model (142 MB)..."
    print_warning "This may take a few minutes on slow connections..."
    
    wget --progress=bar:force \
         -c https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.bin \
         -O models/ggml-base.bin.tmp
    
    mv models/ggml-base.bin.tmp models/ggml-base.bin
    print_success "Whisper Base model downloaded"
else
    print_success "Whisper Base model already exists"
fi

# 4. Create build directory
print_info "Configuring build..."

mkdir -p build
cd build

# 5. Run CMake
print_info "Running CMake..."

CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=Release"

if [ "$VOSK_AVAILABLE" = true ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DVOSK_ENABLED=ON"
fi

cmake .. $CMAKE_FLAGS

if [ $? -ne 0 ]; then
    print_error "CMake configuration failed"
    exit 1
fi

# 6. Build
print_info "Building Speech Recorder..."
print_info "Using $(nproc) CPU cores"

make -j$(nproc)

if [ $? -ne 0 ]; then
    print_error "Build failed"
    exit 1
fi

cd ..

# 7. Success message
echo ""
echo "╔══════════════════════════════════════════╗"
echo "║       Build Complete! 🎉                ║"
echo "╚══════════════════════════════════════════╝"
echo ""
print_success "Speech Recorder built successfully"
echo ""
echo "Run with:"
echo "  ${GREEN}./build/speech-recorder${NC}"
echo ""
echo "Install system-wide with:"
echo "  ${GREEN}sudo make -C build install${NC}"
echo ""
echo "Features enabled:"
echo "  • Whisper transcription: ✓"
if [ "$VOSK_AVAILABLE" = true ]; then
    echo "  • Vosk transcription: ✓"
else
    echo "  • Vosk transcription: ✗ (not installed)"
fi
echo ""
echo "Download more models from Tools > Manage Models"
echo ""
echo "Visit us at: ${BLUE}https://sparklylabz.com${NC}"
echo ""
