#ifndef RULES_HPP
#define RULES_HPP

#include <FastLED.h>
#include <math.h>
#include "pixelData.hpp"

// Rule to render diagonal stripes
CRGB diagStripes(pixelData data, float timeElapsed);

#endif