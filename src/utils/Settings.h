#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

class Settings {
public:
    static Settings& instance() {
        static Settings s;
        return s;
    }
    
    // Audio settings
    QString inputDevice() const;
    void setInputDevice(const QString& device);
    
    int sampleRate() const;
    void setSampleRate(int rate);
    
    bool noiseGateEnabled() const;
    void setNoiseGateEnabled(bool enabled);
    
    // Model settings
    QString defaultModel() const;
    void setDefaultModel(const QString& model);
    
    bool keepModelLoaded() const;
    void setKeepModelLoaded(bool keep);
    
    QString languageOverride() const;
    void setLanguageOverride(const QString& lang);
    
    // Interface settings
    QString theme() const;
    void setTheme(const QString& theme);
    
    int fontSize() const;
    void setFontSize(int size);
    
    bool showConfidence() const;
    void setShowConfidence(bool show);
    
    // Advanced settings
    bool autoSaveDrafts() const;
    void setAutoSaveDrafts(bool autoSave);
    
    int logLevel() const;
    void setLogLevel(int level);
    
    // Model directory
    QString modelDirectory() const;
    
private:
    Settings();
    QSettings m_settings;
    
    static constexpr const char* ORG_NAME = "SparklyLabz";
    static constexpr const char* APP_NAME = "SpeechRecorder";
};

#endif // SETTINGS_H
