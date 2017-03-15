#include "imageviewer.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(ImageViewer::tr("Pixel Sorter"));
    ImageViewer imageViewer;
    imageViewer.show();

    return app.exec();
}
