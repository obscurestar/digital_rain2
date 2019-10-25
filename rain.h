//rain.h
//Use random walks to do a palette shift.  

#ifndef RAIN_H  //These preprocessor directives let you include without worrying about dupes.
#define RAIN_H  //Once defined it doesn't need to be re-included.

#include "color.h"

extern const short NUM_LEDS; //Let's address this in V3!
extern COLOR pix[];  //Ditto!

class Rain
{
public:   //public variables.
  int mShiftOdds; //The 1 in n odds of picking a new hue mask.
public:   //public functions.
  Rain(); //Default constructor
  void loopStep();  //Per frame call from loop.
private:
  byte pickHueMask();   //Get a new hue mask
  byte walkPixels();    //The math of the shimmer.
private:  //class private variables
  byte mHueMask;  //Which bits are active on this iteration.
  bool mDirty;    //Set to true when any RGB not in current set is set for any pixel in chain.
};

Rain::Rain() : mDirty(false),
               mShiftOdds(1000)
{
  mHueMask = pickHueMask();
}

void Rain::loopStep()
{
  if (!walkPixels())
  {
    if (!random(mShiftOdds))
    {
      mHueMask = pickHueMask();
    }
  }
}

byte Rain::pickHueMask()
{
  /*Generate a color.  I want to use mostly calming colors with bursts of pure red being somewhat uncommon
   * so we're going to randomly create a 3-bit mask which is never 000 or 100
   * 2 010 green
   * 3 110 yellow
   * 4 001 blue
   * 5 101 magenta
   * 6 011 cyan
   */
  
  byte cbits=random(5)+2;  //generate value from 2-6

   /*Now let's create the rare opportunity for red.  We'll do this by inverting the bitmask
    * when our random value is 0.  if the color happens to be cyan, it will be flipped to red. 
    */

  if (!random(10))
  {
    cbits = ~cbits;  // ~ is the complimentary operator 010 becomes 101 etc
  }
  return cbits;
}

//Iterate through pixels and stagger around in the relative color space. 
byte Rain::walkPixels()
{
  mDirty=false;
  for (int p=0;p<NUM_LEDS; ++p)  //Loop through pixels.
  {
    for (int c=0;c<3;++c)     //Loop through RBG sub-pixels of each pixel.
    {
      if ( (mHueMask >> c) & 1 )
      {
          signed int val = pix[p].c[c] + (random(3) - 1); // rand result set [0,1,2] - 1 = [-1, 0, 1]
          if (val > 0 && val < 255)
          {
            pix[p].c[c] = val;  //Stagger around in the relative color space.
          }
      }
      else
      {                       //Stagger towards 0, let iterator know this one doesn't count. 
        if (pix[p].c[c] > 0) //This RGB should not be set in this hue. Still draining previous color
        {
          mDirty=true;
          if (!random(6))
          {
            pix[p].c[c] --; //Wander slowly towards 0.
          }
        }
      }
    }
  }
  return mDirty;
}


#endif //RAIN_H
