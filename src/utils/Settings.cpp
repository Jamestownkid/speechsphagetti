#include "Settings.h"
#include <QStandardPaths>
#include <QDir>

Settings::Settings() 
    : m_settings(ORG_NAME, APP_NAME) {
    // Initialize default model directory if not set
    if (!m_settings.contains("modelDirectory")) {
        QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QString modelDir = QDir(dataDir).filePath("models");
        m_settings.setValue("modelDirectory", modelDir);
        
        // Create directory if it doesn't exist
        QDir().mkpath(modelDir);
    }
}

// Audio settings
QString Settings::inputDevice() const {
    return m_settings.value("audio/inputDevice", "default").toString();
}

void Settings::setInputDevice(const QString& device) {
    m_settings.setValue("audio/inputDevice", device);
}

int Settings::sampleRate() const {
    return m_settings.value("audio/sampleRate", 16000).toInt();
}

void Settings::setSampleRate(int rate) {
    m_settings.setValue("audio/sampleRate", rate);
}

bool Settings::noiseGateEnabled() const {
    return m_settings.value("audio/noiseGate", false).toBool();
}

void Settings::setNoiseGateEnabled(bool enabled) {
    m_settings.setValue("audio/noiseGate", enabled);
}

// Model settings
QString Settings::defaultModel() const {
    return m_settings.value("model/default", "Whisper Base").toString();
}

void Settings::setDefaultModel(const QString& model) {
    m_settings.setValue("model/default", model);
}

bool Settings::keepModelLoaded() const {
    return m_settings.value("model/keepLoaded", true).toBool();
}

void Settings::setKeepModelLoaded(bool keep) {
    m_settings.setValue("model/keepLoaded", keep);
}

QString Settings::languageOverride() const {
    return m_settings.value("model/language", "en").toString();
}

void Settings::setLanguageOverride(const QString& lang) {
    m_settings.setValue("model/language", lang);
}

// Interface settings
QString Settings::theme() const {
    return m_settings.value("interface/theme", "dark").toString();
}

void Settings::setTheme(const QString& theme) {
    m_settings.setValue("interface/theme", theme);
}

int Settings::fontSize() const {
    return m_settings.value("interface/fontSize", 14).toInt();
}

void Settings::setFontSize(int size) {
    m_settings.setValue("interface/fontSize", size);
}

bool Settings::showConfidence() const {
    return m_settings.value("interface/showConfidence", false).toBool();
}

void Settings::setShowConfidence(bool show) {
    m_settings.setValue("interface/showConfidence", show);
}

// Advanced settings
bool Settings::autoSaveDrafts() const {
    return m_settings.value("advanced/autoSave", false).toBool();
}

void Settings::setAutoSaveDrafts(bool autoSave) {
    m_settings.setValue("advanced/autoSave", autoSave);
}

int Settings::logLevel() const {
    return m_settings.value("advanced/logLevel", 1).toInt();
}

void Settings::setLogLevel(int level) {
    m_settings.setValue("advanced/logLevel", level);
}

QString Settings::modelDirectory() const {
    return m_settings.value("modelDirectory").toString();
}

