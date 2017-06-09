#include "imageviewer.h"
#include <QApplication>
#include <QDesktopWidget>
#include <omp.h>

int main(int argc, char *argv[])
{
    //set number of threads used for pixel sorting
    omp_set_num_threads(omp_get_num_procs());

    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(ImageViewer::tr("Pixel Sorter"));
    ImageViewer imageViewer;
    //center the window in the middle of the screen
    //based on: https://forum.qt.io/topic/21035/how-to-move-the-window-to-the-center-of-the-screen/4
    imageViewer.move(QApplication::desktop()->screen()->rect().center() - imageViewer.rect().center());
    imageViewer.show();

    return app.exec();
}
