@echo off
REM ════════════════════════════════════════════════════════════════
REM 
REM       SPEECH RECORDER - ONE-CLICK WINDOWS INSTALLER
REM 
REM  This will set up Speech Recorder on Windows using WSL2
REM  (Windows Subsystem for Linux)
REM 
REM ════════════════════════════════════════════════════════════════

color 0A
title Speech Recorder - Windows Installer

cls
echo.
echo ════════════════════════════════════════════════════════════
echo.
echo       WELCOME TO SPEECH RECORDER FOR WINDOWS
echo.
echo ════════════════════════════════════════════════════════════
echo.
echo Speech Recorder uses Linux audio libraries.
echo.
echo To run on Windows, we'll use WSL2 (Windows Subsystem for Linux).
echo Don't worry - it's easy and FREE!
echo.
echo ════════════════════════════════════════════════════════════
echo.
pause

cls
echo.
echo Checking if WSL2 is installed...
echo.

wsl --status >nul 2>&1
if %errorlevel% equ 0 (
    echo ✅ WSL2 is already installed!
    goto :wsl_installed
) else (
    echo ⚠️  WSL2 is not installed yet.
    goto :install_wsl
)

:install_wsl
cls
echo.
echo ════════════════════════════════════════════════════════════
echo   STEP 1: INSTALLING WSL2
echo ════════════════════════════════════════════════════════════
echo.
echo This will install Ubuntu Linux inside Windows.
echo It's official from Microsoft and completely safe!
echo.
echo After installation, your PC will restart.
echo Then run this script again to continue.
echo.
pause

echo.
echo Installing WSL2 with Ubuntu...
echo.

wsl --install -d Ubuntu

echo.
echo ════════════════════════════════════════════════════════════
echo   WSL2 INSTALLED!
echo ════════════════════════════════════════════════════════════
echo.
echo Your computer will restart now.
echo.
echo After restart:
echo   1. Ubuntu will open and ask for username/password
echo   2. Create a simple username and password
echo   3. Run this script again (double-click this file)
echo.
pause
shutdown /r /t 30 /c "Restarting for WSL2 installation. Save your work!"
exit

:wsl_installed
cls
echo.
echo ════════════════════════════════════════════════════════════
echo   STEP 2: INSTALLING SPEECH RECORDER IN WSL2
echo ════════════════════════════════════════════════════════════
echo.
echo Now installing Speech Recorder...
echo.
pause

REM Get the current directory in Windows format and convert to WSL path
set CURRENT_DIR=%cd%
set WSL_PATH=%CURRENT_DIR:\=/%
set WSL_PATH=/mnt/c%WSL_PATH:C:=%

echo Launching installation in WSL2...
echo.

wsl -d Ubuntu -- bash -c "cd '%WSL_PATH%' && chmod +x CLICK_ME_IF_YOUR_ON_LINUX.sh && ./CLICK_ME_IF_YOUR_ON_LINUX.sh"

if %errorlevel% neq 0 (
    echo.
    echo ⚠️  Installation encountered an error.
    echo.
    echo Try this manually:
    echo   1. Open 'Ubuntu' from Start menu
    echo   2. Run these commands:
    echo      cd /mnt/c/Users/YOUR_USERNAME/Downloads/speech-recorder
    echo      chmod +x CLICK_ME_IF_YOUR_ON_LINUX.sh
    echo      ./CLICK_ME_IF_YOUR_ON_LINUX.sh
    echo.
    pause
    exit /b 1
)

cls
echo.
echo ════════════════════════════════════════════════════════════
echo   ✅ INSTALLATION COMPLETE!
echo ════════════════════════════════════════════════════════════
echo.
echo Speech Recorder is now installed!
echo.
echo.
echo ════════════════════════════════════════════════════════════
echo   HOW TO USE IT:
echo ════════════════════════════════════════════════════════════
echo.
echo OPTION 1 - Easy Desktop Shortcut:
echo   We'll create a desktop icon you can double-click!
echo.
echo OPTION 2 - Manual Launch:
echo   1. Open 'Ubuntu' from Windows Start menu
echo   2. Type: speech-recorder
echo   3. Press Enter
echo.
choice /c 12 /n /m "Choose option (1 for Desktop Shortcut, 2 for Manual): "

if %errorlevel% equ 1 (
    goto :create_shortcut
) else (
    goto :manual_finish
)

:create_shortcut
echo.
echo Creating desktop shortcut...

REM Create a batch file on desktop that launches the app
set DESKTOP=%USERPROFILE%\Desktop
set SHORTCUT=%DESKTOP%\Speech Recorder.bat

(
echo @echo off
echo title Speech Recorder
echo wsl -d Ubuntu -- speech-recorder
) > "%SHORTCUT%"

echo.
echo ✅ Desktop shortcut created!
echo.
echo Double-click "Speech Recorder.bat" on your desktop to launch!
echo.
goto :final_message

:manual_finish
echo.
echo 👍 No problem!
echo.
echo To launch manually:
echo   1. Press Windows key
echo   2. Type: Ubuntu
echo   3. Click to open
echo   4. Type: speech-recorder
echo   5. Press Enter
echo.

:final_message
echo.
echo ════════════════════════════════════════════════════════════
echo   🎉 ALL DONE! ENJOY SPEECH RECORDER! 🎉
echo ════════════════════════════════════════════════════════════
echo.
echo Tips:
echo   • The app window will show up on your Windows desktop
echo   • It works just like a normal Windows app
echo   • All your recordings save to your home folder
echo.
choice /c YN /m "Launch Speech Recorder now? (Y/N)"

if %errorlevel% equ 1 (
    echo.
    echo 🚀 Launching Speech Recorder...
    start /b wsl -d Ubuntu -- speech-recorder
    timeout /t 3 >nul
    echo.
    echo ✅ App launched! Check your screen.
    echo.
    echo You can close this window now.
    timeout /t 5
) else (
    echo.
    echo 👍 Launch it anytime using your desktop shortcut!
    echo.
    pause
)

exit /b 0

