#ifndef PIXELSORTER_H
#define PIXELSORTER_H

#include <QtWidgets>
#include <functional>

enum PixelSorterColor {red=0, green, blue, hue, saturation, value};

class PixelSorter
{
 public:
  static void pixelSortHorizontal(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex);
  static void pixelSortVertical(QImage *image, PixelSorterColor sortType, int startIndex, int countIndex, int skipIndex, int endIndex);
  static std::function<bool (QRgb const &, QRgb const &)> sortFunc(PixelSorterColor sortType);
 private:
  // Disallow creating an instance of this object
  PixelSorter() {}
};

#endif // PIXELSORTER_H
