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

static uint8_t  currentAspect = 2 ;

// NOTE: as is now 1 aspect consumes 52 bytes. So I can add like 15 or so..

Aspect aspects[8] =
{
    {   2,                                  // nAspect
        2,                                  // nLeds
        {   {  ON, OFF },                   // Green
            { OFF,  ON }, }, },             // Red

    {   5,                                  // nAspect 
        3,                                  // nLeds
        {   {  ON, OFF, OFF },              // Green
            {   X, OFF, OFF },              // Green flashing
            { OFF,  ON, OFF },              // Yellow
            { OFF,   X, OFF },              // Yellow flashing
            { OFF, OFF,  ON }, } } ,        // red

    {   7,                                  // nAspect 
        4,                                  // nLeds
        {   {  ON, OFF, OFF, OFF },         // red
            { OFF,  ON, OFF,  ON },         // double yellow
            { OFF,   X, OFF,   X },         // double yellow flashing
            { OFF,  ON, OFF, OFF },         // single yellow
            { OFF,   X, OFF, OFF },         // single yellow flashing
            { OFF, OFF,   X, OFF },         // red flashing
            { OFF, OFF,  ON, OFF }, },},    // RED
} ;

Aspect getAspect( uint8_t index )
{
    return aspects[index] ;
}


Signal::Signal(){
}

void Signal::begin( uint8_t _type, uint8_t _beginPin, uint8_t _ledCount )
{
    type        = _type ;
    beginPin    = _beginPin ;
    ledCount    = _ledCount ;
}


void Signal::update()
{
    uint32 currTime = millis() ;
    if( currTime - prevTime >= 1000 )  // CHANGE IN CONSTANT OR VARIABLE...
    {   prevTime = currTime ;

        for( int i = 0 ; i < ledCount ; i ++ )
        {
            // update all of led.
        }
    }
}

/* SETTERS */
uint8_t Signal::setAspectExt( uint16 dccAddress, uint8 _aspect )
{
    if( dccAddress == myAddress )
    {
        aspect = _aspect ;
        return 1 ;
    }
    return 0 ;
}

uint8_t Signal::setAspect( uint16 dccAddress, uint8 dir )
{
    uint16 beginAddress = myAddress ;
    uint16   endAddress = beginAddress + nAddresses -1 ;
    if( dccAddress >= beginAddress && dccAddress <=endAddress )
    {
        aspect = ((dccAddress - myAddress) % nAddresses) * 2 + dir ; // TEST ME
        printNumberln("Aspect set: ",aspect) ;
    }

}

void Signal::setType( uint8_t _type )
{
    type = _type ;
    ledCount = aspects[type].nLeds ;
    nAspects = aspects[type].nAspect ;
    nAddresses = (nAspects-1) / 2 + 1  ;
}

void Signal::setFirstIO( uint8_t _IO )
{
    beginPin = _IO ;
}

void Signal::setAddress( uint16 _address )
{
    myAddress = _address ;
}


/* GETTERS */
uint8_t Signal::getLedCount()
{
    return ledCount ;
}

uint8_t Signal::get1stPin()
{
    return beginPin ;
}

uint8_t Signal::getAspectAmount()
{
    return nAspects ;
}

uint16 Signal::getAddress()
{
    return myAddress ;
}

uint16 Signal::getAddressAmount()
{
    return( nAddresses ) ;
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