/*Digital Rain 2.  
  Previously:  Prototype exists now only in compiled form.  RIP unbacked-up repository.
  I loved you, but respected you not nearly enough.In your honor, your reproduction
  shall be written and rewritten to teach.

  Overview:  Using coin tosses and a few simple rules, we shall cycle a limited palette in a
  way that shimmers.  See rain.h tab at top of window for more details.
  
  Terms:
    LED   Generally used when referring to the physical hardware.
    PIXEL Generally to refer to the digital representation of the LED
  */

//TODO protect constants in V3
//Hardware constants
const short PIN_LED = 6;  //Pin connecting the IN on the LED strip to the CPU board.
const short NUM_LEDS = 16; //Num LEDS in our array.

#include <Adafruit_NeoPixel.h>  //Someone else did the heavy lifting.  Say thanks!
#ifdef __AVR__
  #include <avr/power.h>
#endif

/*Include the other tabs from the top of this window in our application. 
 * This structure isn't required but it's a helpful way to collect 
 * related notions and it's good to get organized early on
 */
 #include "color.h"  //utility structures related to colors
 #include "rain.h"  //The digital rain algorithm
  
//TODO protect globals in V3.
//Global variables
/*Handle to the LED array. We will use this to apply the values to the pixels in the array.*/
Adafruit_NeoPixel H_LEDS = Adafruit_NeoPixel(NUM_LEDS, PIN_LED, NEO_GRBW + NEO_KHZ800);

byte MAX_BRITE=255;  //The brightest we want our display to get. 

/*pix is 4 bytes per LED. Remember our 328 CPU only has 2K of active memory.
 * if you have 512 pix, that's all of the memory including the memory for calling functions
 * making computations, talking to our libraries, etc. 
 * We will address this in a later version by using a 32K expansion chip on the IC2 bus.
 */
COLOR pix[NUM_LEDS]; //Array of data for our pixels.  See above!
Rain rain;           //Container class for rain algorithm.  See rain.h for details

void setup() {       //Builtin function run once at start of app.
  Serial.begin(115200); //Open serial(Com speed) Useful for debug but watch string memory use.

  //strip.setBrightness(BRIGHTNESS);  //TODO experiment with this feature.
  H_LEDS.begin(); //Initialize communication with WS281* chain.
  H_LEDS.show(); //No values are set so this should be black.
}

void loop() { //Builtin function.
  rain.loopStep();
  display_pix();
  //TODO:
  /*
   * This delay is hard-coded and we have to recompile and re-upload any time
   * we want to change it.  A variable control would be cool.  In V3 we'll 
   * think of that control as a discrete concept and discuss implementing it
   * over serial, potentiometer, or digital signal and some example uses of 
   * each.
   */
  delay(10);
}

/*display_pix transcribes the contents of the pix array to the LED driver hardware.
 * In some future version we may want different display routines for stateless 
 * patterns or scaling, mirroring, compressing and otherwise distorting part of a 
 * pattern and projecting it across the array.
 */
void display_pix()
{
  for (int i=0;i<NUM_LEDS;++i)
  {
    H_LEDS.setPixelColor(i,pix[i].l);
  }
  H_LEDS.show();
}
