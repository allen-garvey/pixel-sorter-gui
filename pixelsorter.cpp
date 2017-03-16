#include "pixelsorter.h"
#include <algorithm>
#include <iostream>

void PixelSorter::pixelSortHorizontal(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex){
    int imageHeight = image->height();
    int imageWidth = image->width();
    //sanity check on endIndex
    endIndex = endIndex > imageHeight ? imageHeight : endIndex;

    auto sortFunc = [&](QRgb const & a, QRgb const & b) -> bool {
        if(sortType == red){
            return qRed(a) < qRed(b);
        }
        else if(sortType == green){
            return qGreen(a) < qGreen(b);
        }
        return qBlue(a) < qBlue(b);
    };

    //pixel manipulation based on: http://stackoverflow.com/questions/2095039/qt-qimage-pixel-manipulation
    int linesSorted = 0;
    QRgb *rowData;
    for(int i=startIndex;i<endIndex;i++){
        rowData = (QRgb*)image->scanLine(i);
        std::sort(rowData, rowData + imageWidth - 1,
                  sortFunc );
        linesSorted++;
        if(linesSorted == countIndex){
            linesSorted = 0;
            i += skipIndex;
        }
    }
}
