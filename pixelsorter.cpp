#include "pixelsorter.h"
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <array>
#include <vector>

void PixelSorter::countingPixelSort(QRgb *scanLine, int length, PixelSorterColor sortType)
{
    const int RGB_LIMIT = 256;
    const int HUE_LIMIT = 360;
    const int NUM_THREADS = omp_get_max_threads();

    int MATRIX_SIZE;
    if(sortType == hue){
        MATRIX_SIZE = HUE_LIMIT * NUM_THREADS;
    }
    else{
        MATRIX_SIZE = RGB_LIMIT * NUM_THREADS;
    }

    std::vector<QRgb> *pixelMatrix = new std::vector<QRgb>[MATRIX_SIZE];

    #pragma omp parallel for
    for(int i=0;i<MATRIX_SIZE;i++){
        pixelMatrix[i] = std::vector<QRgb>();
    }

    #pragma omp parallel for
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
        int currentThreadNum = omp_get_thread_num();
        int matrixIndex = (pixelIndex * NUM_THREADS) + currentThreadNum;
        pixelMatrix[matrixIndex].push_back(currentPixel);
    }

    int *vectorOffsets = new int[MATRIX_SIZE];
    vectorOffsets[0] = 0;
    for(int i=1;i<MATRIX_SIZE;i++){
        vectorOffsets[i] = vectorOffsets[i-1] + pixelMatrix[i-1].size();
    }

    #pragma omp parallel for
    for(int i=0;i<MATRIX_SIZE;i++){
        std::vector<QRgb> currentVector = pixelMatrix[i];
        int currentVectorSize = currentVector.size();
        int currentOffset = vectorOffsets[i];
        for(int j=0;j<currentVectorSize;j++){
            scanLine[currentOffset + j] = currentVector[j];
        }
    }

    delete[] vectorOffsets;
    delete[] pixelMatrix;
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
