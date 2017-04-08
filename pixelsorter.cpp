#include "pixelsorter.h"
#include <algorithm>

std::function<bool (QRgb const &, QRgb const &)> PixelSorter::sortFunc(PixelSorterColor sortType){
    switch(sortType){
        case red:
            return [](QRgb const & a, QRgb const & b) -> bool { return qRed(a) < qRed(b); };
        case green:
            return [](QRgb const & a, QRgb const & b) -> bool { return qGreen(a) < qGreen(b); };
        case hue:
            return [](QRgb const & a, QRgb const & b) -> bool { return QColor(a).hue() < QColor(b).hue(); };
        case saturation:
            return [](QRgb const & a, QRgb const & b) -> bool { return QColor(a).saturation() < QColor(b).saturation(); };
        case value:
            return [](QRgb const & a, QRgb const & b) -> bool { return QColor(a).value() < QColor(b).value(); };
        default:
            //blue
            return [](QRgb const & a, QRgb const & b) -> bool { return qBlue(a) < qBlue(b); };
    }
}

void PixelSorter::pixelSortHorizontal(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex){
    auto sortFunc = PixelSorter::sortFunc(sortType);
    int imageHeight = image->height();
    int imageWidth = image->width();
    //sanity check on endIndex
    endIndex = endIndex > imageHeight ? imageHeight : endIndex;

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
