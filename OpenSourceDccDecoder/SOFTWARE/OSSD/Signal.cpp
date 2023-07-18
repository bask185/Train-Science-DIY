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
            {   X, OFF, OFF },              // AKI  OFF
            { OFF,   X,   X },              // AKI ON   NOTE! need differentiating between the 2 blink leds
        },
    },

    {   2,                                  // nAspect // Simple 2 tone green and red, no blinking. Good for many signals
        2,                                  // nLeds
        {   {  ON, OFF },                   // Green
            { OFF,  ON },                   // Red
        },
    },

    // DUTCH
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

    // UK
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

    // GERMANY
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

    // BELGIAN
    {   6,  // S    R    Y2   Y1   G        // nAspect  Belgian home signal with shunt LEFT SIDE
        5,                                  // nLeds
        {   { OFF, OFF, OFF, OFF,  ON },    // green
            { OFF, OFF, OFF,  ON,  ON },    // yellow yellow
            { OFF, OFF,  ON,  ON, OFF },    // red
            {  ON,  ON, OFF, OFF, OFF },    // red + shunt
            { OFF, OFF, OFF,  ON,  ON },    // green + yellow  GrGH
            { OFF, OFF,  ON, OFF,  ON },    // green + yellow  GrGV
        },
    },
    {   6,  // S    R    Y2   Y1   G        // nAspect  Belgian home signal with shunt OPPOSING SIDE, same as above but with blinking instead
        5,                                  // nLeds
        {   { OFF, OFF, OFF, OFF,   X },    // green
            { OFF, OFF, OFF,   X,   X },    // yellow yellow
            { OFF, OFF,   X,  ON, OFF },    // red
            {   X,   X, OFF, OFF, OFF },    // red + shunt
            { OFF, OFF, OFF,   X,   X },    // green + yellow  GrGH
            { OFF, OFF,   X, OFF,   X },    // green + yellow  GrGV
        },
    },

    // AUSTRIA
    /*
    MAIN ASPECTS
    RED
    GREEN
    GREEN GREEN
    GREEN YELLOW

    DISTANT
    GREEN GREEN
    GREEN GREEN YELLOW
    off (main is red)
    green yellow yellow
    yellow yellow

    AUSTRA 'SUPER' SIGNAL
    Sams as above but with 4 extra lights for departure (green) and pass red allowed, and
    RED
    RED + double white shunting admitted
    RED + white permission to pass at stop (flashing white)
    GREEN
    GREEN + flashing green departure allowed
    GREEN GREEN
    GREEN YELLOW

    // SWISS



    */


} ;

static Aspect localAspect ;



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

        if( currentAspect ) digitalWrite( beginPin  , HIGH ) ;
        else                digitalWrite( beginPin+1, HIGH ) ;
    } 

    if( set == false && (millis() - prevTime) >= 100 ) // if time has expired, kill coils and clear set flag
    {   set  = true ;
        
        digitalWrite( beginPin  , LOW ) ;
        digitalWrite( beginPin+1, LOW ) ;
    }

    return set ;
}


uint8 Signal::update() // TODO: type 2 needs differentiating for blinking led 0 and led 1
{
    if( type > 0 )
    {
        memcpy_P( &localAspect, &aspects[type], sizeof( localAspect ) ) ; // keep or copy content to local signal variables

        uint32 currTime = millis() ;
        if( currTime - prevTime >= interval )  // CHANGE IN CONSTANT OR VARIABLE...
        {   prevTime = currTime ;

            for( int led = 0 ; led < ledCount ; led ++ )
            {
                uint8 state = localAspect.aspects[currentAspect][led++] ; // left operand = row, right is COL  OUTPUT: ON, OFF or X
                uint8   pin = GPIO[beginPin+led] ;
                switch( state )
                {
                    case  ON: digitalWrite( pin, HIGH ) ; break ;
                    case OFF: digitalWrite( pin,  LOW ) ; break ;
                    case   X: digitalWrite( pin, !digitalRead( pin )) ; break ;
                }                
            }
        }
        return true ;          // signal always return true
    }

    else
    {
        return updateCoils() ;  // coils return false while being powered, to prevent more than one being triggered at once.
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

    memcpy_P( &localAspect , &aspects[type], sizeof( localAspect ) ) ;

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