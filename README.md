# Primatives for the "flex strip"
This code demonstrates driving the 8x32 "flexible" Adafruit matrix with an Uno.

## Libraries
There are a couple layers for includes when driving a matrix in Arduino:
* `#include Adafruit_GFX.h` is the core graphics functionality library.  This provides a common interface for whatever HW is being used.
* `#include Adafruit_NeoMatrix.h` is the hardware specific library for the flex matrix
* `#include Adafruit_NeoPixel.h` is the lowest level library, which drives the matrix pixel-by-pixel.

Note that we're using Adafruit_NeoPixel.h as the low level driver, NOT FastLED.h.  This means we don't get the cool gradient and palette functions that FastLED provides.

## Matrix initalization
Here's the block that sets up our matrix object for the 8x32:
```
#define LED_PIN 6
#define MATRIX_ROWS 8
#define MATRIX_COLUMNS 32

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_COLUMNS, MATRIX_ROWS, LED_PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
```
A couple notes here:
* You can move LED_PIN to wherever you want...and note that we're only using one pin rather than the multitude we need for the 16-pin RGB matrixes.
* I'm using BOTTOM/RIGHT/ZIGZAG to get the patterns happy.  That was trial and error.  :)

##  Primatives
Full reference is here:
https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives

Note that this is from that top level GFX library, so not all primatives are avalilable for all hardware.  Specifically, loading images won't work with this matrix (as it has no back-buffer).  Here's a list of the ones that are happy:
```
void fillScreen(uint16_t color);
void drawPixel(uint16_t x, uint16_t y, uint16_t color);
void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
void fillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
void fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
```
All of these are member functions of the object you created as an Adafruit_Neomatrix..."matrix" in the examaple code above.  This means you'd call them like so:
```
matrix.fillScreen(0);    // Black out the screen
```
Color for all of these is a 16 bit RBG type...5 bits R, 6 bits G, 5 bits B.  You can use matrix.Color(R,G,B) with 0-255 values to produce the appropriate color (ie, converting from a 24-bit CRGB type to this 16 bit type)

Text primatives:
If you want a single character:
```
void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
```
Or, if you want to print a string:
```
void setCursor(uint16_t x0, uint16_t y0);
void setTextColor(uint16_t color);
void setTextColor(uint16_t color, uint16_t backgroundcolor);
void setTextSize(uint8_t size);
void setTextWrap(boolean w);
```
followed by a call to `matrix.print(my_string)`
