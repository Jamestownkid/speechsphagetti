#!/bin/bash
# Debian package builder for Speech Recorder
# Made by SparklyLabz - https://sparklylabz.com

set -e

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo "╔══════════════════════════════════════════╗"
echo "║  Speech Recorder .deb Package Builder   ║"
echo "║        Made by SparklyLabz              ║"
echo "╚══════════════════════════════════════════╝"
echo ""

# Get version from CMakeLists.txt
VERSION=$(grep "project(SpeechRecorder VERSION" CMakeLists.txt | sed 's/.*VERSION \([0-9.]*\).*/\1/')
ARCH=$(dpkg --print-architecture)
PACKAGE_NAME="speech-recorder_${VERSION}_${ARCH}"

echo -e "${BLUE}[INFO]${NC} Building package: ${PACKAGE_NAME}"
echo ""

# 1. Check if build exists
if [ ! -f "build/speech-recorder" ]; then
    echo -e "${RED}[ERROR]${NC} Build not found. Run ./build.sh first!"
    exit 1
fi

echo -e "${BLUE}[INFO]${NC} Creating package directory structure..."

# 2. Create package directory
PKG_DIR="packaging/deb_build/${PACKAGE_NAME}"
rm -rf "${PKG_DIR}"
mkdir -p "${PKG_DIR}/DEBIAN"
mkdir -p "${PKG_DIR}/usr/bin"
mkdir -p "${PKG_DIR}/usr/share/applications"
mkdir -p "${PKG_DIR}/usr/share/icons/hicolor/256x256/apps"
mkdir -p "${PKG_DIR}/usr/share/doc/speech-recorder"

# 3. Create control file
echo -e "${BLUE}[INFO]${NC} Generating control file..."

cat > "${PKG_DIR}/DEBIAN/control" << EOF
Package: speech-recorder
Version: ${VERSION}
Section: sound
Priority: optional
Architecture: ${ARCH}
Depends: libqt5core5a (>= 5.12), libqt5widgets5 (>= 5.12), libqt5network5 (>= 5.12), libqt5printsupport5 (>= 5.12), libpulse0 (>= 10.0)
Recommends: libvosk0
Suggests: pulseaudio
Installed-Size: $(du -sk build/speech-recorder | cut -f1)
Maintainer: SparklyLabz <support@sparklylabz.com>
Homepage: https://sparklylabz.com
Description: Professional speech-to-text application
 Speech Recorder is a modern desktop application that records audio
 and transcribes it to text using state-of-the-art AI models.
 .
 Features:
  - Whisper and Vosk transcription engines
  - Multiple model support (Tiny, Base, Small, Large)
  - Export to TXT, DOCX, and PDF
  - Real-time audio level meter
  - Model download manager
  - Professional dark theme UI
 .
 Supports 99 languages via Whisper models.
EOF

# 4. Create postinst script (runs after installation)
echo -e "${BLUE}[INFO]${NC} Creating postinst script..."

cat > "${PKG_DIR}/DEBIAN/postinst" << 'EOF'
#!/bin/bash
set -e

# Create model directory
MODEL_DIR="$HOME/.local/share/speech-recorder/models"
mkdir -p "$MODEL_DIR"

# Check if Whisper Base model exists
if [ ! -f "$MODEL_DIR/ggml-base.bin" ]; then
    echo ""
    echo "╔═══════════════════════════════════════════════╗"
    echo "║  Speech Recorder installed successfully!     ║"
    echo "╚═══════════════════════════════════════════════╝"
    echo ""
    echo "IMPORTANT: Download a speech recognition model:"
    echo ""
    echo "Option 1 (Recommended): Use the GUI"
    echo "  1. Launch Speech Recorder"
    echo "  2. Go to Tools → Manage Models"
    echo "  3. Click 'Download' next to Whisper Base"
    echo ""
    echo "Option 2: Manual download (142 MB)"
    echo "  wget https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.bin \\"
    echo "       -O $MODEL_DIR/ggml-base.bin"
    echo ""
else
    echo "Speech Recorder installed successfully!"
    echo "Model already present: $MODEL_DIR/ggml-base.bin"
fi

# Update desktop database
if [ -x /usr/bin/update-desktop-database ]; then
    update-desktop-database -q /usr/share/applications || true
fi

# Update icon cache
if [ -x /usr/bin/gtk-update-icon-cache ]; then
    gtk-update-icon-cache -q -t -f /usr/share/icons/hicolor || true
fi

exit 0
EOF

chmod +x "${PKG_DIR}/DEBIAN/postinst"

# 5. Create prerm script (runs before removal)
echo -e "${BLUE}[INFO]${NC} Creating prerm script..."

cat > "${PKG_DIR}/DEBIAN/prerm" << 'EOF'
#!/bin/bash
set -e

# Nothing to do before removal
exit 0
EOF

chmod +x "${PKG_DIR}/DEBIAN/prerm"

# 6. Create postrm script (runs after removal)
echo -e "${BLUE}[INFO]${NC} Creating postrm script..."

cat > "${PKG_DIR}/DEBIAN/postrm" << 'EOF'
#!/bin/bash
set -e

if [ "$1" = "purge" ]; then
    # Only remove user data if purging
    echo "Removing user data (models and config)..."
    rm -rf "$HOME/.local/share/speech-recorder" || true
    rm -rf "$HOME/.config/SparklyLabz" || true
fi

# Update desktop database
if [ -x /usr/bin/update-desktop-database ]; then
    update-desktop-database -q /usr/share/applications || true
fi

# Update icon cache
if [ -x /usr/bin/gtk-update-icon-cache ]; then
    gtk-update-icon-cache -q -t -f /usr/share/icons/hicolor || true
fi

exit 0
EOF

chmod +x "${PKG_DIR}/DEBIAN/postrm"

# 7. Copy files
echo -e "${BLUE}[INFO]${NC} Copying application files..."

cp build/speech-recorder "${PKG_DIR}/usr/bin/"
cp packaging/speech-recorder.desktop "${PKG_DIR}/usr/share/applications/"
cp resources/icons/app_icon.svg "${PKG_DIR}/usr/share/icons/hicolor/256x256/apps/speech-recorder.svg"

# 8. Copy documentation
echo -e "${BLUE}[INFO]${NC} Copying documentation..."

cp README.md "${PKG_DIR}/usr/share/doc/speech-recorder/"
cp LICENSE "${PKG_DIR}/usr/share/doc/speech-recorder/"
cp CHANGELOG.md "${PKG_DIR}/usr/share/doc/speech-recorder/"

# Create copyright file
cat > "${PKG_DIR}/usr/share/doc/speech-recorder/copyright" << EOF
Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/
Upstream-Name: speech-recorder
Upstream-Contact: SparklyLabz <support@sparklylabz.com>
Source: https://sparklylabz.com

Files: *
Copyright: 2025 SparklyLabz
License: MIT
 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:
 .
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
 .
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
EOF

gzip -9 -n "${PKG_DIR}/usr/share/doc/speech-recorder/CHANGELOG.md"

# 9. Set permissions
echo -e "${BLUE}[INFO]${NC} Setting permissions..."

find "${PKG_DIR}" -type d -exec chmod 755 {} \;
find "${PKG_DIR}" -type f -exec chmod 644 {} \;
chmod +x "${PKG_DIR}/usr/bin/speech-recorder"
chmod +x "${PKG_DIR}/DEBIAN/postinst"
chmod +x "${PKG_DIR}/DEBIAN/prerm"
chmod +x "${PKG_DIR}/DEBIAN/postrm"

# 10. Calculate md5sums
echo -e "${BLUE}[INFO]${NC} Calculating checksums..."

cd "${PKG_DIR}"
find usr -type f -exec md5sum {} \; > DEBIAN/md5sums
cd - > /dev/null

# 11. Build package
echo -e "${BLUE}[INFO]${NC} Building .deb package..."

dpkg-deb --build "${PKG_DIR}"

# 12. Move to root directory
mv "packaging/deb_build/${PACKAGE_NAME}.deb" .

# 13. Cleanup
rm -rf "packaging/deb_build"

# 14. Verify package
echo ""
echo -e "${BLUE}[INFO]${NC} Package contents:"
dpkg-deb --contents "${PACKAGE_NAME}.deb" | head -20

echo ""
echo -e "${BLUE}[INFO]${NC} Package info:"
dpkg-deb --info "${PACKAGE_NAME}.deb"

# 15. Success message
echo ""
echo "╔══════════════════════════════════════════╗"
echo "║       Package Built Successfully!        ║"
echo "╚══════════════════════════════════════════╝"
echo ""
echo -e "${GREEN}Package created:${NC} ${PACKAGE_NAME}.deb"
echo ""
echo "Install with:"
echo -e "  ${GREEN}sudo dpkg -i ${PACKAGE_NAME}.deb${NC}"
echo -e "  ${GREEN}sudo apt-get install -f${NC}  (if dependencies missing)"
echo ""
echo "Or:"
echo -e "  ${GREEN}sudo apt install ./${PACKAGE_NAME}.deb${NC}"
echo ""
echo "Test package:"
echo "  lintian ${PACKAGE_NAME}.deb"
echo ""
echo "Uninstall with:"
echo "  sudo apt remove speech-recorder"
echo ""
echo "Visit us at: ${BLUE}https://sparklylabz.com${NC}"
echo ""

