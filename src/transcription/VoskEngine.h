#ifndef VOSKENGINE_H
#define VOSKENGINE_H

#include <string>
#include <vector>
#include <memory>

// Forward declaration for Vosk types
struct VoskModel;
struct VoskRecognizer;

class VoskEngine {
public:
    VoskEngine();
    explicit VoskEngine(const std::string& modelPath);
    ~VoskEngine();
    
    // Main transcription method
    std::string transcribe(const std::vector<int16_t>& audioData);
    
    // Check if model is loaded
    bool isModelLoaded() const;
    
    // Load a specific model
    bool loadModel(const std::string& modelPath);
    
private:
    void cleanup();
    
    VoskModel* m_model;
    VoskRecognizer* m_recognizer;
    
    static constexpr int SAMPLE_RATE = 16000;
    static constexpr const char* DEFAULT_MODEL_PATH = "./models/vosk/vosk-model-small-en-us-0.15";
};

#endif // VOSKENGINE_H
