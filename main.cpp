#include "imageviewer.h"
#include <QApplication>
#include <QCommandLineParser>
#include "ui_imageviewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(ImageViewer::tr("Pixel Sorter"));
    ImageViewer imageViewer;
    imageViewer.show();


//    QMainWindow *widget = new QMainWindow;
//    Ui::ImageViewer ui;
//    ui.setupUi(widget);
//    widget->show();


    return app.exec();
}
