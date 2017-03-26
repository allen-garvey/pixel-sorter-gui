#ifndef PIXELSORTER_H
#define PIXELSORTER_H

#include <QtWidgets>

enum PixelSorterColor {red=0, green, blue, hue, saturation, value};

class PixelSorter
{
 public:
  static void pixelSortHorizontal(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex);
  static void pixelSortVertical(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex);

 private:
  // Disallow creating an instance of this object
  PixelSorter() {}
};

#endif // PIXELSORTER_H
