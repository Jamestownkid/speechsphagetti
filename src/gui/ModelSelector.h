#ifndef MODELSELECTOR_H
#define MODELSELECTOR_H

#include <QComboBox>
#include <QMap>

class ModelSelector : public QComboBox {
    Q_OBJECT

public:
    explicit ModelSelector(QWidget* parent = nullptr);
    
    void refreshAvailableModels();
    QString selectedModel() const;
    bool isModelDownloaded(const QString& modelName) const;
    
signals:
    void modelChanged(const QString& modelName);
    
private slots:
    void onCurrentIndexChanged(int index);
    
private:
    void updateModelList();
    QString getModelDisplayName(const QString& modelName, bool isDownloaded) const;
    
    QMap<QString, bool> m_modelStatus; // model name -> is downloaded
    
    // Model definitions
    struct ModelInfo {
        QString name;
        QString filename;
        QString downloadUrl;
        int sizeMB;
    };
    
    static const QList<ModelInfo> AVAILABLE_MODELS;
};

#endif // MODELSELECTOR_H
