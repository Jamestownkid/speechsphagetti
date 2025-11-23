#include "WhisperTranscriber.h"
#include "whisper.h"
#include <stdexcept>
#include <QDebug>
#include <QFile>

WhisperTranscriber::WhisperTranscriber() 
    : m_ctx(nullptr)
    , m_modelPath("./models/ggml-base.bin") {
    
    if (!QFile::exists(m_modelPath)) {
        throw std::runtime_error(
            "Whisper model not found at " + m_modelPath.toStdString() + "\n"
            "Download from: https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.bin"
        );
    }
    
    struct whisper_context_params cparams = whisper_context_default_params();
    m_ctx = whisper_init_from_file_with_params(m_modelPath.toStdString().c_str(), cparams);
    
    if (!m_ctx) {
        throw std::runtime_error("Failed to load Whisper model from " + m_modelPath.toStdString());
    }
    
    qDebug() << "Whisper model loaded:" << m_modelPath;
}

WhisperTranscriber::WhisperTranscriber(const QString& modelPath) 
    : m_ctx(nullptr)
    , m_modelPath(modelPath) {
    
    if (!QFile::exists(m_modelPath)) {
        throw std::runtime_error(
            "Whisper model not found: " + m_modelPath.toStdString()
        );
    }
    
    struct whisper_context_params cparams = whisper_context_default_params();
    m_ctx = whisper_init_from_file_with_params(m_modelPath.toStdString().c_str(), cparams);
    
    if (!m_ctx) {
        throw std::runtime_error("Failed to load Whisper model: " + m_modelPath.toStdString());
    }
    
    qDebug() << "Whisper model loaded:" << m_modelPath;
}

WhisperTranscriber::~WhisperTranscriber() {
    if (m_ctx) {
        whisper_free(m_ctx);
    }
}

std::string WhisperTranscriber::transcribe(const std::vector<int16_t>& audioData) {
    if (!m_ctx) {
        throw std::runtime_error("Whisper context not initialized");
    }
    
    if (audioData.empty()) {
        return "";
    }
    
    // 2a. convert int16 to float
    std::vector<float> floatData = convertToFloat(audioData);
    
    // 2b. setup whisper params
    whisper_full_params params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
    
    params.n_threads = 4;           // use 4 CPU threads
    params.language = "en";          // force English
    params.print_special = false;
    params.print_progress = false;
    params.print_timestamps = false;
    params.single_segment = false;   // allow multiple segments
    params.no_context = false;       // use context for better accuracy
    
    // 2c. run transcription
    int result = whisper_full(m_ctx, params, floatData.data(), floatData.size());
    
    if (result != 0) {
        throw std::runtime_error("Whisper transcription failed with code: " + std::to_string(result));
    }
    
    // 2d. collect all segments into result string
    std::string transcription;
    const int n_segments = whisper_full_n_segments(m_ctx);
    
    for (int i = 0; i < n_segments; ++i) {
        const char* text = whisper_full_get_segment_text(m_ctx, i);
        if (text) {
            transcription += text;
            transcription += " ";
        }
    }
    
    // 2e. trim trailing space
    if (!transcription.empty() && transcription.back() == ' ') {
        transcription.pop_back();
    }
    
    return transcription;
}

std::vector<float> WhisperTranscriber::convertToFloat(const std::vector<int16_t>& pcm) {
    // 3a. convert 16-bit PCM to float [-1.0, 1.0]
    std::vector<float> result(pcm.size());
    
    for (size_t i = 0; i < pcm.size(); ++i) {
        result[i] = static_cast<float>(pcm[i]) / 32768.0f;
    }
    
    return result;
}

bool WhisperTranscriber::isModelLoaded() const {
    return m_ctx != nullptr;
}
