#include "AudioRecorder.h"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <QDebug>
#include <cmath>
#include <stdexcept>

AudioRecorder::AudioRecorder() 
    : m_pulseAudioHandle(nullptr)
    , m_isRecording(false) {
    
    // 1a. setup pulse audio connection
    pa_sample_spec ss;
    ss.format = PA_SAMPLE_S16LE;   // 16-bit signed little-endian
    ss.channels = CHANNELS;         // mono
    ss.rate = SAMPLE_RATE;          // 16kHz
    
    int error = 0;
    
    // 1b. create pulse simple connection
    m_pulseAudioHandle = pa_simple_new(
        nullptr,                    // default server
        "SpeechRecorder",           // app name  
        PA_STREAM_RECORD,           // recording stream
        nullptr,                    // default device
        "Speech Input",             // stream description
        &ss,                        // sample spec
        nullptr,                    // default channel map
        nullptr,                    // default buffer attrs
        &error                      // error code
    );
    
    if (!m_pulseAudioHandle) {
        throw std::runtime_error(
            QString("PulseAudio init failed: %1").arg(pa_strerror(error)).toStdString()
        );
    }
}

AudioRecorder::~AudioRecorder() {
    if (m_isRecording) {
        stopRecording();
    }
    
    if (m_pulseAudioHandle) {
        pa_simple_free(m_pulseAudioHandle);
    }
}

void AudioRecorder::startRecording() {
    if (m_isRecording) {
        return;
    }
    
    // 2a. clear old buffer
    m_audioBuffer.clear();
    m_audioBuffer.reserve(SAMPLE_RATE * 60); // reserve 1 minute initially
    
    // 2b. start recording thread
    m_isRecording = true;
    m_recordThread = std::make_unique<QThread>();
    
    // 2c. move recording to thread
    connect(m_recordThread.get(), &QThread::started,
            [this]() { recordingLoop(); });
    
    m_recordThread->start();
}

std::vector<int16_t> AudioRecorder::stopRecording() {
    if (!m_isRecording) {
        return {};
    }
    
    // 3a. signal thread to stop
    m_isRecording = false;
    
    // 3b. wait for thread to finish
    if (m_recordThread && m_recordThread->isRunning()) {
        m_recordThread->quit();
        m_recordThread->wait(5000); // 5 second timeout
    }
    
    // 3c. return captured audio
    return std::move(m_audioBuffer);
}

void AudioRecorder::recordingLoop() {
    int16_t buffer[BUFFER_SIZE];
    int error = 0;
    
    while (m_isRecording) {
        // 4a. read audio chunk from pulse
        int bytes_read = pa_simple_read(
            m_pulseAudioHandle,
            buffer,
            sizeof(buffer),
            &error
        );
        
        if (bytes_read < 0) {
            emit recordingError(QString("Read error: %1").arg(pa_strerror(error)));
            break;
        }
        
        // 4b. append to main buffer
        m_audioBuffer.insert(m_audioBuffer.end(), 
                           buffer, 
                           buffer + BUFFER_SIZE);
        
        // 4c. calculate and emit audio level
        float rms = calculateRMS(buffer, BUFFER_SIZE);
        emit audioLevelChanged(rms);
    }
}

float AudioRecorder::calculateRMS(const int16_t* buffer, size_t size) {
    if (size == 0) return 0.0f;
    
    // 5a. calculate root mean square for audio level
    double sum = 0.0;
    for (size_t i = 0; i < size; ++i) {
        double normalized = static_cast<double>(buffer[i]) / 32768.0;
        sum += normalized * normalized;
    }
    
    // 5b. return RMS value (0.0 to 1.0)
    return std::sqrt(sum / size);
}
