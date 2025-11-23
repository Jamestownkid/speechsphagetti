#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QTabWidget;
class QComboBox;
class QSpinBox;
class QCheckBox;
class QLineEdit;

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    
private slots:
    void onApply();
    void onReset();
    
private:
    void setupUI();
    void loadSettings();
    void saveSettings();
    
    // UI components
    QTabWidget* m_tabs;
    
    // Audio tab
    QComboBox* m_inputDeviceCombo;
    QComboBox* m_sampleRateCombo;
    QCheckBox* m_noiseGateCheck;
    
    // Model tab
    QComboBox* m_defaultModelCombo;
    QCheckBox* m_keepLoadedCheck;
    QComboBox* m_languageCombo;
    
    // Interface tab
    QComboBox* m_themeCombo;
    QSpinBox* m_fontSizeSpin;
    QCheckBox* m_showConfidenceCheck;
    
    // Advanced tab
    QCheckBox* m_autoSaveCheck;
    QComboBox* m_logLevelCombo;
    QLineEdit* m_modelDirEdit;
};

#endif // SETTINGSDIALOG_H

