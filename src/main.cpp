#include <QApplication>
#include "MainWindow.h"
#include <QStyleFactory>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 1a. set app metadata
    app.setApplicationName("Speech Recorder");
    app.setOrganizationName("SparklyLabz");
    app.setOrganizationDomain("sparklylabz.com");
    
    // 1b. set dark theme by default (looks cleaner)
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);
    
    // 1c. create and show main window
    MainWindow window;
    window.setWindowTitle("Speech Recorder - SparklyLabz");
    window.resize(800, 600);
    window.show();
    
    return app.exec();
}
