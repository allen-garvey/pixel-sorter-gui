#include "pixelsorter.h"
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <array>
#include <vector>

void PixelSorter::countingPixelSort(QRgb *scanLine, int length, PixelSorterColor sortType)
{
    const int rgbLimit = 256;

    std::array <std::vector<QRgb>, rgbLimit> pixelMatrix;

    #pragma omp parallel for
    for(int i=0;i<rgbLimit;i++){
        pixelMatrix[i] = std::vector<QRgb>();
    }

    for(int i=0;i<length;i++){
        QRgb currentPixel = scanLine[i];
        int pixelIndex;
        switch(sortType){
            case red:
                pixelIndex = qRed(currentPixel);
                break;
            case green:
                pixelIndex = qGreen(currentPixel);
                break;
            case hue:
                pixelIndex = QColor(currentPixel).hue();
                //achromatic colors have hue of -1, so in that case use value
                if(pixelIndex < 0){
                    pixelIndex = QColor(currentPixel).value();
                }
                break;
            case saturation:
                pixelIndex = QColor(currentPixel).saturation();
                break;
            case value:
                pixelIndex = QColor(currentPixel).value();
                break;
            default:
                //blue
                pixelIndex = qBlue(currentPixel);
                break;
        }
        pixelMatrix[pixelIndex].push_back(currentPixel);
    }

    for(int i=0, scanLineIndex=0;i<rgbLimit;i++){
        std::vector<QRgb> currentVector = pixelMatrix[i];
        int currentVectorSize = currentVector.size();
        for(int j=0;j<currentVectorSize;j++){
            scanLine[scanLineIndex] = currentVector[j];
            scanLineIndex++;
        }
    }

}


void PixelSorter::pixelSortHorizontal(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex){
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
        PixelSorter::countingPixelSort(rowData, imageWidth, sortType);
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
