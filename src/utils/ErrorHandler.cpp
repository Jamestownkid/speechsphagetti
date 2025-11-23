#include "ErrorHandler.h"
#include <QMessageBox>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QStorageInfo>
#include <QSysInfo>
#include <QDebug>

void ErrorHandler::show(QWidget* parent, ErrorLevel level, const QString& title, const QString& message) {
    switch (level) {
        case INFO:
            QMessageBox::information(parent, title, message);
            break;
        case WARNING:
            QMessageBox::warning(parent, title, message);
            break;
        case ERROR:
        case CRITICAL:
            QMessageBox::critical(parent, title, message);
            break;
    }
}

void ErrorHandler::showPulseAudioError(QWidget* parent, const QString& details) {
    QString diagnostics = getPulseAudioDiagnostics();
    
    QString message = QString(
        "<h3>Audio System Error</h3>"
        "<p><b>Problem:</b> %1</p>"
        "<p><b>Possible causes:</b></p>"
        "<ul>"
        "<li>PulseAudio not running</li>"
        "<li>No microphone connected</li>"
        "<li>Microphone permissions denied</li>"
        "</ul>"
        "<p><b>Troubleshooting:</b></p>"
        "<pre>%2</pre>"
        "<p>Visit <a href='https://sparklylabz.com/speech-recorder/troubleshooting'>our troubleshooting guide</a></p>"
    ).arg(details).arg(diagnostics);
    
    QMessageBox msgBox(parent);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("Audio Error");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.exec();
}

void ErrorHandler::showModelLoadError(QWidget* parent, const QString& modelName, const QString& details) {
    QString message = QString(
        "<h3>Failed to Load Model: %1</h3>"
        "<p><b>Error:</b> %2</p>"
        "<p><b>Solutions:</b></p>"
        "<ul>"
        "<li>Download the model using <b>Tools → Manage Models</b></li>"
        "<li>Check if model file is corrupted (try re-downloading)</li>"
        "<li>Ensure sufficient RAM (see model requirements)</li>"
        "</ul>"
        "<p><b>Model requirements:</b> See Help → About for details</p>"
    ).arg(modelName).arg(details);
    
    QMessageBox msgBox(parent);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("Model Load Error");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.exec();
}

void ErrorHandler::showTranscriptionError(QWidget* parent, const QString& details) {
    QString message = QString(
        "<h3>Transcription Failed</h3>"
        "<p><b>Error:</b> %1</p>"
        "<p><b>Possible causes:</b></p>"
        "<ul>"
        "<li>Audio recording was too short or empty</li>"
        "<li>Model crashed or ran out of memory</li>"
        "<li>Audio format incompatible</li>"
        "</ul>"
        "<p><b>Try:</b></p>"
        "<ul>"
        "<li>Record at least 1 second of clear speech</li>"
        "<li>Check microphone levels (should show green bars)</li>"
        "<li>Use a smaller model if experiencing crashes</li>"
        "</ul>"
    ).arg(details);
    
    QMessageBox msgBox(parent);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Transcription Error");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.exec();
}

void ErrorHandler::showFileError(QWidget* parent, const QString& operation, const QString& filename) {
    QString message = QString(
        "<h3>File Operation Failed</h3>"
        "<p><b>Operation:</b> %1</p>"
        "<p><b>File:</b> %2</p>"
        "<p><b>Possible causes:</b></p>"
        "<ul>"
        "<li>No permission to write to this location</li>"
        "<li>Disk full</li>"
        "<li>File in use by another program</li>"
        "</ul>"
        "<p><b>Try:</b></p>"
        "<ul>"
        "<li>Save to your home directory instead</li>"
        "<li>Check disk space: <tt>df -h</tt></li>"
        "<li>Close other programs using this file</li>"
        "</ul>"
    ).arg(operation).arg(filename);
    
    QMessageBox msgBox(parent);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("File Error");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.exec();
}

void ErrorHandler::showMemoryWarning(QWidget* parent, int requiredMB, int availableMB) {
    QString message = QString(
        "<h3>Insufficient Memory</h3>"
        "<p><b>Required:</b> %1 MB</p>"
        "<p><b>Available:</b> %2 MB</p>"
        "<p><b>Recommendations:</b></p>"
        "<ul>"
        "<li>Close other applications</li>"
        "<li>Use a smaller model (Whisper Tiny or Vosk Small)</li>"
        "<li>Add more RAM to your system</li>"
        "</ul>"
        "<p>The application may crash or freeze if you proceed.</p>"
    ).arg(requiredMB).arg(availableMB);
    
    QMessageBox msgBox(parent);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Memory Warning");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.exec();
}

QString ErrorHandler::getPulseAudioDiagnostics() {
    QString result;
    
    // Check if pulseaudio is running
    QProcess ps;
    ps.start("pgrep", QStringList() << "-x" << "pulseaudio");
    ps.waitForFinished(1000);
    
    if (ps.exitCode() != 0) {
        result += "❌ PulseAudio is not running\n";
        result += "   Start with: pulseaudio --start\n\n";
    } else {
        result += "✓ PulseAudio is running\n\n";
    }
    
    // Try to get pactl info
    QProcess pactl;
    pactl.start("pactl", QStringList() << "info");
    pactl.waitForFinished(2000);
    
    if (pactl.exitCode() == 0) {
        result += "✓ PulseAudio responding\n\n";
        
        // List sources
        QProcess sources;
        sources.start("pactl", QStringList() << "list" << "sources" << "short");
        sources.waitForFinished(2000);
        
        if (sources.exitCode() == 0) {
            QString sourceOutput = sources.readAllStandardOutput();
            if (!sourceOutput.isEmpty()) {
                result += "Available audio sources:\n";
                result += sourceOutput;
            } else {
                result += "❌ No audio sources found\n";
                result += "   Check microphone connection\n";
            }
        }
    } else {
        result += "❌ Cannot communicate with PulseAudio\n";
        result += "   Error: " + pactl.readAllStandardError() + "\n";
    }
    
    return result;
}

QString ErrorHandler::getSystemInfo() {
    QString info;
    
    info += "OS: " + QSysInfo::prettyProductName() + "\n";
    info += "Kernel: " + QSysInfo::kernelVersion() + "\n";
    info += "Arch: " + QSysInfo::currentCpuArchitecture() + "\n";
    
    // RAM
    qint64 ramMB = getAvailableRAM();
    if (ramMB > 0) {
        info += QString("Available RAM: %1 MB\n").arg(ramMB);
    }
    
    // Qt version
    info += "Qt version: " + QString(qVersion()) + "\n";
    
    return info;
}

bool ErrorHandler::checkDiskSpace(const QString& path, qint64 requiredBytes) {
    QStorageInfo storage(path);
    
    if (storage.isValid()) {
        qint64 available = storage.bytesAvailable();
        return available >= requiredBytes;
    }
    
    return false; // Assume insufficient if we can't check
}

qint64 ErrorHandler::getAvailableRAM() {
    QFile meminfo("/proc/meminfo");
    
    if (!meminfo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return -1;
    }
    
    QTextStream in(&meminfo);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("MemAvailable:")) {
            QStringList parts = line.split(QRegExp("\\s+"));
            if (parts.size() >= 2) {
                qint64 kb = parts[1].toLongLong();
                return kb / 1024; // Convert to MB
            }
        }
    }
    
    return -1;
}

