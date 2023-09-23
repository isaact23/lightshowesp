#ifndef MAIN_HPP
#define MAIN_HPP

#define PIN 4
#define NUM_LEDS 390

#include "fastLedInclude.hpp"
#include "rules.hpp"
#include "pixelData.hpp"

// Run the SONUS light program.
void sonusProgram();

// Fill light strip with a color, start inclusive, end exclusive.
void fill(int start, int end, int color);

#endif
