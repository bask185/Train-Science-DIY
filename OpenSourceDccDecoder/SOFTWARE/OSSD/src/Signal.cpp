#include "Signal.h"



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

    digitalWrite( pinA, HIGH ) ;
    digitalWrite( pinB,  LOW ) ;
    digitalWrite( pinC,  LOW ) ;
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

uint8_t Signal::update()
{
    return 1 ;
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