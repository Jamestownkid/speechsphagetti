#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include <QThread>
#include <vector>
#include <atomic>
#include <memory>

// forward declare to avoid pulse headers in header file
typedef struct pa_simple pa_simple;

class AudioRecorder : public QObject {
    Q_OBJECT

public:
    AudioRecorder();
    ~AudioRecorder();
    
    void startRecording();
    std::vector<int16_t> stopRecording();
    
signals:
    void audioLevelChanged(float level);
    void recordingError(const QString& error);

private:
    void recordingLoop();
    float calculateRMS(const int16_t* buffer, size_t size);
    
    pa_simple* m_pulseAudioHandle;
    std::unique_ptr<QThread> m_recordThread;
    std::atomic<bool> m_isRecording;
    std::vector<int16_t> m_audioBuffer;
    
    // constants
    static constexpr int SAMPLE_RATE = 16000;
    static constexpr int CHANNELS = 1;
    static constexpr int BUFFER_SIZE = 1024;
};

#endif // AUDIORECORDER_H
