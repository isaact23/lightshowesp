#include "pixelData.hpp"

int x_offsets[] = {10, 48, 60, 110, 125};
int y_offsets[] = {35, 75, 85, 105, 110};
int seg_lengths[] = {11, 32, 11, 48, 11, 18, 11, 27, 10, 15, 11, 16, 12, 27, 11, 17, 12, 48, 11, 31};

// Get the coordinate and other data corresponding to a pixel index.
pixelData getPixelData(int pixel) {

  // Get the segment that the pixel belongs to,
  // and the percentage of the way the pixel is along the segment
  int seg = 0;
  float percent = 0;
  int startPixel = 0;
  int endPixel = 0;
  for (int i = 0; i < SEG_COUNT; i++) {
    endPixel += seg_lengths[i];
    if (pixel < endPixel) {
      seg = i;
      percent = ((float) pixel - startPixel) / (endPixel - startPixel);
      break;
    }
    startPixel = endPixel;
  }

  // Get the segment direction and box number
  int dir = seg % 4;
  int box = floor(seg / 4);

  // Get indices of start and end coordinates

  // Calculate start and end coordinates
  int startX = 0;
  int startY = 0;
  int endX = 0;
  int endY = 0;

  // Bottom segments (moving right)
  if (dir == 0) {
    startX = box * 2;
    endX = box * 2 + 1;
    startY = 0;
    endY = 0;
  }
  // Right segments (moving up)
  else if (dir == 1) {
    startX = box * 2 + 1;
    endX = startX;
    startY = 0;
    endY = 1;
  }
  // Top segments (moving left)
  else if (dir == 2) {
    startX = box * 2 + 1;
    endX = box * 2;
    startY = 1;
    endY = 1;
  }
  // Left segments (moving down)
  else if (dir == 3) {
    startX = box * 2;
    endX = startX;
    startY = 1;
    endY = 0;
  }

  // Convert coordinate indices to actual coordinates
  coordinate startCoord = getCornerCoordinate(startX, startY);
  coordinate endCoord = getCornerCoordinate(endX, endY);

  // Interpolate between coordinates
  coordinate coord = {
    .x = floor((endCoord.x - startCoord.x) * percent + startCoord.x),
    .y = floor((endCoord.y - startCoord.y) * percent + startCoord.y)
  };

  // Create pixel data
  pixelData data = {
    .coord = coord,
    .percent = percent,
    .box = box,
    .seg = seg
  };

  return data;
}

// Get a coordinate from a corner of the SONUS logo
coordinate getCornerCoordinate(int x, int y) {

  int xCoord;
  int yCoord;

  if (x < BOX_COUNT) {
    xCoord = x_offsets[x];
    if (y == 0) {
      yCoord = y_offsets[x];
    } else {
      yCoord = HEIGHT - y_offsets[x];
    }
  } else {
    int flippedX = BOX_COUNT * 2 - x - 1;
    xCoord = WIDTH - x_offsets[flippedX];
    if (y == 0) {
      yCoord = y_offsets[flippedX];
    } else {
      yCoord = HEIGHT - y_offsets[flippedX];
    }
  }

  coordinate corner = {
    .x = xCoord,
    .y = yCoord
  };

  return corner;
}