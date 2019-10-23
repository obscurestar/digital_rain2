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
  /*In the interest of a simple algorith, let's say that when we pick a new hue, we want it 
   * to only include 1 or 2 'pure' colors.  Further, to make the space more chill and make
   * going to pure red a more rare event.  We can do this with a cool math trick.
   * We're going to use 3 bits of a byte of represent Red, Green and Blue.  If this bit is set
   * then our mask will evaluate to true for it. To avoid black, white and pure red at the same time
   * we chose a random number from 0-2, add 1 to it makeing 1-4 and then bit shift that result
   * one to the left.  This gives us a value of 2-6 which corresponds to the bits:
   * 2 010 green
   * 3 110 yellow
   * 4 001 blue
   * 5 101 magenta
   * 6 011 cyan
   */
  
  byte cbits=(random(3)+1)<<1;  //generate value from 2-6

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
      if (1 & mHueMask>>c)    //Stagger around in the relative color space.
      {

        /*
         * Remeber that we only have one byte for the intensity of the subpixel
         */
        if ( pix[p].c[c] < 255)    
        {
          pix[p].c[c] += random(2);  //Possibly add 1 to this RGB component
        }
        if (pix[p].c[c] > 0) 
        {
          pix[p].c[c] -= random(2);  //Possibly sub 1 and possibly cancel prev op
        }
      }
      else
      {                       //Stagger towards 0, let iterator know this one doesn't count. 
        if (pix[p].c[c] > 0) //This RGB should not be set in this hue. Still draining previous color
        {
          mDirty=true;
          pix[p].c[c] -= random(2); //Wander slowly towards 0.
        }
      }
    }
  }
  return mDirty;
}


#endif //RAIN_H
