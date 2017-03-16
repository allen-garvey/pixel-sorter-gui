#include "pixelsorter.h"
#include <algorithm>
#include <iostream>

void PixelSorter::pixelSortHorizontal(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex){
    int imageHeight = image->height();
    int imageWidth = image->width();
    //sanity check on endIndex
    endIndex = endIndex > imageHeight ? imageHeight : endIndex;

    //pixel manipulation based on: http://stackoverflow.com/questions/2095039/qt-qimage-pixel-manipulation
    int linesSorted = 0;
    QRgb *rowData;
    for(int i=startIndex;i<endIndex;i++){
        rowData = (QRgb*)image->scanLine(i);
        std::sort(rowData, rowData + imageWidth - 1,
                  [](QRgb const & a, QRgb const & b) -> bool
                  { return qRed(a) < qRed(b); } );
        linesSorted++;
        if(linesSorted == countIndex){
            linesSorted = 0;
            i += skipIndex;
        }
    }
}
