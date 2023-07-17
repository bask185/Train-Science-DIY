#include "Signal.h"
#include "config.h"


/******* SK NOTES **********


*/

static uint8_t  currentAspect = 2 ;

// NOTE: as is now 1 aspect consumes 52 bytes. So I can add like 15 or so..

const Aspect aspects[15] =
{
    {   2,                                  // nAspect  SPECIAL double coils, default mode for points
        2,                                  // nLeds
        {   { C },                          // OFF
            { C }, }, },                    // toggle

    {   3,                                  // nAspect
        1,                                  // nLeds   SPECIAL single output only devices
        {   { OFF },                        // OFF
            {  ON },                        // ON
            {   X }, }, },                  // toggle

    {   4,                                  // nAspect  SPECIAL, RAILWAY CROSSINGS
        3,                                  // nLeds
        {   { OFF, OFF, OFF },              // AHOB OFF
            {  ON,   X,   X },              // AHOB ON  NOTE! need differentiating between the 2 blink leds
            { OFF,   X, OFF },              // AKI  OFF
            {   X, OFF,   X }, }, },        // AKI ON   NOTE! need differentiating between the 2 blink leds
            
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
            { OFF, OFF,  ON }, }, } ,        // red

    {   7,                                  // nAspect 
        4,                                  // nLeds
        {   {  ON, OFF, OFF, OFF },         // red
            { OFF,  ON, OFF,  ON },         // double yellow
            { OFF,   X, OFF,   X },         // double yellow flashing
            { OFF,  ON, OFF, OFF },         // single yellow
            { OFF,   X, OFF, OFF },         // single yellow flashing
            { OFF, OFF,   X, OFF },         // red flashing
            { OFF, OFF,  ON, OFF }, }, },    // RED

} ;

Aspect getAspect( uint8_t index )
{
    return aspects[index] ;
}


Signal::Signal()
{
}

void Signal::begin( uint8_t _type, uint8_t _beginPin, uint8_t _ledCount )
{
    type        = _type ;
    beginPin    = _beginPin ;
    ledCount    = _ledCount ;
}




uint8 Signal::updateCoils()
{
    uint8 set ;
    if( aspect != aspectPrev ) // state has changed, turn on a coil
    {  aspectPrev = aspect ;

        prevTime = millis() ;
        set = false ;

        if( aspect ) digitalWrite( beginPin  , HIGH ) ; // FIXME
        else         digitalWrite( beginPin+1, HIGH ) ; // FIXME
    }

    if( set == false && (millis() - prevTime) >= 100 ) // if time has expired, kill coils and clear set flag
    {   set  = true ;
        
        digitalWrite( beginPin  , LOW ) ;
        digitalWrite( beginPin+1, LOW ) ;
    }

    return set ;
}


// uint8_t aspects[maxAspect][maxLeds] ;
// const Aspect aspects[15] =
uint8 Signal::update()
{
    if( type > 0 )
    {
        uint32 currTime = millis() ;
        if( currTime - prevTime >= interval )  // CHANGE IN CONSTANT OR VARIABLE...
        {   prevTime = currTime ;

            uint8 led = 0 ;

            for( int i = 0 ; i < ledCount ; i ++ )
            {
                uint8 state = aspects[type].aspects[aspect][led++] ; // left operand = row, right is COL  OUTPUT: ON, OFF or X
                uint8   pin = GPIO[beginPin+led] ;
                switch( state )
                {
                    case  ON: digitalWrite( pin,  ON ) ; break ;
                    case OFF: digitalWrite( pin, OFF ) ; break ;
                    case   X: digitalWrite( pin, !digitalRead( pin )) ; break ;
                }                
            }
        }
        return true ;          // signal always return true
    }

    else
    {
        return  updateCoils() ;  // coild return false while being powered, to prevent more than one being triggered at once.
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
    uint16   endAddress = beginAddress + nAddresses - 1 ;

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