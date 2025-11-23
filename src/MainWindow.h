#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTime>
#include <memory>
#include <vector>

// forward declarations
class QPushButton;
class QTextEdit;
class QLabel;
class QProgressBar;
class QTimer;
class QMenuBar;
class AudioRecorder;
class WhisperTranscriber;
class TranscriptionWorker;
class ModelSelector;
class ModelManager;
class SettingsDialog;
class VoskEngine;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Button handlers
    void onRecordButtonClicked();
    void onClearButtonClicked();
    void onCopyButtonClicked();
    void onSaveTXTClicked();
    void onExportDOCXClicked();
    void onExportPDFClicked();
    
    // Transcription handlers
    void onTranscriptionComplete(const QString& text);
    void onTranscriptionError(const QString& error);
    
    // Audio handlers
    void updateAudioLevel(float level);
    void updateRecordingTimer();
    
    // Menu actions
    void onNewRecording();
    void onSaveAs();
    void onExport();
    void onQuit();
    void onManageModels();
    void onSettings();
    void onAbout();
    
    // Model selection
    void onModelChanged(const QString& modelName);
    void onModelsUpdated();

private:
    void setupUI();
    void setupMenuBar();
    void startRecording();
    void stopRecording();
    void setStatus(const QString& status);
    void loadTranscriber(const QString& modelName);
    
    // UI elements
    QPushButton* m_recordButton;
    QPushButton* m_clearButton;
    QPushButton* m_copyButton;
    QPushButton* m_saveTXTButton;
    QPushButton* m_exportDOCXButton;
    QPushButton* m_exportPDFButton;
    QTextEdit* m_textDisplay;
    QLabel* m_statusLabel;
    QLabel* m_timerLabel;
    QLabel* m_footerLabel;
    QProgressBar* m_audioLevel;
    ModelSelector* m_modelSelector;
    
    // Dialogs
    ModelManager* m_modelManager;
    SettingsDialog* m_settingsDialog;
    
    // Core components
    std::unique_ptr<AudioRecorder> m_audioRecorder;
    std::unique_ptr<WhisperTranscriber> m_whisperTranscriber;
    std::unique_ptr<VoskEngine> m_voskEngine;
    
    // Timer for recording duration
    QTimer* m_recordingTimer;
    QTime m_recordingStartTime;
    
    // State tracking
    bool m_isRecording;
    QString m_currentModel;
    std::vector<int16_t> m_audioBuffer;
};

#endif // MAINWINDOW_H
