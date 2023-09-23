#include <FastLED.h>
#include <math.h>

#define PIN 4
#define NUM_LEDS 390

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

// Run the SONUS light program.
void sonusProgram();

// Fill light strip with a color, start inclusive, end exclusive.
void fill(int start, int end, int color);

// Rule to render diagonal stripes
CRGB diagStripes(pixelData data, float timeElapsed);

// Rule to render diagonal rainbow
CRGB rainbowStripes(pixelData data, float timeElapsed);

int x_offsets[] = {10, 48, 60, 110, 125};
int y_offsets[] = {35, 75, 85, 105, 110};
int seg_lengths[] = {11, 32, 11, 48, 11, 18, 11, 27, 10, 15, 11, 16, 12, 27, 11, 17, 12, 48, 11, 31};

CRGB leds[NUM_LEDS];

void setup() {
  sonusProgram();
}

void loop() {}

// Run the SONUS light program.
void sonusProgram() {
  Serial.begin(9600);
  Serial.println("Starting SONUS program!");

  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUM_LEDS);

  //fill(CRGB::Black, 0, NUM_LEDS);
  //return;

  float timeSinceStart = 0;

  CRGB (*rule)(pixelData data, float timeElapsed) = diagStripes;

  while (true) {

    for (int i = 0; i < NUM_LEDS; i++) {
      pixelData data = getPixelData(i);

      leds[i] = rule(data, timeSinceStart);
    }
    FastLED.show();
    delay(10);
    timeSinceStart += 0.01f;
  }
}

// Fill light strip with a color, start inclusive, end exclusive.
void fill(int start, int end, int color) {
  for (int i = start; i < end; i++) {
    leds[i] = color;
  }

  FastLED.show();
}

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

// Rule to render diagonal stripes
CRGB diagStripes(pixelData data, float timeElapsed) {

  const int STRIPE_WIDTH = 80;
  const float SPEED = 300;

  int pix = (data.coord.x + data.coord.y + (timeElapsed * SPEED));
  int stripe = floor(pix / STRIPE_WIDTH);

  // Show 10 stripes, then hide 30 stripes
  if (stripe % 2 == 0 && (stripe / 10) % 4 == 0 ) {
    return CRGB::Black;
  } else {
    return CRGB::Red;
  }
}

// Rule to render diagonal rainbow
CRGB rainbowStripes(pixelData data, float timeElapsed) {

  const float SPEED = 300;

  int pix = (data.coord.x + data.coord.y + (timeElapsed * SPEED));
  //int stripe = floor(pix / STRIPE_WIDTH);

  CRGB color;
  color.setHSV(pix, 255, 255);
  return color;

  // Show 10 stripes, then hide 30 stripes
  /*switch (stripe % 6) {
    case 0:
      return CRGB::Red;
    case 1:
      return CRGB::Orange;
    case 2:
      return CRGB::Yellow;
    case 3:
      return CRGB::Green;
    case 4:
      return CRGB::Blue;
    case 5:
      return CRGB::Purple;
  }*/

  
}
