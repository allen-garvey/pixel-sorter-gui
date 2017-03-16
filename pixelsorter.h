#ifndef PIXELSORTER_H
#define PIXELSORTER_H

#include <QtWidgets>

enum PixelSorterColor {red, green, blue};

class PixelSorter
{
 public:
  static void pixelSortHorizontal(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex);

 private:
  // Disallow creating an instance of this object
  PixelSorter() {}
};

#endif // PIXELSORTER_H
