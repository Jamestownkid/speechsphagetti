#include "ModelSelector.h"
#include "../utils/Settings.h"
#include <QDir>
#include <QFile>
#include <QDebug>

// Define available models
const QList<ModelSelector::ModelInfo> ModelSelector::AVAILABLE_MODELS = {
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
    {"Vosk Small", "vosk-model-small-en-us-0.15", "https://alphacephei.com/vosk/models/vosk-model-small-en-us-0.15.zip", 40},
    {"Vosk Large", "vosk-model-en-us-0.22", "https://alphacephei.com/vosk/models/vosk-model-en-us-0.22.zip", 1800}
};

ModelSelector::ModelSelector(QWidget* parent)
    : QComboBox(parent) {
    
    // Set styling
    setMinimumWidth(200);
    setToolTip("Select speech recognition model");
    
    // Connect selection change
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ModelSelector::onCurrentIndexChanged);
    
    // Initial population
    refreshAvailableModels();
}

void ModelSelector::refreshAvailableModels() {
    // Get model directory
    QString modelDir = Settings::instance().modelDirectory();
    QDir dir(modelDir);
    
    // Check which models are downloaded
    m_modelStatus.clear();
    for (const ModelInfo& model : AVAILABLE_MODELS) {
        QString modelPath = dir.filePath(model.filename);
        
        // Check if file or directory exists
        bool exists = QFile::exists(modelPath) || QDir(modelPath).exists();
        m_modelStatus[model.name] = exists;
        
        qDebug() << "Model" << model.name << "status:" << (exists ? "Downloaded" : "Not downloaded");
    }
    
    updateModelList();
}

void ModelSelector::updateModelList() {
    // Store current selection
    QString currentSelection = currentText().split(" (").first();
    
    // Clear and repopulate
    clear();
    
    int indexToSelect = -1;
    int currentIndex = 0;
    
    for (const ModelInfo& model : AVAILABLE_MODELS) {
        bool isDownloaded = m_modelStatus.value(model.name, false);
        QString displayName = getModelDisplayName(model.name, isDownloaded);
        
        addItem(displayName, model.name);
        
        // Disable if not downloaded
        if (!isDownloaded) {
            setItemData(currentIndex, QColor(Qt::gray), Qt::ForegroundRole);
        }
        
        // Remember index if this was previously selected
        if (model.name == currentSelection) {
            indexToSelect = currentIndex;
        }
        
        currentIndex++;
    }
    
    // Restore selection or select first available model
    if (indexToSelect >= 0 && m_modelStatus.value(AVAILABLE_MODELS[indexToSelect].name, false)) {
        setCurrentIndex(indexToSelect);
    } else {
        // Select first downloaded model
        for (int i = 0; i < count(); ++i) {
            QString modelName = itemData(i).toString();
            if (m_modelStatus.value(modelName, false)) {
                setCurrentIndex(i);
                break;
            }
        }
    }
}

QString ModelSelector::getModelDisplayName(const QString& modelName, bool isDownloaded) const {
    if (isDownloaded) {
        return QString("%1 ✓").arg(modelName);
    } else {
        return QString("%1 (Download Required)").arg(modelName);
    }
}

QString ModelSelector::selectedModel() const {
    return currentData().toString();
}

bool ModelSelector::isModelDownloaded(const QString& modelName) const {
    return m_modelStatus.value(modelName, false);
}

void ModelSelector::onCurrentIndexChanged(int index) {
    if (index < 0) return;
    
    QString modelName = itemData(index).toString();
    
    // Check if model is actually downloaded
    if (!m_modelStatus.value(modelName, false)) {
        // User selected a non-downloaded model - this shouldn't happen normally
        // but we'll emit the signal anyway to let MainWindow handle it
        qWarning() << "User selected non-downloaded model:" << modelName;
    }
    
    emit modelChanged(modelName);
}

