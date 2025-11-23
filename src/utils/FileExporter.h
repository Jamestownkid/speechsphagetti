#ifndef FILEEXPORTER_H
#define FILEEXPORTER_H

#include <QString>

class FileExporter {
public:
    // Export functions
    static bool exportToTXT(const QString& text, const QString& filepath);
    static bool exportToDOCX(const QString& text, const QString& filepath);
    static bool exportToPDF(const QString& text, const QString& filepath);
    static void copyToClipboard(const QString& text);
    
private:
    static QString wrapDOCXText(const QString& text);
};

#endif // FILEEXPORTER_H
