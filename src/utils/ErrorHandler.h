#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <QString>
#include <QWidget>

class ErrorHandler {
public:
    enum ErrorLevel {
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };
    
    static void show(QWidget* parent, ErrorLevel level, const QString& title, const QString& message);
    static void showPulseAudioError(QWidget* parent, const QString& details);
    static void showModelLoadError(QWidget* parent, const QString& modelName, const QString& details);
    static void showTranscriptionError(QWidget* parent, const QString& details);
    static void showFileError(QWidget* parent, const QString& operation, const QString& filename);
    static void showMemoryWarning(QWidget* parent, int requiredMB, int availableMB);
    
    // Diagnostic helpers
    static QString getPulseAudioDiagnostics();
    static QString getSystemInfo();
    static bool checkDiskSpace(const QString& path, qint64 requiredBytes);
    static qint64 getAvailableRAM();
    
private:
    ErrorHandler() = default;
};

#endif // ERRORHANDLER_H

