#include "SettingsDialog.h"
#include "../utils/Settings.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Settings");
    resize(600, 500);
    setupUI();
    loadSettings();
}

void SettingsDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Create tab widget
    m_tabs = new QTabWidget(this);
    
    // ===== AUDIO TAB =====
    QWidget* audioTab = new QWidget();
    QFormLayout* audioLayout = new QFormLayout(audioTab);
    
    m_inputDeviceCombo = new QComboBox();
    m_inputDeviceCombo->addItem("Default");
    m_inputDeviceCombo->addItem("PulseAudio Default");
    audioLayout->addRow("Input Device:", m_inputDeviceCombo);
    
    m_sampleRateCombo = new QComboBox();
    m_sampleRateCombo->addItem("16000 Hz (Recommended)", 16000);
    m_sampleRateCombo->addItem("44100 Hz", 44100);
    m_sampleRateCombo->addItem("48000 Hz", 48000);
    audioLayout->addRow("Sample Rate:", m_sampleRateCombo);
    
    m_noiseGateCheck = new QCheckBox("Enable noise gate (reduce background noise)");
    audioLayout->addRow("", m_noiseGateCheck);
    
    audioLayout->addRow(new QLabel("<i>Note: Changes require app restart</i>"));
    
    m_tabs->addTab(audioTab, "Audio");
    
    // ===== MODEL TAB =====
    QWidget* modelTab = new QWidget();
    QFormLayout* modelLayout = new QFormLayout(modelTab);
    
    m_defaultModelCombo = new QComboBox();
    m_defaultModelCombo->addItem("Whisper Tiny");
    m_defaultModelCombo->addItem("Whisper Base");
    m_defaultModelCombo->addItem("Whisper Small");
    m_defaultModelCombo->addItem("Vosk Small");
    m_defaultModelCombo->addItem("Vosk Large");
    modelLayout->addRow("Default Model:", m_defaultModelCombo);
    
    m_keepLoadedCheck = new QCheckBox("Keep model loaded in memory");
    m_keepLoadedCheck->setChecked(true);
    modelLayout->addRow("", m_keepLoadedCheck);
    
    m_languageCombo = new QComboBox();
    m_languageCombo->addItem("English", "en");
    m_languageCombo->addItem("Spanish", "es");
    m_languageCombo->addItem("French", "fr");
    m_languageCombo->addItem("German", "de");
    m_languageCombo->addItem("Auto-detect", "auto");
    modelLayout->addRow("Language:", m_languageCombo);
    
    modelLayout->addRow(new QLabel("<i>Whisper supports 99+ languages</i>"));
    
    m_tabs->addTab(modelTab, "Models");
    
    // ===== INTERFACE TAB =====
    QWidget* interfaceTab = new QWidget();
    QFormLayout* interfaceLayout = new QFormLayout(interfaceTab);
    
    m_themeCombo = new QComboBox();
    m_themeCombo->addItem("Dark Theme", "dark");
    m_themeCombo->addItem("Light Theme", "light");
    m_themeCombo->addItem("System Default", "system");
    interfaceLayout->addRow("Theme:", m_themeCombo);
    
    m_fontSizeSpin = new QSpinBox();
    m_fontSizeSpin->setRange(8, 24);
    m_fontSizeSpin->setValue(14);
    m_fontSizeSpin->setSuffix(" pt");
    interfaceLayout->addRow("Font Size:", m_fontSizeSpin);
    
    m_showConfidenceCheck = new QCheckBox("Show transcription confidence scores");
    interfaceLayout->addRow("", m_showConfidenceCheck);
    
    interfaceLayout->addRow(new QLabel("<i>Theme changes require restart</i>"));
    
    m_tabs->addTab(interfaceTab, "Interface");
    
    // ===== ADVANCED TAB =====
    QWidget* advancedTab = new QWidget();
    QFormLayout* advancedLayout = new QFormLayout(advancedTab);
    
    m_autoSaveCheck = new QCheckBox("Automatically save draft transcriptions");
    advancedLayout->addRow("Auto-Save:", m_autoSaveCheck);
    
    m_logLevelCombo = new QComboBox();
    m_logLevelCombo->addItem("Errors Only", 0);
    m_logLevelCombo->addItem("Warnings", 1);
    m_logLevelCombo->addItem("Info", 2);
    m_logLevelCombo->addItem("Debug", 3);
    advancedLayout->addRow("Log Level:", m_logLevelCombo);
    
    QHBoxLayout* modelDirLayout = new QHBoxLayout();
    m_modelDirEdit = new QLineEdit();
    m_modelDirEdit->setReadOnly(true);
    QPushButton* browseBtn = new QPushButton("Browse...");
    connect(browseBtn, &QPushButton::clicked, [this]() {
        QString dir = QFileDialog::getExistingDirectory(this, "Select Model Directory", 
                                                       m_modelDirEdit->text());
        if (!dir.isEmpty()) {
            m_modelDirEdit->setText(dir);
        }
    });
    modelDirLayout->addWidget(m_modelDirEdit);
    modelDirLayout->addWidget(browseBtn);
    advancedLayout->addRow("Model Directory:", modelDirLayout);
    
    m_tabs->addTab(advancedTab, "Advanced");
    
    mainLayout->addWidget(m_tabs);
    
    // ===== BUTTON ROW =====
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    QPushButton* resetBtn = new QPushButton("Reset to Defaults");
    connect(resetBtn, &QPushButton::clicked, this, &SettingsDialog::onReset);
    buttonLayout->addWidget(resetBtn);
    
    buttonLayout->addStretch();
    
    QPushButton* cancelBtn = new QPushButton("Cancel");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelBtn);
    
    QPushButton* applyBtn = new QPushButton("Apply");
    applyBtn->setDefault(true);
    connect(applyBtn, &QPushButton::clicked, this, &SettingsDialog::onApply);
    buttonLayout->addWidget(applyBtn);
    
    mainLayout->addLayout(buttonLayout);
}

void SettingsDialog::loadSettings() {
    Settings& settings = Settings::instance();
    
    // Audio
    m_inputDeviceCombo->setCurrentText(settings.inputDevice());
    int sampleRate = settings.sampleRate();
    for (int i = 0; i < m_sampleRateCombo->count(); ++i) {
        if (m_sampleRateCombo->itemData(i).toInt() == sampleRate) {
            m_sampleRateCombo->setCurrentIndex(i);
            break;
        }
    }
    m_noiseGateCheck->setChecked(settings.noiseGateEnabled());
    
    // Models
    m_defaultModelCombo->setCurrentText(settings.defaultModel());
    m_keepLoadedCheck->setChecked(settings.keepModelLoaded());
    QString lang = settings.languageOverride();
    for (int i = 0; i < m_languageCombo->count(); ++i) {
        if (m_languageCombo->itemData(i).toString() == lang) {
            m_languageCombo->setCurrentIndex(i);
            break;
        }
    }
    
    // Interface
    QString theme = settings.theme();
    for (int i = 0; i < m_themeCombo->count(); ++i) {
        if (m_themeCombo->itemData(i).toString() == theme) {
            m_themeCombo->setCurrentIndex(i);
            break;
        }
    }
    m_fontSizeSpin->setValue(settings.fontSize());
    m_showConfidenceCheck->setChecked(settings.showConfidence());
    
    // Advanced
    m_autoSaveCheck->setChecked(settings.autoSaveDrafts());
    m_logLevelCombo->setCurrentIndex(settings.logLevel());
    m_modelDirEdit->setText(settings.modelDirectory());
}

void SettingsDialog::saveSettings() {
    Settings& settings = Settings::instance();
    
    // Audio
    settings.setInputDevice(m_inputDeviceCombo->currentText());
    settings.setSampleRate(m_sampleRateCombo->currentData().toInt());
    settings.setNoiseGateEnabled(m_noiseGateCheck->isChecked());
    
    // Models
    settings.setDefaultModel(m_defaultModelCombo->currentText());
    settings.setKeepModelLoaded(m_keepLoadedCheck->isChecked());
    settings.setLanguageOverride(m_languageCombo->currentData().toString());
    
    // Interface
    settings.setTheme(m_themeCombo->currentData().toString());
    settings.setFontSize(m_fontSizeSpin->value());
    settings.setShowConfidence(m_showConfidenceCheck->isChecked());
    
    // Advanced
    settings.setAutoSaveDrafts(m_autoSaveCheck->isChecked());
    settings.setLogLevel(m_logLevelCombo->currentIndex());
}

void SettingsDialog::onApply() {
    saveSettings();
    QMessageBox::information(this, "Settings Saved", 
                           "Settings have been saved. Some changes may require restarting the app.");
    accept();
}

void SettingsDialog::onReset() {
    auto reply = QMessageBox::question(this, "Reset Settings",
                                      "Are you sure you want to reset all settings to defaults?",
                                      QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        Settings& settings = Settings::instance();
        
        // Reset to defaults (just reload default values)
        settings.setInputDevice("default");
        settings.setSampleRate(16000);
        settings.setNoiseGateEnabled(false);
        settings.setDefaultModel("Whisper Base");
        settings.setKeepModelLoaded(true);
        settings.setLanguageOverride("en");
        settings.setTheme("dark");
        settings.setFontSize(14);
        settings.setShowConfidence(false);
        settings.setAutoSaveDrafts(false);
        settings.setLogLevel(1);
        
        loadSettings();
        QMessageBox::information(this, "Reset Complete", "Settings have been reset to defaults.");
    }
}

