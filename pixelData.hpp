#ifndef PIXEL_DATA_HPP
#define PIXEL_DATA_HPP

#include <math.h>

#define WIDTH 300
#define HEIGHT 300

#define SEG_COUNT 20
#define BOX_COUNT 5

struct coordinate {
  int x;
  int y;
};
typedef struct coordinate coordinate;

struct pixelData {
  coordinate coord;
  float percent;
  int box;
  int seg;
};
typedef struct pixelData pixelData;

// Get the coordinate and other data corresponding to a pixel index.
pixelData getPixelData(int pixel);

// Get a coordinate from a corner of the SONUS logo
coordinate getCornerCoordinate(int x, int y);

#endif
