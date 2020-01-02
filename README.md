# Primatives for the "flex strip"
This code demonstrates driving the 8x32 "flexible" Adafruit matrix with an Uno.

## Libraries
There are a couple layers for includes when driving a matrix in Arduino:
* `#include Adafruit_GFX.h` is the core graphics functionality library.  This provides a common interface for whatever HW is being used.
* `#include Adafruit_NeoMatrix.h` is the hardware specific library for the flex matrix
* `#include Adafruit_NeoPixel.h` is the lowest level library, which drives the matrix pixel-by-pixel.

Note that this example does *not* use the FastLED library.
