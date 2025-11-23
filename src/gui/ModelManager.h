#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QDialog>
#include <QMap>
#include <QNetworkAccessManager>

class QTableWidget;
class QLabel;
class QProgressBar;
class QPushButton;
class QNetworkReply;

class ModelManager : public QDialog {
    Q_OBJECT

public:
    explicit ModelManager(QWidget* parent = nullptr);
    ~ModelManager();
    
signals:
    void modelsChanged(); // Emitted when models are downloaded/removed
    
private slots:
    void onDownloadClicked(int row);
    void onRemoveClicked(int row);
    void onDownloadProgress(qint64 received, qint64 total);
    void onDownloadFinished();
    void onDownloadError();
    
private:
    void setupUI();
    void refreshModelList();
    void updateStorageInfo();
    QString formatSize(qint64 bytes) const;
    bool isModelDownloaded(const QString& filename) const;
    qint64 getModelSize(const QString& filename) const;
    void downloadModel(const QString& name, const QString& url, const QString& filename);
    void extractZipIfNeeded(const QString& filepath);
    
    // UI components
    QTableWidget* m_modelTable;
    QLabel* m_storageLabel;
    QPushButton* m_closeButton;
    
    // Download management
    QNetworkAccessManager* m_networkManager;
    QNetworkReply* m_currentDownload;
    QString m_currentDownloadFile;
    int m_downloadingRow;
    QProgressBar* m_downloadProgressBar;
    
    // Model information
    struct ModelInfo {
        QString name;
        QString filename;
        QString url;
        int sizeMB;
    };
    
    static const QList<ModelInfo> MODELS;
    QString m_modelDirectory;
};

#endif // MODELMANAGER_H
