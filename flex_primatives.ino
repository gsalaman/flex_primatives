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

typedef enum
{
  DISPLAY_TEXT,
  DISPLAY_RECTANGLES,
  DISPLAY_RAINBOW,
  DISPLAY_DROPS
} display_mode_type;

display_mode_type display_mode=DISPLAY_RECTANGLES;

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

typedef struct
{
  bool     active;
  int      center_x;
  int      center_y;
  int      radius;
  uint32_t color;  
} drop_data_type;

#define NUM_DROPS 1
drop_data_type drops[NUM_DROPS];
#define MAX_DROP_RADIUS 3

void init_drops( void )
{
  int i;

  for (i=0;i<NUM_DROPS;i++)
  {
    drops[i].active = false;
  }
}

void display_drops( void )
{
  bool new_drop=false;
  int  i;
  
  // walk through the drops array, increasing the radius of any active drops.  If any
  //   are too big, mark them as inactive.
  // if we have any inactive, make the FIRST active (so we're only adding one at a time)
  //    May change this in future to randomly decide to make one active

  for (i = 0; i < NUM_DROPS; i++)
  {
    if (drops[i].active)
    {
      if (drops[i].radius >= MAX_DROP_RADIUS)
      {
        //Serial.print("active drop ");
        //Serial.print(i);
        //Serial.println(" hit max radius");
        
        drops[i].active = false;
      }
      else 
      {
        drops[i].radius++;
        matrix.fillCircle(drops[i].center_x, drops[i].center_y,drops[i].radius,0);
        matrix.drawCircle(drops[i].center_x, drops[i].center_y,drops[i].radius,drops[i].color);
        matrix.show();
        //Serial.print("Increase radius of drop ");
        //Serial.print(i);
        //Serial.print(" now radius");
        //Serial.println(drops[i].radius);
      }
    }
    else if (!new_drop)
    {
      //Serial.print("New drop: ");
      //Serial.println(i);
      
      new_drop = true;
      drops[i].center_x = random(0,32);
      drops[i].center_y = random(0,8);
      drops[i].radius = 0;
      drops[i].color = rainbow[random(0,32)];
      drops[i].active = true;
    }

    
  }
  delay(100);
}

void display_rectangles( void )
{
  int x=0;
  int y=0;
  int x_jump=4;
  while (y < MATRIX_ROWS/2)
  {
    matrix.fillScreen(0);
    matrix.drawRect(x,y,MATRIX_COLUMNS-(2*x),MATRIX_ROWS-(2*y),matrix.Color(255,0,0));
    matrix.show();
    delay(100);
    y++;
    x=x+x_jump;
  }

  while (y > 0)
  {
    y--;
    x=x-x_jump;
    matrix.fillScreen(0);
    matrix.drawRect(x,y,MATRIX_COLUMNS-(2*x),MATRIX_ROWS-(2*y),matrix.Color(255,0,0));
    matrix.show();
    delay(100);

    
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

  init_drops();

  Serial.println("Init complete");
  
}

int x    = matrix.width();

#define PIXELS_PER_CHAR 6
char message[]="Glenn Rules";

int pass = 0;

void loop() 
{
  int message_pixels;
  
  Serial.println(display_mode);

  switch (display_mode)
  {
 
     case DISPLAY_TEXT:     
       matrix.fillScreen(0);
       matrix.setCursor(x, 0);
       matrix.print(message);
       message_pixels = PIXELS_PER_CHAR * strlen(message);
       if(--x < -message_pixels) 
       {
          x = matrix.width();
          if(++pass >= 3) pass = 0;
          matrix.setTextColor(colors[pass]);
       }
       matrix.show();
       delay(100);
       break;
 
     
     case DISPLAY_RECTANGLES:
       display_rectangles();
     break;

     case DISPLAY_RAINBOW:     
       for (int i=0;i<NUM_RAINBOW_COLORS;i++)
       {
         matrix.drawLine(i,0,i,7,rainbow[i]);
       }
       matrix.show();
     break;
 
     case DISPLAY_DROPS:
       display_drops();
     break;

     default:
      Serial.println("Unknown mode!");
  }  
  
}
