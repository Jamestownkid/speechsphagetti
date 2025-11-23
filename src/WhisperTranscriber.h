#ifndef WHISPERTRANSCRIBER_H
#define WHISPERTRANSCRIBER_H

#include <QString>
#include <string>
#include <vector>
#include <memory>

// Forward declare Whisper types
struct whisper_context;

class WhisperTranscriber {
public:
    WhisperTranscriber();
    explicit WhisperTranscriber(const QString& modelPath);
    ~WhisperTranscriber();
    
    // Main transcription method
    std::string transcribe(const std::vector<int16_t>& audioData);
    
    // Check if model is loaded
    bool isModelLoaded() const;
    
private:
    // Convert int16 PCM to float samples for Whisper
    std::vector<float> convertToFloat(const std::vector<int16_t>& pcm);
    
    whisper_context* m_ctx;
    QString m_modelPath;
};

#endif // WHISPERTRANSCRIBER_H
