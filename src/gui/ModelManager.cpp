#include "ModelManager.h"
#include "../utils/Settings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QDebug>

const QList<ModelManager::ModelInfo> ModelManager::MODELS = {
    {"Whisper Tiny", "ggml-tiny.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-tiny.bin", 75},
    {"Whisper Tiny En", "ggml-tiny.en.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-tiny.en.bin", 75},
    {"Whisper Base", "ggml-base.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.bin", 142},
    {"Whisper Base En", "ggml-base.en.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.en.bin", 142},
    {"Whisper Small", "ggml-small.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-small.bin", 466},
    {"Whisper Small En", "ggml-small.en.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-small.en.bin", 466},
    {"Whisper Medium", "ggml-medium.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-medium.bin", 1500},
    {"Whisper Medium En", "ggml-medium.en.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-medium.en.bin", 1500},
    {"Whisper Large V1", "ggml-large-v1.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-large-v1.bin", 2900},
    {"Whisper Large V2", "ggml-large-v2.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-large-v2.bin", 2900},
    {"Whisper Large V3", "ggml-large-v3.bin", "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-large-v3.bin", 2900},
    {"Vosk Small", "vosk-model-small-en-us-0.15.zip", "https://alphacephei.com/vosk/models/vosk-model-small-en-us-0.15.zip", 40},
    {"Vosk Large", "vosk-model-en-us-0.22.zip", "https://alphacephei.com/vosk/models/vosk-model-en-us-0.22.zip", 1800}
};

ModelManager::ModelManager(QWidget* parent)
    : QDialog(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_currentDownload(nullptr)
    , m_downloadingRow(-1)
    , m_downloadProgressBar(nullptr) {
    
    setWindowTitle("Manage Models - Speech Recorder");
    resize(800, 500);
    
    m_modelDirectory = Settings::instance().modelDirectory();
    
    setupUI();
    refreshModelList();
}

ModelManager::~ModelManager() {
    if (m_currentDownload) {
        m_currentDownload->abort();
    }
}

void ModelManager::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("<h2>Speech Recognition Models</h2>");
    mainLayout->addWidget(titleLabel);
    
    QLabel* infoLabel = new QLabel("Download models to enable different transcription engines. "
                                  "Whisper models are more accurate, Vosk models are faster.");
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #888; margin-bottom: 10px;");
    mainLayout->addWidget(infoLabel);
    
    // Model table
    m_modelTable = new QTableWidget(MODELS.size(), 5);
    m_modelTable->setHorizontalHeaderLabels({"Model Name", "Size", "Status", "Action", "Progress"});
    m_modelTable->horizontalHeader()->setStretchLastSection(true);
    m_modelTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_modelTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_modelTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_modelTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_modelTable->verticalHeader()->setVisible(false);
    m_modelTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_modelTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_modelTable);
    
    // Storage info
    m_storageLabel = new QLabel();
    m_storageLabel->setStyleSheet("color: #888; font-size: 11px;");
    mainLayout->addWidget(m_storageLabel);
    updateStorageInfo();
    
    // Close button
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    m_closeButton = new QPushButton("Close");
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(m_closeButton);
    mainLayout->addLayout(buttonLayout);
}

void ModelManager::refreshModelList() {
    // Ensure model directory exists
    QDir dir(m_modelDirectory);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    for (int row = 0; row < MODELS.size(); ++row) {
        const ModelInfo& model = MODELS[row];
        
        // Model name
        m_modelTable->setItem(row, 0, new QTableWidgetItem(model.name));
        
        // Size
        m_modelTable->setItem(row, 1, new QTableWidgetItem(QString("%1 MB").arg(model.sizeMB)));
        
        // Status
        bool downloaded = isModelDownloaded(model.filename);
        QTableWidgetItem* statusItem = new QTableWidgetItem(downloaded ? "✓ Ready" : "Not Downloaded");
        statusItem->setForeground(downloaded ? QColor(Qt::green) : QColor(Qt::gray));
        m_modelTable->setItem(row, 2, statusItem);
        
        // Action button
        QPushButton* actionBtn = new QPushButton();
        if (downloaded) {
            actionBtn->setText("Remove");
            actionBtn->setStyleSheet("background-color: #d32f2f;");
            connect(actionBtn, &QPushButton::clicked, [this, row]() {
                onRemoveClicked(row);
            });
        } else {
            actionBtn->setText("Download");
            actionBtn->setStyleSheet("background-color: #4CAF50;");
            connect(actionBtn, &QPushButton::clicked, [this, row]() {
                onDownloadClicked(row);
            });
        }
        m_modelTable->setCellWidget(row, 3, actionBtn);
        
        // Progress (empty initially)
        m_modelTable->setItem(row, 4, new QTableWidgetItem(""));
    }
    
    updateStorageInfo();
}

void ModelManager::onDownloadClicked(int row) {
    if (m_currentDownload) {
        QMessageBox::warning(this, "Download in Progress", 
                           "Please wait for the current download to complete.");
        return;
    }
    
    const ModelInfo& model = MODELS[row];
    
    auto reply = QMessageBox::question(this, "Download Model",
                                      QString("Download %1 (%2 MB)?\n\nThis may take several minutes.")
                                      .arg(model.name).arg(model.sizeMB),
                                      QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        downloadModel(model.name, model.url, model.filename);
        m_downloadingRow = row;
        
        // Update button to show "Downloading..."
        QPushButton* btn = qobject_cast<QPushButton*>(m_modelTable->cellWidget(row, 3));
        if (btn) {
            btn->setText("Downloading...");
            btn->setEnabled(false);
        }
        
        // Add progress bar
        m_downloadProgressBar = new QProgressBar();
        m_downloadProgressBar->setRange(0, 100);
        m_downloadProgressBar->setValue(0);
        m_modelTable->setCellWidget(row, 4, m_downloadProgressBar);
    }
}

void ModelManager::onRemoveClicked(int row) {
    const ModelInfo& model = MODELS[row];
    
    auto reply = QMessageBox::question(this, "Remove Model",
                                      QString("Remove %1 from disk?\n\nThis will free up %2 MB.")
                                      .arg(model.name).arg(model.sizeMB),
                                      QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        QString filepath = QDir(m_modelDirectory).filePath(model.filename);
        
        // Remove .zip extension if present
        QString actualFilename = model.filename;
        if (actualFilename.endsWith(".zip")) {
            actualFilename = actualFilename.left(actualFilename.length() - 4);
        }
        QString actualPath = QDir(m_modelDirectory).filePath(actualFilename);
        
        bool success = false;
        
        // Try to remove as file
        if (QFile::exists(actualPath)) {
            success = QFile::remove(actualPath);
        }
        // Try to remove as directory
        else if (QDir(actualPath).exists()) {
            QDir dir(actualPath);
            success = dir.removeRecursively();
        }
        
        if (success) {
            QMessageBox::information(this, "Model Removed", 
                                   QString("%1 has been removed.").arg(model.name));
            refreshModelList();
            emit modelsChanged();
        } else {
            QMessageBox::critical(this, "Error", 
                                QString("Failed to remove %1").arg(model.name));
        }
    }
}

void ModelManager::downloadModel(const QString& name, const QString& url, const QString& filename) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "SpeechRecorder/1.0");
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, 
                        QNetworkRequest::NoLessSafeRedirectPolicy);
    
    m_currentDownloadFile = QDir(m_modelDirectory).filePath(filename);
    m_currentDownload = m_networkManager->get(request);
    
    connect(m_currentDownload, &QNetworkReply::downloadProgress,
            this, &ModelManager::onDownloadProgress);
    connect(m_currentDownload, &QNetworkReply::finished,
            this, &ModelManager::onDownloadFinished);
    // Use errorOccurred for Qt 5.15+, error for Qt 5.12
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(m_currentDownload, &QNetworkReply::errorOccurred,
            this, &ModelManager::onDownloadError);
#else
    connect(m_currentDownload, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &ModelManager::onDownloadError);
#endif
    
    qDebug() << "Downloading" << name << "from" << url;
}

void ModelManager::onDownloadProgress(qint64 received, qint64 total) {
    if (m_downloadProgressBar && total > 0) {
        int percentage = static_cast<int>((received * 100) / total);
        m_downloadProgressBar->setValue(percentage);
        
        // Update progress text
        QString progressText = QString("%1 / %2")
            .arg(formatSize(received))
            .arg(formatSize(total));
        m_modelTable->item(m_downloadingRow, 4)->setText(progressText);
    }
}

void ModelManager::onDownloadFinished() {
    if (!m_currentDownload) return;
    
    if (m_currentDownload->error() == QNetworkReply::NoError) {
        // Save downloaded data
        QFile file(m_currentDownloadFile);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(m_currentDownload->readAll());
            file.close();
            
            qDebug() << "Download completed:" << m_currentDownloadFile;
            
            // Extract if it's a zip file
            if (m_currentDownloadFile.endsWith(".zip")) {
                extractZipIfNeeded(m_currentDownloadFile);
            }
            
            QMessageBox::information(this, "Download Complete",
                                   QString("Model downloaded successfully to:\n%1")
                                   .arg(m_currentDownloadFile));
            
            refreshModelList();
            emit modelsChanged();
        } else {
            QMessageBox::critical(this, "Error", 
                                QString("Failed to save file: %1").arg(file.errorString()));
        }
    }
    
    m_currentDownload->deleteLater();
    m_currentDownload = nullptr;
    m_downloadingRow = -1;
}

void ModelManager::onDownloadError() {
    if (m_currentDownload && m_currentDownload->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Download Error",
                            QString("Download failed: %1").arg(m_currentDownload->errorString()));
        
        m_currentDownload->deleteLater();
        m_currentDownload = nullptr;
        
        // Refresh to restore buttons
        refreshModelList();
    }
}

void ModelManager::extractZipIfNeeded(const QString& filepath) {
    // Extract zip file using system unzip command
    QFileInfo fileInfo(filepath);
    QString outputDir = fileInfo.dir().path();
    
    QProcess unzip;
    unzip.setWorkingDirectory(outputDir);
    
    QStringList args;
    args << "-o" << filepath << "-d" << outputDir;
    
    qDebug() << "Extracting:" << filepath;
    unzip.start("unzip", args);
    unzip.waitForFinished(300000); // 5 minute timeout
    
    if (unzip.exitCode() == 0) {
        qDebug() << "Extraction successful";
        // Remove zip file after extraction
        QFile::remove(filepath);
    } else {
        qWarning() << "Extraction failed:" << unzip.readAllStandardError();
        QMessageBox::warning(this, "Extraction Warning",
                           "Downloaded zip file but extraction failed. "
                           "You may need to extract it manually with: unzip " + filepath);
    }
}

bool ModelManager::isModelDownloaded(const QString& filename) const {
    // Remove .zip extension if present
    QString actualFilename = filename;
    if (actualFilename.endsWith(".zip")) {
        actualFilename = actualFilename.left(actualFilename.length() - 4);
    }
    
    QString filepath = QDir(m_modelDirectory).filePath(actualFilename);
    
    // Check as file or directory
    return QFile::exists(filepath) || QDir(filepath).exists();
}

qint64 ModelManager::getModelSize(const QString& filename) const {
    QString filepath = QDir(m_modelDirectory).filePath(filename);
    
    QFileInfo info(filepath);
    if (info.isFile()) {
        return info.size();
    } else if (info.isDir()) {
        // Calculate directory size
        qint64 totalSize = 0;
        QDir dir(filepath);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        
        for (const QFileInfo& fileInfo : list) {
            if (fileInfo.isFile()) {
                totalSize += fileInfo.size();
            } else if (fileInfo.isDir()) {
                totalSize += getModelSize(fileInfo.filePath());
            }
        }
        return totalSize;
    }
    
    return 0;
}

void ModelManager::updateStorageInfo() {
    qint64 totalSize = 0;
    int downloadedCount = 0;
    
    for (const ModelInfo& model : MODELS) {
        if (isModelDownloaded(model.filename)) {
            totalSize += getModelSize(model.filename);
            downloadedCount++;
        }
    }
    
    m_storageLabel->setText(QString("Models stored in: %1 | %2 models downloaded | %3 used")
                           .arg(m_modelDirectory)
                           .arg(downloadedCount)
                           .arg(formatSize(totalSize)));
}

QString ModelManager::formatSize(qint64 bytes) const {
    if (bytes < 1024) {
        return QString("%1 B").arg(bytes);
    } else if (bytes < 1024 * 1024) {
        return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    } else if (bytes < 1024 * 1024 * 1024) {
        return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    } else {
        return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
    }
}

