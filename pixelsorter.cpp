#include "pixelsorter.h"
#include <algorithm>
#include <omp.h>
#include <iostream>

void PixelSorter::pixelSortHorizontal(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex){
    auto sortFunc = [&](QRgb const & a, QRgb const & b) -> bool {
            switch(sortType){
                case red:
                    return qRed(a) < qRed(b);
                case green:
                    return qGreen(a) < qGreen(b);
                case hue:
                    return QColor(a).hue() < QColor(b).hue();
                case saturation:
                    return QColor(a).saturation() < QColor(b).saturation();
                case value:
                    return QColor(a).value() < QColor(b).value();
                default:
                    //blue
                    return qBlue(a) < qBlue(b);
            }
    };
    int imageHeight = image->height();
    int imageWidth = image->width();
    //sanity check on endIndex
    endIndex = endIndex > imageHeight ? imageHeight : endIndex;

    double startTime = omp_get_wtime();
    //pixel manipulation based on: http://stackoverflow.com/questions/2095039/qt-qimage-pixel-manipulation
    int linesSorted = 0;
    QRgb *rowData;
    for(int i=startIndex;i<endIndex;i++){
        rowData = (QRgb*)image->scanLine(i);
        std::sort(rowData, rowData + imageWidth - 1, sortFunc);
        linesSorted++;
        if(linesSorted == countIndex){
            linesSorted = 0;
            i += skipIndex;
        }
    }

    double endTime = omp_get_wtime();
    double megaPixelSortsPerSecond = (double)(imageHeight * imageWidth)/(endTime-startTime)/1000000.;
    std::cout << "MegaPixelSortsPerSecond: " << megaPixelSortsPerSecond << std::endl;
}

void PixelSorter::pixelSortVertical(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex){
    QTransform rotationTransform;
    rotationTransform.rotate(90);
    QImage sortedImage = image->transformed(rotationTransform);

    PixelSorter::pixelSortHorizontal(&sortedImage, sortType, startIndex, countIndex, skipIndex, endIndex);

    //return to original orientation
    rotationTransform.rotate(180);
    sortedImage = sortedImage.transformed(rotationTransform);
    *image = sortedImage;

}
