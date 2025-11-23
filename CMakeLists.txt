cmake_minimum_required(VERSION 3.16)
project(SpeechRecorder VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find packages
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network PrintSupport)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PULSEAUDIO REQUIRED libpulse-simple)

# Optional: Vosk support
pkg_check_modules(VOSK vosk)
if(VOSK_FOUND)
    add_definitions(-DVOSK_AVAILABLE)
    message(STATUS "Vosk support enabled")
else()
    message(STATUS "Vosk support disabled (libvosk not found)")
endif()

# Add whisper.cpp as subdirectory (if exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/whisper.cpp/CMakeLists.txt")
    add_subdirectory(external/whisper.cpp EXCLUDE_FROM_ALL)
    set(WHISPER_AVAILABLE ON)
else()
    message(WARNING "whisper.cpp not found in external/. Clone it with:")
    message(WARNING "  git clone https://github.com/ggerganov/whisper.cpp.git external/whisper.cpp")
    set(WHISPER_AVAILABLE OFF)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${PULSEAUDIO_INCLUDE_DIRS}
)

if(WHISPER_AVAILABLE)
    include_directories(external/whisper.cpp)
endif()

if(VOSK_FOUND)
    include_directories(${VOSK_INCLUDE_DIRS})
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/AudioRecorder.cpp
    src/WhisperTranscriber.cpp
    src/TranscriptionWorker.cpp
    src/transcription/VoskEngine.cpp
    src/gui/ModelSelector.cpp
    src/gui/ModelManager.cpp
    src/gui/SettingsDialog.cpp
    src/utils/FileExporter.cpp
    src/utils/Settings.cpp
    src/utils/ErrorHandler.cpp
)

set(HEADERS
    src/MainWindow.h
    src/AudioRecorder.h
    src/WhisperTranscriber.h
    src/TranscriptionWorker.h
    src/transcription/VoskEngine.h
    src/gui/ModelSelector.h
    src/gui/ModelManager.h
    src/gui/SettingsDialog.h
    src/utils/FileExporter.h
    src/utils/Settings.h
    src/utils/ErrorHandler.h
)

# Resource files
set(RESOURCES
    resources/resources.qrc
)

# Create executable
add_executable(speech-recorder 
    ${SOURCES} 
    ${HEADERS}
    ${RESOURCES}
)

# Link libraries
target_link_libraries(speech-recorder
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::PrintSupport
    ${PULSEAUDIO_LIBRARIES}
    pthread
)

if(WHISPER_AVAILABLE)
    target_link_libraries(speech-recorder whisper)
endif()

if(VOSK_FOUND)
    target_link_libraries(speech-recorder ${VOSK_LIBRARIES})
endif()

# Set PulseAudio compile flags
target_compile_options(speech-recorder PRIVATE ${PULSEAUDIO_CFLAGS_OTHER})

# Installation rules
install(TARGETS speech-recorder DESTINATION bin)
install(FILES packaging/speech-recorder.desktop DESTINATION share/applications)
install(FILES resources/icons/app_icon.svg 
        DESTINATION share/icons/hicolor/256x256/apps 
        RENAME speech-recorder.svg)

# Print build configuration
message(STATUS "=== Speech Recorder Build Configuration ===")
message(STATUS "Qt5 version: ${Qt5_VERSION}")
message(STATUS "PulseAudio: ${PULSEAUDIO_VERSION}")
message(STATUS "Whisper.cpp: ${WHISPER_AVAILABLE}")
message(STATUS "Vosk: ${VOSK_FOUND}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "==========================================")
