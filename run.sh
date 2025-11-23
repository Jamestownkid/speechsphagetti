#!/bin/bash
# Easy launcher for Linux
# Made by SparklyLabz

echo "╔══════════════════════════════════════════╗"
echo "║      Speech Recorder Launcher           ║"
echo "║        Made by SparklyLabz              ║"
echo "╚══════════════════════════════════════════╝"
echo ""

# Check if already built
if [ ! -f "build/speech-recorder" ]; then
    echo "App not built yet. Building now..."
    echo ""
    ./build.sh
    
    if [ $? -ne 0 ]; then
        echo ""
        echo "❌ Build failed. Please fix errors above."
        exit 1
    fi
fi

# Run the app
echo ""
echo "🚀 Launching Speech Recorder..."
echo ""

./build/speech-recorder

echo ""
echo "Thanks for using Speech Recorder!"
echo "Visit: https://sparklylabz.com"

