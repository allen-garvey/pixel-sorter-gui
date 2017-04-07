#include "pixelsorter.h"
#include <algorithm>

std::function<bool (QRgb const &, QRgb const &)> PixelSorter::sortFunc(PixelSorterColor sortType){
    return [&](QRgb const & a, QRgb const & b) -> bool {
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
    //TODO: add vertical sorting functionality
}
