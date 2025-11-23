#ifndef TRANSCRIPTIONWORKER_H
#define TRANSCRIPTIONWORKER_H

#include <QThread>
#include <vector>

class WhisperTranscriber;

class TranscriptionWorker : public QThread {
    Q_OBJECT

public:
    TranscriptionWorker(WhisperTranscriber* transcriber, 
                       const std::vector<int16_t>& audioData);
    
protected:
    void run() override;
    
signals:
    void transcriptionComplete(const QString& text);
    void transcriptionError(const QString& error);
    
private:
    WhisperTranscriber* m_transcriber;
    std::vector<int16_t> m_audioData;
};

#endif // TRANSCRIPTIONWORKER_H
