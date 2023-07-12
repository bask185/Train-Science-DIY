#include "Signal.h"


/* problems to solve:
more DCC addresses?
should de done in INO. depending on aspect type the amount of DCC addresses and 
such should be translated to a state from 1 to 4.

This signal code should be kept as modular as possible
Some kind of table in which the amount of aspects and how they look like are
in binary format.
*/

/******* ASPECT TYPES **********
Below there is a table with aspects. By default there are 10 different types of signals
each type of signal may have 10 different aspects. If you need a new type, or more aspects 
you are free to alter the numbers appriopately

The aspects themselfes are written in binary format. The bits correspond with the outputs from right to left
I added three commonly used aspects.

As of now FLASHING is not (yet) supported. I need to create a struct for that in order to use bitfields.
 *
 * */ 

static uint8_t  currentAspect ;



Aspect aspect[8] = {

{   2,                          // nAspect
    2,                          // nLeds
{   {  ON, OFF },               // Green
    { OFF,  ON }, }, },            // Red

{   5,                          // nAspect 
    3,                          // nLeds
{   {  ON, OFF, OFF },          // Green
    {   X, OFF, OFF },          // Green flashing
    { OFF,  ON, OFF },          // Yellow
    { OFF,   X, OFF },          // Yellow flashing
    { OFF, OFF,  ON }, } } ,      // red

{   6,                          // nAspect 
    4,                          // nLeds
{   {  ON, OFF, OFF, OFF },     // red
    { OFF,  ON, OFF,  ON },     // double yellow
    { OFF,   X, OFF,   X },     // double yellow flashing
    { OFF,  ON, OFF, OFF },     // single yellow
    { OFF,   X, OFF, OFF },     // single yellow flashing
    { OFF, OFF,  ON, OFF }, }, },  // RED
} ;

uint8_t getAspectAmount()
{
    return aspect[currentAspect].nAspect ;
}

uint8_t getOutputAmount()
{
    return aspect[currentAspect].nLeds ;
}


Signal::Signal()
{
}

void Signal::begin( uint8_t tone, uint8_t _A, uint8_t _B, uint8_t _C, uint8_t _D ) // tone is 2, 3 or 4.
{
    pinA = _A ;
    pinB = _B ;
    pinC = _C ;

    pinMode( pinA, OUTPUT ) ;
    pinMode( pinB, OUTPUT ) ;
    pinMode( pinC, OUTPUT ) ;
    pinMode( pinD, OUTPUT ) ;

    digitalWrite( pinA,  LOW ) ;
    digitalWrite( pinB,  LOW ) ;
    digitalWrite( pinC,  LOW ) ;
    digitalWrite( pinD,  LOW ) ;
}

void Signal::setMode( uint8_t _aspect )
{
    aspect = _aspect ;
}

void Signal::setState( uint8_t state )
{
    if( state == 0 ) // green
    {
        digitalWrite( pinA, HIGH ) ;
        digitalWrite( pinB,  LOW ) ;
        digitalWrite( pinC,  LOW ) ;
    }
    if( state == 1 ) // yellow
    {
        digitalWrite( pinA,  LOW ) ;
        digitalWrite( pinB, HIGH ) ;
        digitalWrite( pinC,  LOW ) ;
    }
    if( state == 2 ) // red
    {
        digitalWrite( pinA,  LOW ) ;
        digitalWrite( pinB,  LOW ) ;
        digitalWrite( pinC, HIGH ) ;
    }    
    if( state == 3 ) // off
    {
        digitalWrite( pinA,  LOW ) ;
        digitalWrite( pinB,  LOW ) ;
        digitalWrite( pinC,  LOW ) ;
    }    
}

/*  NS signal, green yellow n red
   ___
  /   \
 /     \
|   G   |
|       |
|   Y   |
|       |
|   R   |
 \     /
  \___/
    |
    |
    |
*/

/* german signals

main signal
   _____
  /     \
 / G     \
 | R   R |
 |       |
 |       |
 | Y     |
 |_______|
    ||
    ||
    ||


pre signal
      ____
     /    \
    / Y G  |
   /      /
  /      /
 |  Y G /
  \____/
    ||
    ||
    ||

combined main and pre
    _____
   /     \
  / G     \
  | R   R |
  |       |
  |       |
  | Y     |
  |_______|
     ||
     ||___
     /    \
    / Y G  |
   /      /
  /      /
 |  Y G /
  \____/
     ||
     ||
     ||

*/