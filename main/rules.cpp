#include "rules.hpp"

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