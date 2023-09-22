#include <FastLED.h>
#include <math.h>

#define PIN 4
#define NUM_LEDS 390

#define WIDTH 300
#define HEIGHT 300

#define SEG_COUNT 20
#define BOX_COUNT 5

CRGB leds[NUM_LEDS];
int x_offsets[] = {10, 48, 60, 110, 125};
int y_offsets[] = {35, 75, 85, 105, 110};
int seg_lengths[] = {11, 32, 11, 48, 11, 18, 11, 27, 10, 15, 11, 16, 12, 27, 11, 17, 12, 48, 11, 31};


struct coordinate {
  int x;
  int y;
};
typedef struct coordinate coordinate;

// Run segment test
void testSegs();

// Run direction test
void testDirs();

// Fill light strip with a color, start inclusive, end exclusive.
void fill(int start, int end, int color);

// Get the coordinate corresponding to a pixel index.
coordinate getCoordinateFromPixel(int pixel);

// Get a coordinate from a corner of the SONUS logo
coordinate getCornerCoordinate(int x, int y);

void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");

  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(60);

  delay(3000);

  for (int i = 0; i < NUM_LEDS; i++) {
    coordinate coord = getCoordinateFromPixel(i);
    Serial.printf("Pixel %i has x %i y %i\n", i, coord.x, coord.y);

    if (coord.y > 200) {
      leds[i] = CRGB::Red;
    } else {
      //leds[i] = CRGB::Green;
    }
    FastLED.show();
  }
}

void loop() {
  
}

// Run segment test
void testSegs() {
  int startPixel = 0;

  for (int i = 0; i < SEG_COUNT; i++) {
    int endPixel = startPixel + seg_lengths[i];
    fill(startPixel, endPixel, CRGB::Red);
    delay(1000);
    fill(startPixel, endPixel, CRGB::Black);
    delay(1000);

    startPixel = endPixel;
  }
}

// Run direction test
/*void testDirs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int dir = (getSegmentFromPixel(i) % 4);
    if (dir == 0) {
      leds[i] = CRGB::Red;
    } else if (dir == 1) {
      leds[i] = CRGB::Green;
    } else if (dir == 2) {
      leds[i] = CRGB::Blue;
    } else if (dir == 3) {
      leds[i] = CRGB::Black;
    } else {
      leds[i] = CRGB::White;
    }
  }
  FastLED.show();
}*/

// Fill light strip with a color, start inclusive, end exclusive.
void fill(int start, int end, int color) {
  for (int i = start; i < end; i++) {
    leds[i] = color;
  }

  FastLED.show();
}

// Get the coordinate corresponding to a pixel index.
coordinate getCoordinateFromPixel(int pixel) {

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
  Serial.printf("%.4f\n", percent);

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

  return coord;
}

// Get a coordinate from a corner of the SONUS logo
coordinate getCornerCoordinate(int x, int y) {

  int x_offsets[] = {10, 48, 60, 110, 125};
  int y_offsets[] = {35, 75, 85, 105, 110};

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
