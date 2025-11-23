#include "VoskEngine.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <stdexcept>
#include <cstring>

// Vosk headers
#ifdef VOSK_AVAILABLE
#include <vosk_api.h>
#endif

VoskEngine::VoskEngine()
    : m_model(nullptr)
    , m_recognizer(nullptr) {
#ifdef VOSK_AVAILABLE
    // Try to load default model
    if (QDir(DEFAULT_MODEL_PATH).exists()) {
        loadModel(DEFAULT_MODEL_PATH);
    } else {
        qWarning() << "Vosk default model not found at:" << DEFAULT_MODEL_PATH;
    }
#else
    qWarning() << "Vosk support not compiled in. Rebuild with VOSK_AVAILABLE defined.";
#endif
}

VoskEngine::VoskEngine(const std::string& modelPath)
    : m_model(nullptr)
    , m_recognizer(nullptr) {
#ifdef VOSK_AVAILABLE
    loadModel(modelPath);
#else
    throw std::runtime_error("Vosk support not available - rebuild with libvosk");
#endif
}

VoskEngine::~VoskEngine() {
    cleanup();
}

bool VoskEngine::loadModel(const std::string& modelPath) {
#ifdef VOSK_AVAILABLE
    // Clean up existing model if any
    cleanup();
    
    // Check if model directory exists
    if (!QDir(QString::fromStdString(modelPath)).exists()) {
        qWarning() << "Vosk model directory not found:" << modelPath.c_str();
        return false;
    }
    
    // Set Vosk log level (0 = errors only)
    vosk_set_log_level(-1);
    
    // Load the model
    m_model = vosk_model_new(modelPath.c_str());
    
    if (!m_model) {
        qCritical() << "Failed to load Vosk model from:" << modelPath.c_str();
        return false;
    }
    
    // Create recognizer
    m_recognizer = vosk_recognizer_new(m_model, SAMPLE_RATE);
    
    if (!m_recognizer) {
        qCritical() << "Failed to create Vosk recognizer";
        vosk_model_free(m_model);
        m_model = nullptr;
        return false;
    }
    
    qDebug() << "Vosk model loaded successfully from:" << modelPath.c_str();
    return true;
#else
    return false;
#endif
}

std::string VoskEngine::transcribe(const std::vector<int16_t>& audioData) {
#ifdef VOSK_AVAILABLE
    if (!m_model || !m_recognizer) {
        throw std::runtime_error("Vosk engine not initialized - no model loaded");
    }
    
    if (audioData.empty()) {
        return "";
    }
    
    // Reset recognizer for fresh transcription
    vosk_recognizer_reset(m_recognizer);
    
    // Feed audio data in chunks
    const size_t CHUNK_SIZE = 8000; // Process in 0.5 second chunks
    size_t offset = 0;
    
    while (offset < audioData.size()) {
        size_t remaining = audioData.size() - offset;
        size_t chunkSize = std::min(remaining, CHUNK_SIZE);
        
        const char* audioPtr = reinterpret_cast<const char*>(audioData.data() + offset);
        size_t byteSize = chunkSize * sizeof(int16_t);
        
        int result = vosk_recognizer_accept_waveform(m_recognizer, audioPtr, byteSize);
        
        offset += chunkSize;
    }
    
    // Get final result
    const char* resultJson = vosk_recognizer_final_result(m_recognizer);
    
    if (!resultJson) {
        return "";
    }
    
    // Parse JSON to extract text
    // Simple parsing - look for "text" : "..."
    std::string jsonStr(resultJson);
    size_t textPos = jsonStr.find("\"text\" : \"");
    
    if (textPos == std::string::npos) {
        textPos = jsonStr.find("\"text\":\"");
        if (textPos != std::string::npos) {
            textPos += 8; // length of "text":"
        }
    } else {
        textPos += 10; // length of "text" : "
    }
    
    if (textPos == std::string::npos) {
        qWarning() << "Could not parse Vosk result:" << resultJson;
        return "";
    }
    
    size_t endPos = jsonStr.find("\"", textPos);
    if (endPos == std::string::npos) {
        return "";
    }
    
    std::string text = jsonStr.substr(textPos, endPos - textPos);
    
    qDebug() << "Vosk transcription result:" << text.c_str();
    return text;
#else
    return "Vosk transcription not available - rebuild with libvosk";
#endif
}

bool VoskEngine::isModelLoaded() const {
#ifdef VOSK_AVAILABLE
    return m_model != nullptr && m_recognizer != nullptr;
#else
    return false;
#endif
}

void VoskEngine::cleanup() {
#ifdef VOSK_AVAILABLE
    if (m_recognizer) {
        vosk_recognizer_free(m_recognizer);
        m_recognizer = nullptr;
    }
    
    if (m_model) {
        vosk_model_free(m_model);
        m_model = nullptr;
    }
#endif
}

