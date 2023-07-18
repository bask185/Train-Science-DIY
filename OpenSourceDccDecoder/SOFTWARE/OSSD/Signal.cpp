#include "Signal.h"
#include "config.h"

const int nAspects = 30 ;
const static Aspect aspects[nAspects] PROGMEM =
{
    {   2,                                  // nAspect  SPECIAL double coils, default mode for points
        2,                                  // nLeds
        {   { C },                          // OFF
            { C },                          // toggle
        },
    },

    {   3,                                  // nAspect
        1,                                  // nLeds   SPECIAL single output only devices, 2nd address in use for optional blinking
        {   { OFF },                        // OFF
            {  ON },                        // ON
            {   X },                        // toggle
        },
    },

    {   4,                                  // nAspect  SPECIAL, RAILWAY CROSSINGS
        3,                                  // nLeds
        {   { OFF, OFF, OFF },              // AHOB OFF
            {  ON,   X,   X },              // AHOB ON  NOTE! need differentiating between the 2 blink leds
            { OFF,   X, OFF },              // AKI  OFF
            {   X, OFF,   X },              // AKI ON   NOTE! need differentiating between the 2 blink leds
        },
    },

    {   2,                                  // nAspect // Simple 2 tone green and red, no blinking. Good for H/V block signale
        2,                                  // nLeds
        {   {  ON, OFF },                   // Green
            { OFF,  ON },                   // Red
        },
    },

    {   5,                                  // nAspect  // Standard 3 tone dutch signal without number designator
        3,                                  // nLeds
        {   {  ON, OFF, OFF },              // Green
            {   X, OFF, OFF },              // Green flashing
            { OFF,  ON, OFF },              // Yellow
            { OFF,   X, OFF },              // Yellow flashing
            { OFF, OFF,  ON },              // red
        },
    } ,        

    {   8,                                  // nAspect  // Standard 3 tone dutch signal with number designator
        4,                                  // nLeds
        {   { OFF,  ON, OFF, OFF },         // Green
            { OFF,   X, OFF, OFF },         // Green flashing
            {  ON,   X, OFF, OFF },         // Green flashing w number
            { OFF, OFF,  ON, OFF },         // Yellow
            {  ON, OFF,  ON, OFF },         // Yellow w number
            { OFF, OFF,   X, OFF },         // Yellow flashing
            {   X, OFF,   X, OFF },         // Yellow flashing w number
            { OFF, OFF, OFF,  ON },         // red (RED BLINKING NOT IMPLEMENTED, OVERKILL, saves an address)
        },
    } ,  

    {   6,                                  // nAspect  UK 4-tone signaling system NOTE: to add a white point indicator I recommend using a separate single output
        4,                                  // nLeds
        {   {  ON, OFF, OFF, OFF },         // red
            { OFF,  ON, OFF,  ON },         // double yellow
            { OFF,   X, OFF,   X },         // double yellow flashing
            { OFF,  ON, OFF, OFF },         // single yellow
            { OFF,   X, OFF, OFF },         // single yellow flashing
            { OFF, OFF,  ON, OFF },         // green
        },
    },

    {   3,                                  // nAspect  German H/V Haupt signale also In signale or curve signal.
        3,                                  // nLeds
        {   { OFF, OFF,  ON },              // green
            { OFF,  ON,  ON },              // green yellow
            {  ON, OFF, OFF },              // red
        },
    },

    {   3,                                  // nAspect  German H/V Vor signale
        4,                                  // nLeds
        {   { OFF,  ON, OFF,  ON },         // green  green
            {  ON, OFF, OFF,  ON },         // yellow green
            {  ON, OFF,  ON, OFF },         // yellow yellow
            { OFF, OFF, OFF, OFF },         // None
        },
    },

    {   3,                                  // nAspect  German H/V Out signal
        5,                                  // nLeds
        {   { OFF, OFF, OFF, OFF,  ON },    // green
            { OFF, OFF, OFF,  ON,  ON },    // green yellow
            {  ON, OFF,  ON, OFF, OFF },    // 1x red with shunt
            { OFF,  ON,  ON, OFF, OFF },    // 2x red
        },
    },

    {   6,                                  // nAspect  Belgian home signal DO ME PLEASE
        5,                                  // nLeds
        {   { OFF, OFF, OFF, OFF,  ON },    // green
            { OFF, OFF, OFF,  ON,  ON },    // yellow yellow
            {  ON, OFF,  ON, OFF, OFF },    // red
            { OFF,  ON,  ON, OFF, OFF },    // red + shunt
            { OFF,  ON,  ON, OFF, OFF },    // green + yellow right GrGH
            { OFF,  ON,  ON, OFF, OFF },    // green + yellow down  GrGV
        },
    },

} ;

static Aspect localAspect ;

// Aspect getAspect( uint8_t index )
// {
//     memcpy_P(aspect , &aspects[index], sizeof( aspect ) ) ;

//     return aspect[index] ;
// }


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
    if( currentAspect != aspectPrev ) // state has changed, turn on a coil
    {  aspectPrev = currentAspect ;

        prevTime = millis() ;
        set = false ;

        if( currentAspect ) digitalWrite( beginPin  , HIGH ) ; // FIXME what's wrong?
        else                digitalWrite( beginPin+1, HIGH ) ; // FIXME
    } 

    if( set == false && (millis() - prevTime) >= 100 ) // if time has expired, kill coils and clear set flag
    {   set  = true ;
        
        digitalWrite( beginPin  , LOW ) ;
        digitalWrite( beginPin+1, LOW ) ;
    }

    return set ;
}


uint8 Signal::update()
{
    if( type > 0 )
    {
        memcpy_P( localAspect , &aspects[type], sizeof( localAspect ) ) ;

        uint32 currTime = millis() ;
        if( currTime - prevTime >= interval )  // CHANGE IN CONSTANT OR VARIABLE...
        {   prevTime = currTime ;

            for( int led = 0 ; led < ledCount ; led ++ )
            {
                uint8 state = localAspect.aspects[currentAspect][led++] ; // left operand = row, right is COL  OUTPUT: ON, OFF or X
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
        currentAspect = _aspect ;
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
        currentAspect = ((dccAddress - myAddress) % nAddresses) * 2 + dir ; // TEST ME
        printNumberln("Aspect set: ",currentAspect) ;
    }
}

void Signal::setType( uint8_t _type )
{
    type = _type ;

    memcpy_P( localAspect , &aspects[type], sizeof( localAspect ) ) ;

    ledCount = localAspect.nLeds ;
    nAspects = localAspect.nAspect ;

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

uint8_t Signal::getType()
{
    return type ;
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