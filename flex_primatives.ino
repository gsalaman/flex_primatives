// Adafruit_NeoMatrix example for single NeoPixel Shield.

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

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

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

#define NUM_RAINBOW_COLORS 32
uint16_t rainbow[NUM_RAINBOW_COLORS];

void init_rainbow()
{
  int r=255;
  int g=0;
  int b=0;
  int step_size = 256/(NUM_RAINBOW_COLORS/2);
  int i;
  
  //walk from r to g
  for (i = 0; i < NUM_RAINBOW_COLORS/2; i++)
  {
    rainbow[i]=matrix.Color(r,g,b);

    Serial.print(i);
    Serial.print(" -> r:");
    Serial.print(r);
    Serial.print(" g:");
    Serial.print(g);
    Serial.print(" b:");
    Serial.println(b);
    
    r=r-step_size;
    g=g+step_size;
  }

  r=0;   // rounding error from last math...
  g=255;
  
  //now walk from g to b
  for (;i<NUM_RAINBOW_COLORS;i++)
  {
    rainbow[i]=matrix.Color(r,g,b);
    Serial.print(i);
    Serial.print(" -> r:");
    Serial.print(r);
    Serial.print(" g:");
    Serial.print(g);
    Serial.print(" b:");
    Serial.println(b);
    g = g-step_size;
    b = b+step_size;  
  }
}

void setup() 
{
  Serial.begin(9600);
  
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
  matrix.fillScreen(0);
  init_rainbow();
  matrix.show();

  Serial.println("Init complete");
  
}

int x    = matrix.width();

#define PIXELS_PER_CHAR 6
char message[]="Glenn Rules";

int pass = 0;

#define MODE_TEXT      1
#define MODE_RECTANGLE 2
#define MODE_RAINBOW   3
#define MODE_DROPS     4

void loop() 
{

  int my_mode=4;
  
  Serial.println(my_mode);

  if (my_mode == MODE_TEXT)
  {
       matrix.fillScreen(0);
       matrix.setCursor(x, 0);
       matrix.print(message);
       int message_pixels = PIXELS_PER_CHAR * strlen(message);
       if(--x < -message_pixels) 
       {
          x = matrix.width();
          if(++pass >= 3) pass = 0;
          matrix.setTextColor(colors[pass]);
       }
       matrix.show();
       delay(100);
  }
  else if (my_mode == MODE_RECTANGLE)
  {
      matrix.drawRect(1,1,4,4,colors[0]);
      matrix.show();
      Serial.println("rect");
      delay(100);
  }
  else if (my_mode == MODE_RAINBOW)
  {
      for (int i=0;i<NUM_RAINBOW_COLORS;i++)
      {
        matrix.drawLine(i,0,i,7,rainbow[i]);
      }
      matrix.show();
  }
  else if (my_mode == MODE_DROPS)
  {
    long y = random(0,8);
    long x = random(0,32);
    long radius = random(1,4);
    long color = random(0,32);

    matrix.drawCircle(x,y,radius, rainbow[color]);
    matrix.show();

    delay(1000);
    
  }
  else
  {
      Serial.println("Unknown mode!");
  }  
  
}
