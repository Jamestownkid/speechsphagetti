#!/bin/bash

# ╔═══════════════════════════════════════════════════════════════╗
# ║                                                               ║
# ║       SPEECH RECORDER - ONE-CLICK LINUX INSTALLER            ║
# ║                                                               ║
# ║  This will install Speech Recorder as a normal Linux app     ║
# ║  You'll find it in your Applications menu after install      ║
# ║                                                               ║
# ╚═══════════════════════════════════════════════════════════════╝

set -e

# Change to script directory
cd "$(dirname "$0")"

echo ""
echo "╔═══════════════════════════════════════════════════════╗"
echo "║                                                       ║"
echo "║     WELCOME TO SPEECH RECORDER INSTALLER             ║"
echo "║                                                       ║"
echo "╚═══════════════════════════════════════════════════════╝"
echo ""
echo "This installer will:"
echo "  ✅ Install all required dependencies"
echo "  ✅ Build Speech Recorder"
echo "  ✅ Install it to your system"
echo "  ✅ Add an icon to your Applications menu"
echo "  ✅ Download the base AI model (142 MB)"
echo ""
echo "After installation, search for 'Speech Recorder' in your apps!"
echo ""
read -p "Press ENTER to start installation (will ask for password)..."

echo ""
echo "════════════════════════════════════════════════════════"
echo "Step 1/4: Installing dependencies..."
echo "════════════════════════════════════════════════════════"

sudo apt update
sudo apt install -y \
  build-essential cmake git wget \
  qtbase5-dev qtbase5-dev-tools qttools5-dev-tools \
  libqt5svg5-dev libpulse-dev pkg-config

echo ""
echo "════════════════════════════════════════════════════════"
echo "Step 2/4: Building Speech Recorder..."
echo "════════════════════════════════════════════════════════"

# Make build script executable
chmod +x build.sh

# Build the application
./build.sh

echo ""
echo "════════════════════════════════════════════════════════"
echo "Step 3/4: Installing to system..."
echo "════════════════════════════════════════════════════════"

# Install to /usr instead of /usr/local for better desktop integration
sudo cmake --install build --prefix /usr

# Ensure .desktop file is executable and readable
sudo chmod 644 /usr/share/applications/speech-recorder.desktop 2>/dev/null || true

# Make sure icon is in the right place
sudo mkdir -p /usr/share/icons/hicolor/256x256/apps 2>/dev/null || true
sudo cp resources/icons/app_icon.svg /usr/share/icons/hicolor/256x256/apps/speech-recorder.svg 2>/dev/null || true

# Update desktop and icon caches
sudo update-desktop-database /usr/share/applications 2>/dev/null || true
sudo gtk-update-icon-cache -f /usr/share/icons/hicolor 2>/dev/null || true

echo "✅ Installed to /usr/bin/speech-recorder"
echo "✅ Desktop entry created"

echo ""
echo "════════════════════════════════════════════════════════"
echo "Step 4/4: Creating desktop shortcut (optional)..."
echo "════════════════════════════════════════════════════════"

read -p "Add icon to desktop? (y/n): " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]; then
    DESKTOP_FILE="$HOME/Desktop/speech-recorder.desktop"
    cat > "$DESKTOP_FILE" << 'EOF'
[Desktop Entry]
Version=1.0
Type=Application
Name=Speech Recorder
Comment=Record and transcribe speech with AI
Exec=speech-recorder
Icon=speech-recorder
Terminal=false
Categories=Audio;AudioVideo;Recorder;
Keywords=speech;audio;transcription;whisper;recorder;voice;
MimeType=audio/x-wav;audio/mpeg;
EOF
    chmod +x "$DESKTOP_FILE"
    gio set "$DESKTOP_FILE" metadata::trusted true 2>/dev/null || true
    echo "✅ Desktop shortcut created!"
else
    echo "⏭️  Skipped desktop shortcut"
fi

echo ""
echo ""
echo "╔═══════════════════════════════════════════════════════╗"
echo "║                                                       ║"
echo "║           ✅ INSTALLATION COMPLETE! ✅               ║"
echo "║                                                       ║"
echo "╚═══════════════════════════════════════════════════════╝"
echo ""
echo "🎉 Speech Recorder is now installed!"
echo ""
echo "How to open it:"
echo "  1. Press Super (Windows key)"
echo "  2. Type 'Speech Recorder'"
echo "  3. Click the app!"
echo ""
echo "Or run from terminal: speech-recorder"
echo ""
read -p "Launch Speech Recorder now? (y/n): " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "🚀 Launching..."
    speech-recorder &
    sleep 2
    echo ""
    echo "✅ App launched! Check your screen."
    echo ""
else
    echo "👍 You can launch it anytime from your Applications menu!"
fi

echo ""
echo "════════════════════════════════════════════════════════"
echo "     Installation finished! Enjoy Speech Recorder!     "
echo "════════════════════════════════════════════════════════"
echo ""

