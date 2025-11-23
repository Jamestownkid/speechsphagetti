#include "MainWindow.h"
#include "AudioRecorder.h"
#include "WhisperTranscriber.h"
#include "TranscriptionWorker.h"
#include "transcription/VoskEngine.h"
#include "gui/ModelSelector.h"
#include "gui/ModelManager.h"
#include "gui/SettingsDialog.h"
#include "utils/FileExporter.h"
#include "utils/Settings.h"
#include "utils/ErrorHandler.h"

#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QProgressBar>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QGroupBox>

// VoskEngine is included directly

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent)
    , m_isRecording(false)
    , m_modelManager(nullptr)
    , m_settingsDialog(nullptr)
    , m_recordingTimer(new QTimer(this)) {
    
    setupMenuBar();
    setupUI();
    
    // Connect recording timer
    connect(m_recordingTimer, &QTimer::timeout, this, &MainWindow::updateRecordingTimer);
    
    // Initialize audio recorder
    try {
        m_audioRecorder = std::make_unique<AudioRecorder>();
        connect(m_audioRecorder.get(), &AudioRecorder::audioLevelChanged, 
                this, &MainWindow::updateAudioLevel);
    } catch (const std::exception& e) {
        ErrorHandler::showPulseAudioError(this, e.what());
    }
    
    // Load default model from settings
    QString defaultModel = Settings::instance().defaultModel();
    m_modelSelector->refreshAvailableModels();
    loadTranscriber(defaultModel);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    // File Menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    
    QAction* newAction = fileMenu->addAction("&New Recording");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::onNewRecording);
    
    QAction* saveAction = fileMenu->addAction("&Save As...");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveAs);
    
    QAction* exportAction = fileMenu->addAction("&Export...");
    connect(exportAction, &QAction::triggered, this, &MainWindow::onExport);
    
    fileMenu->addSeparator();
    
    QAction* quitAction = fileMenu->addAction("&Quit");
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &MainWindow::onQuit);
    
    // Edit Menu
    QMenu* editMenu = menuBar->addMenu("&Edit");
    
    QAction* copyAction = editMenu->addAction("&Copy");
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, this, &MainWindow::onCopyButtonClicked);
    
    QAction* clearAction = editMenu->addAction("C&lear");
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClearButtonClicked);
    
    // Tools Menu
    QMenu* toolsMenu = menuBar->addMenu("&Tools");
    
    QAction* modelsAction = toolsMenu->addAction("Manage &Models...");
    connect(modelsAction, &QAction::triggered, this, &MainWindow::onManageModels);
    
    QAction* settingsAction = toolsMenu->addAction("&Settings...");
    connect(settingsAction, &QAction::triggered, this, &MainWindow::onSettings);
    
    // Help Menu
    QMenu* helpMenu = menuBar->addMenu("&Help");
    
    QAction* aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::setupUI() {
    // Central widget and main layout
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 10);
    
    // Model selector row
    QHBoxLayout* modelLayout = new QHBoxLayout();
    QLabel* modelLabel = new QLabel("Model:", this);
    modelLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    modelLayout->addWidget(modelLabel);
    
    m_modelSelector = new ModelSelector(this);
    connect(m_modelSelector, &ModelSelector::modelChanged, 
            this, &MainWindow::onModelChanged);
    modelLayout->addWidget(m_modelSelector);
    modelLayout->addStretch();
    
    // Timer label
    m_timerLabel = new QLabel("00:00", this);
    m_timerLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #888;");
    modelLayout->addWidget(m_timerLabel);
    
    mainLayout->addLayout(modelLayout);
    
    // Audio level indicator
    m_audioLevel = new QProgressBar(this);
    m_audioLevel->setRange(0, 100);
    m_audioLevel->setValue(0);
    m_audioLevel->setTextVisible(false);
    m_audioLevel->setFixedHeight(10);
    m_audioLevel->setStyleSheet(
        "QProgressBar { border: 1px solid #555; border-radius: 3px; }"
        "QProgressBar::chunk { background-color: #4CAF50; }"
    );
    mainLayout->addWidget(m_audioLevel);
    
    // Record button (big and obvious)
    m_recordButton = new QPushButton("⬤ RECORD", this);
    m_recordButton->setFixedHeight(80);
    m_recordButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #c62828;"
        "  color: white;"
        "  font-size: 24px;"
        "  font-weight: bold;"
        "  border-radius: 40px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #d32f2f;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #b71c1c;"
        "}"
    );
    connect(m_recordButton, &QPushButton::clicked, 
            this, &MainWindow::onRecordButtonClicked);
    mainLayout->addWidget(m_recordButton);
    
    // Status label
    m_statusLabel = new QLabel("Ready", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: #888; font-size: 12px;");
    mainLayout->addWidget(m_statusLabel);
    
    // Text display area
    m_textDisplay = new QTextEdit(this);
    m_textDisplay->setPlaceholderText(
        "Click RECORD and start speaking.\n"
        "When you're done, click STOP and your speech will be transcribed automatically."
    );
    m_textDisplay->setStyleSheet(
        "QTextEdit {"
        "  font-family: 'Consolas', 'Monaco', monospace;"
        "  font-size: 14px;"
        "  padding: 10px;"
        "  border: 1px solid #555;"
        "  border-radius: 5px;"
        "}"
    );
    mainLayout->addWidget(m_textDisplay, 1); // stretch factor 1
    
    // Button row
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_clearButton = new QPushButton("Clear", this);
    m_clearButton->setFixedWidth(100);
    connect(m_clearButton, &QPushButton::clicked, 
            this, &MainWindow::onClearButtonClicked);
    buttonLayout->addWidget(m_clearButton);
    
    m_copyButton = new QPushButton("Copy", this);
    m_copyButton->setFixedWidth(100);
    connect(m_copyButton, &QPushButton::clicked, 
            this, &MainWindow::onCopyButtonClicked);
    buttonLayout->addWidget(m_copyButton);
    
    buttonLayout->addSpacing(20);
    
    // Export buttons
    m_saveTXTButton = new QPushButton("Save TXT", this);
    m_saveTXTButton->setFixedWidth(100);
    connect(m_saveTXTButton, &QPushButton::clicked,
            this, &MainWindow::onSaveTXTClicked);
    buttonLayout->addWidget(m_saveTXTButton);
    
    m_exportDOCXButton = new QPushButton("Export DOCX", this);
    m_exportDOCXButton->setFixedWidth(120);
    connect(m_exportDOCXButton, &QPushButton::clicked,
            this, &MainWindow::onExportDOCXClicked);
    buttonLayout->addWidget(m_exportDOCXButton);
    
    m_exportPDFButton = new QPushButton("Export PDF", this);
    m_exportPDFButton->setFixedWidth(120);
    connect(m_exportPDFButton, &QPushButton::clicked,
            this, &MainWindow::onExportPDFClicked);
    buttonLayout->addWidget(m_exportPDFButton);
    
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    // Footer with SparklyLabz link
    m_footerLabel = new QLabel(this);
    m_footerLabel->setText("<a href='https://sparklylabz.com' style='color: #888;'>Made with ♥ by SparklyLabz | sparklylabz.com</a>");
    m_footerLabel->setOpenExternalLinks(true);
    m_footerLabel->setAlignment(Qt::AlignCenter);
    m_footerLabel->setStyleSheet("color: #666; font-size: 11px; padding: 5px;");
    mainLayout->addWidget(m_footerLabel);
}

void MainWindow::loadTranscriber(const QString& modelName) {
    m_currentModel = modelName;
    
    try {
        if (modelName.startsWith("Whisper")) {
            // Determine model filename based on name
            QString modelFile;
            if (modelName.contains("Tiny En")) modelFile = "ggml-tiny.en.bin";
            else if (modelName.contains("Tiny")) modelFile = "ggml-tiny.bin";
            else if (modelName.contains("Base En")) modelFile = "ggml-base.en.bin";
            else if (modelName.contains("Base")) modelFile = "ggml-base.bin";
            else if (modelName.contains("Small En")) modelFile = "ggml-small.en.bin";
            else if (modelName.contains("Small")) modelFile = "ggml-small.bin";
            else if (modelName.contains("Medium En")) modelFile = "ggml-medium.en.bin";
            else if (modelName.contains("Medium")) modelFile = "ggml-medium.bin";
            else if (modelName.contains("Large V1")) modelFile = "ggml-large-v1.bin";
            else if (modelName.contains("Large V2")) modelFile = "ggml-large-v2.bin";
            else if (modelName.contains("Large V3")) modelFile = "ggml-large-v3.bin";
            else modelFile = "ggml-base.bin"; // fallback
            
            QString modelPath = Settings::instance().modelDirectory() + "/" + modelFile;
            
            // Check if model exists before trying to load
            if (!QFile::exists(modelPath)) {
                throw std::runtime_error(QString("Model file not found: %1\nDownload it from Tools > Manage Models").arg(modelFile).toStdString());
            }
            
            m_whisperTranscriber = std::make_unique<WhisperTranscriber>(modelPath);
            m_voskEngine.reset();
            setStatus(QString("Ready - %1 loaded").arg(modelName));
            
        } else if (modelName.startsWith("Vosk")) {
            // Vosk support is optional
#ifdef VOSK_AVAILABLE
            QString modelPath = Settings::instance().modelDirectory();
            if (modelName == "Vosk Small") {
                modelPath += "/vosk-model-small-en-us-0.15";
            } else {
                modelPath += "/vosk-model-en-us-0.22";
            }
            
            m_voskEngine = std::make_unique<VoskEngine>(modelPath.toStdString());
            m_whisperTranscriber.reset();
            
            if (m_voskEngine->isModelLoaded()) {
                setStatus(QString("Ready - %1 loaded").arg(modelName));
            } else {
                throw std::runtime_error("Vosk model not found. Download from Tools > Manage Models.");
            }
#else
            // Vosk not available - fallback to Whisper Base
            QMessageBox::warning(this, "Vosk Not Available",
                "Vosk support not compiled in. Falling back to Whisper Base.\n\n"
                "To enable Vosk: install libvosk and rebuild.");
            
            // Load Whisper Base as fallback
            m_modelSelector->setCurrentText("Whisper Base");
            return; // Will trigger loadTranscriber again with Whisper Base
#endif
        }
    } catch (const std::exception& e) {
        ErrorHandler::showModelLoadError(this, modelName, e.what());
        setStatus("Error: Model not loaded");
        
        // Try to fall back to Whisper Base if available
        if (modelName != "Whisper Base" && QFile::exists(Settings::instance().modelDirectory() + "/ggml-base.bin")) {
            QMessageBox::information(this, "Using Fallback Model",
                "Loading Whisper Base as fallback model.");
            m_modelSelector->setCurrentText("Whisper Base");
        }
    }
}

void MainWindow::onRecordButtonClicked() {
    if (!m_audioRecorder) {
        QMessageBox::warning(this, "Error", "Audio system not initialized");
        return;
    }
    
    if (m_isRecording) {
        stopRecording();
    } else {
        startRecording();
    }
}

void MainWindow::startRecording() {
    // Clear previous text
    m_textDisplay->clear();
    
    // Update UI state
    m_recordButton->setText("⬛ STOP");
    m_recordButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #d32f2f;"
        "  color: white;"
        "  font-size: 24px;"
        "  font-weight: bold;"
        "  border-radius: 40px;"
        "  border: 3px solid #ff5252;"
        "}"
    );
    
    // Start timer
    m_recordingStartTime = QTime::currentTime();
    m_recordingTimer->start(100); // Update every 100ms
    
    // Start recording
    m_audioRecorder->startRecording();
    m_isRecording = true;
    setStatus("🔴 Recording... Speak now");
}

void MainWindow::stopRecording() {
    // Stop recording and get audio data
    m_audioBuffer = m_audioRecorder->stopRecording();
    
    // Update UI
    m_recordButton->setText("⬤ RECORD");
    m_recordButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #c62828;"
        "  color: white;"
        "  font-size: 24px;"
        "  font-weight: bold;"
        "  border-radius: 40px;"
        "}"
    );
    m_isRecording = false;
    m_audioLevel->setValue(0);
    m_recordingTimer->stop();
    
    // Check if we got any audio
    if (m_audioBuffer.empty() || m_audioBuffer.size() < 1600) { // less than 0.1 sec
        QMessageBox::warning(this, "Warning", 
            "No audio recorded. Please check your microphone.\n\n"
            "Test with: pactl list sources");
        setStatus("Ready");
        m_timerLabel->setText("00:00");
        return;
    }
    
    setStatus("⏳ Transcribing... Please wait");
    
    // Check if we have a transcriber loaded
    if (!m_whisperTranscriber && !m_voskEngine) {
        QMessageBox::critical(this, "Error", 
            "No transcription model loaded.\n"
            "Please select a model from the dropdown.");
        setStatus("Ready");
        return;
    }
    
    // Create worker thread for transcription
    TranscriptionWorker* worker = nullptr;
    if (m_whisperTranscriber) {
        worker = new TranscriptionWorker(m_whisperTranscriber.get(), m_audioBuffer);
    } else if (m_voskEngine) {
        // For Vosk, we need to create a custom worker
        // For now, transcribe directly (TODO: make async)
        try {
            std::string result = m_voskEngine->transcribe(m_audioBuffer);
            onTranscriptionComplete(QString::fromStdString(result));
            return;
        } catch (const std::exception& e) {
            onTranscriptionError(QString::fromStdString(e.what()));
            return;
        }
    }
    
    if (worker) {
        connect(worker, &TranscriptionWorker::transcriptionComplete,
                this, &MainWindow::onTranscriptionComplete);
        connect(worker, &TranscriptionWorker::transcriptionError,
                this, &MainWindow::onTranscriptionError);
        connect(worker, &TranscriptionWorker::finished,
                worker, &QObject::deleteLater);
        
        worker->start();
    }
}

void MainWindow::updateRecordingTimer() {
    if (m_isRecording) {
        int msecs = m_recordingStartTime.msecsTo(QTime::currentTime());
        int seconds = msecs / 1000;
        int minutes = seconds / 60;
        seconds = seconds % 60;
        
        m_timerLabel->setText(QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0')));
        m_timerLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #ff5252;");
    }
}

void MainWindow::onTranscriptionComplete(const QString& text) {
    m_textDisplay->setPlainText(text);
    setStatus("✓ Transcription complete");
    m_timerLabel->setText("00:00");
    m_timerLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #888;");
    
    // Auto-reset status after 3 seconds
    QTimer::singleShot(3000, [this]() {
        setStatus("Ready");
    });
}

void MainWindow::onTranscriptionError(const QString& error) {
    ErrorHandler::showTranscriptionError(this, error);
    setStatus("Error - Ready");
    m_timerLabel->setText("00:00");
    m_timerLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #888;");
}

void MainWindow::updateAudioLevel(float level) {
    // Convert to 0-100 range for progress bar
    int value = static_cast<int>(level * 100);
    m_audioLevel->setValue(value);
}

// Button handlers
void MainWindow::onClearButtonClicked() {
    m_textDisplay->clear();
}

void MainWindow::onCopyButtonClicked() {
    QString text = m_textDisplay->toPlainText();
    if (!text.isEmpty()) {
        FileExporter::copyToClipboard(text);
        setStatus("✓ Copied to clipboard");
        
        QTimer::singleShot(2000, [this]() {
            setStatus("Ready");
        });
    }
}

void MainWindow::onSaveTXTClicked() {
    QString text = m_textDisplay->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::information(this, "No Content", "Nothing to save.");
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(this, "Save Transcription",
                                                   "transcription.txt",
                                                   "Text Files (*.txt)");
    if (!filename.isEmpty()) {
        if (FileExporter::exportToTXT(text, filename)) {
            setStatus("✓ Saved to " + filename);
            QTimer::singleShot(3000, [this]() { setStatus("Ready"); });
        } else {
            ErrorHandler::showFileError(this, "save text", filename);
        }
    }
}

void MainWindow::onExportDOCXClicked() {
    QString text = m_textDisplay->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::information(this, "No Content", "Nothing to export.");
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(this, "Export to DOCX",
                                                   "transcription.docx",
                                                   "Word Documents (*.docx)");
    if (!filename.isEmpty()) {
        if (FileExporter::exportToDOCX(text, filename)) {
            setStatus("✓ Exported to " + filename);
            QTimer::singleShot(3000, [this]() { setStatus("Ready"); });
        } else {
            ErrorHandler::showFileError(this, "export DOCX", filename);
        }
    }
}

void MainWindow::onExportPDFClicked() {
    QString text = m_textDisplay->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::information(this, "No Content", "Nothing to export.");
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(this, "Export to PDF",
                                                   "transcription.pdf",
                                                   "PDF Documents (*.pdf)");
    if (!filename.isEmpty()) {
        if (FileExporter::exportToPDF(text, filename)) {
            setStatus("✓ Exported to " + filename);
            QTimer::singleShot(3000, [this]() { setStatus("Ready"); });
        } else {
            ErrorHandler::showFileError(this, "export PDF", filename);
        }
    }
}

// Menu actions
void MainWindow::onNewRecording() {
    m_textDisplay->clear();
    m_timerLabel->setText("00:00");
    setStatus("Ready");
}

void MainWindow::onSaveAs() {
    onSaveTXTClicked();
}

void MainWindow::onExport() {
    QString text = m_textDisplay->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::information(this, "No Content", "Nothing to export.");
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Export Format",
        "Choose export format:\n\n"
        "• Yes = PDF\n"
        "• No = DOCX\n"
        "• Cancel = TXT",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    
    if (reply == QMessageBox::Yes) {
        onExportPDFClicked();
    } else if (reply == QMessageBox::No) {
        onExportDOCXClicked();
    } else {
        onSaveTXTClicked();
    }
}

void MainWindow::onQuit() {
    close();
}

void MainWindow::onManageModels() {
    if (!m_modelManager) {
        m_modelManager = new ModelManager(this);
        connect(m_modelManager, &ModelManager::modelsChanged,
                this, &MainWindow::onModelsUpdated);
    }
    
    m_modelManager->exec();
}

void MainWindow::onSettings() {
    if (!m_settingsDialog) {
        m_settingsDialog = new SettingsDialog(this);
    }
    
    m_settingsDialog->exec();
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "About Speech Recorder",
        "<h2>Speech Recorder v1.0</h2>"
        "<p>Professional speech-to-text application for Linux</p>"
        "<p><b>Features:</b></p>"
        "<ul>"
        "<li>Real-time audio recording with PulseAudio</li>"
        "<li>Whisper & Vosk transcription engines</li>"
        "<li>Export to TXT, DOCX, and PDF</li>"
        "</ul>"
        "<p><b>Made with ♥ by SparklyLabz</b></p>"
        "<p><a href='https://sparklylabz.com'>sparklylabz.com</a></p>"
        "<p>C++17 • Qt5 • whisper.cpp • Vosk</p>");
}

void MainWindow::onModelChanged(const QString& modelName) {
    if (!m_modelSelector->isModelDownloaded(modelName)) {
        QMessageBox::information(this, "Model Not Downloaded",
            QString("The model '%1' is not downloaded yet.\n\n"
                    "Use Tools > Manage Models to download it.")
            .arg(modelName));
        return;
    }
    
    loadTranscriber(modelName);
}

void MainWindow::onModelsUpdated() {
    // Refresh model selector
    m_modelSelector->refreshAvailableModels();
}

void MainWindow::setStatus(const QString& status) {
    m_statusLabel->setText(status);
}
