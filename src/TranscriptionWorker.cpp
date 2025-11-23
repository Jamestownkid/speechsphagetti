#include "TranscriptionWorker.h"
#include "WhisperTranscriber.h"
#include <QDebug>

TranscriptionWorker::TranscriptionWorker(WhisperTranscriber* transcriber,
                                         const std::vector<int16_t>& audioData)
    : m_transcriber(transcriber)
    , m_audioData(audioData) {
}

void TranscriptionWorker::run() {
    try {
        // 1a. run transcription in this thread
        std::string result = m_transcriber->transcribe(m_audioData);
        
        // 1b. convert to QString and emit
        QString text = QString::fromStdString(result);
        
        // 1c. clean up result if empty
        if (text.isEmpty()) {
            text = "(No speech detected)";
        }
        
        emit transcriptionComplete(text);
        
    } catch (const std::exception& e) {
        emit transcriptionError(QString("Transcription failed: %1").arg(e.what()));
    }
}
