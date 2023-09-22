#include "main.hpp"

CRGB leds[NUM_LEDS];

// Run the SONUS light program.
void sonusProgram() {
  Serial.begin(9600);
  Serial.println("Starting SONUS program!");

  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUM_LEDS);

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
