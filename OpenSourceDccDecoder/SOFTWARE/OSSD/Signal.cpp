#include "Signal.h"
#include "config.h"

const int nAspects = 30 ;
const static Aspect aspects[nAspects] PROGMEM =
{   // #0 DOUBLE COIL MODE
    {   2,                                  // nAspect
        2,                                  // nLeds
        {   { C },                          // OFF
            { C },                          // toggle
        },
    },

    // #1 SINGLE OUTPUT
    {   3,                                  // nAspect
        1,                                  // nLeds   SPECIAL single output only devices, 2nd address in use for optional blinking
        {   { OFF },                        // OFF
            {  ON },                        // ON
            {   X },                        // toggle
        },
    },

    // #2 SPECIAL RAILWAY CROSSING
    {   4,// TREE  Bl2   Bl1                // nAspect
        3,                                  // nLeds
        {   { OFF, OFF, OFF },              // AHOB OFF
            {  ON,   X,   X },              // AHOB ON  NOTE! need differentiating between the 2 blink leds
            {   X, OFF, OFF },              // AKI  OFF
            { OFF,   X,   X },              // AKI ON   NOTE! need differentiating between the 2 blink leds
        },
    },

    // #3 GENERIC TWO TONE GREEN AND RED SIGNAL
    {   2, //  R    G                       // nAspect // Simple 2 tone green and red, no blinking. Good for many signals
        2,                                  // nLeds
        {   { OFF,  ON },                   // Green
            {  ON, OFF },                   // Red
        },
    },

    // #4 DUTCH  STANDARD 3 TONE SIGNAL WITHOUT NUMBER DESIGNATOR
    {   5,//    R    Y    G                 // nAspect
        3,                                  // nLeds
        {   { OFF, OFF,  ON },              // Green
            { OFF, OFF,   X },              // Green flashing
            { OFF,  ON, OFF },              // Yellow
            { OFF,   X, OFF },              // Yellow flashing
            {  ON, OFF, OFF },              // red
        },
    } ,       
    // #5  DUTCH STANDARD 3 TONE SIGNAL WITH NUMBER DESIGNATOR
    {   8, //  W    R    Y    G                                // nAspect
        4,                                  // nLeds
        {   { OFF, OFF, OFF,  ON },         // Green
            { OFF, OFF, OFF,   X },         // Green flashing
            {  ON, OFF, OFF,   X },         // Green flashing w number
            { OFF, OFF,  ON, OFF },         // Yellow
            {  ON, OFF,  ON, OFF },         // Yellow w number
            { OFF, OFF,   X, OFF },         // Yellow flashing
            {   X, OFF,   X, OFF },         // Yellow flashing w number
            { OFF,  ON, OFF, OFF },         // red (RED BLINKING NOT IMPLEMENTED, OVERKILL, saves an address)
        },
    } ,  

    // #6 UK UK 4-TONE SIGNALING SYSTEM 
    {   6, //  R   Y2   G    Y1             // nAspect  
        4,                                  // nLeds
        {   { OFF, OFF,  ON, OFF },         // green
            { OFF,  ON, OFF,  ON },         // double yellow
            { OFF,   X, OFF,   X },         // double yellow flashing
            { OFF,  ON, OFF, OFF },         // single yellow
            { OFF,   X, OFF, OFF },         // single yellow flashing
            {  ON, OFF, OFF, OFF },         // red
        }, // NOTE: to add a white point indicator I recommend using a separate single output
    },

    // #7 GERMANY   GERMAN HP HAUPT SIGNALE ALSO IN SIGNALE OR CURVE SIGNAL.
    {   3,  // R    Y     G                 // nAspect
        3,                                  // nLeds
        {   { OFF, OFF,  ON },              // green
            { OFF,  ON,  ON },              // green yellow
            {  ON, OFF, OFF },              // red
        },
    }, // #8   GERMAN HP VOR SIGNALE
    {   3,   // Y2  G2   Y1   G1            // nAspect
        4,                                  // nLeds
        {   { OFF,  ON, OFF,  ON },         // green  green
            {  ON, OFF, OFF,  ON },         // yellow green
            {  ON, OFF,  ON, OFF },         // yellow yellow
            { OFF, OFF, OFF, OFF },         // None
        },
    }, // #9 GERMAN HP OUT SIGNAL WITH SHUNT
    {   5, //  S    R2   R1   Y    G        // nAspect
        5,                                  // nLeds
        {   { OFF, OFF, OFF, OFF,  ON },    // green
            { OFF, OFF, OFF,  ON,  ON },    // green yellow
            { OFF, OFF,  ON, OFF, OFF },    // 1x red
            {  ON, OFF,  ON, OFF, OFF },    // 1x red with shunt
            { OFF,  ON,  ON, OFF, OFF },    // 2x red
        },
    },

    // #10 BELGIAN HOME SIGNAL WITH SHUNT LEFT SIDE
    {   6,  // S    R    Y2   Y1   G        // nAspect  
        5,                                  // nLeds
        {   { OFF, OFF, OFF, OFF,  ON },    // green
            { OFF, OFF, OFF,  ON,  ON },    // yellow yellow
            { OFF, OFF,  ON,  ON, OFF },    // red
            {  ON,  ON, OFF, OFF, OFF },    // red + shunt
            { OFF, OFF, OFF,  ON,  ON },    // green + yellow  GrGH
            { OFF, OFF,  ON, OFF,  ON },    // green + yellow  GrGV
        },
    },// #11  BELGIAN HOME SIGNAL WITH SHUNT OPPOSING SIDE
    {   6,  // S    R    Y2   Y1   G        // nAspect
        5,                                  // nLeds
        {   { OFF, OFF, OFF, OFF,   X },    // green
            { OFF, OFF, OFF,   X,   X },    // yellow yellow
            { OFF, OFF,   X,  ON, OFF },    // red
            {   X,   X, OFF, OFF, OFF },    // red + shunt
            { OFF, OFF, OFF,   X,   X },    // green + yellow  GrGH
            { OFF, OFF,   X, OFF,   X },    // green + yellow  GrGV
        },
    },
    // #12 AUSTRIA MAIN SIGNAL
    {   4,  // R    Y     G   G        // nAspect
        4,                             // nLeds
        {   { OFF, OFF, OFF,  ON },    // green
            { OFF, OFF,  ON,  ON },    // green green
            { OFF,  ON, OFF,  ON },    // green yellow
            { OFF,  ON, OFF, OFF },    // red
        },
    },// #13    AUSTRIA MAIN SIGNAL WITH SHUNTING AND DEPARTURE LIGHTS
    {   7,  // Wp,  Ws, Gd,  R    Y     G    G        // nAspect  
        7,  //  p=permission, s=shunt d=depart        // nLeds
        {   { OFF, OFF, OFF, OFF, OFF, OFF,  ON },    // green
            { OFF, OFF, OFF, OFF, OFF,  ON,  ON },    // green green
            { OFF, OFF,   X, OFF, OFF, OFF,  ON },    // green + departure green
            { OFF, OFF, OFF, OFF,  ON, OFF,  ON },    // green yellow
            { OFF, OFF, OFF,  ON, OFF, OFF, OFF },    // red
            { OFF,  ON, OFF,  ON, OFF, OFF, OFF },    // red + double white shunting admitted
            {   X, OFF, OFF,  ON, OFF, OFF, OFF },    // red + white permission to pass at stop (flashing white)
        },
    }, // #14 AUSTRIA DISTANT SIGNAL
    {   4,  // Y    Y     G   G        // nAspect    
        4,                             // nLeds
        {   { OFF, OFF,  ON,  ON },    // green green
            { OFF,  ON,  ON,  ON },    // green green yellow
            {  ON,  ON, OFF,  ON },    // green yellow yellow
            {  ON,  ON, OFF, OFF },    // yellow yellow
        },
    },


    /*
    // SWISS


    // French

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

        if( currentAspect ) digitalWrite( GPIO[beginPin]  , HIGH ) ;
        else                digitalWrite( GPIO[beginPin+1], HIGH ) ;
    } 

    if( set == false && (millis() - prevTime) >= 100 ) // if time has expired, kill coils and clear set flag
    {   set  = true ;
        
        digitalWrite( GPIO[beginPin]  , LOW ) ;
        digitalWrite( GPIO[beginPin+1], LOW ) ;
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


            for( int led = 0 ; led <ledCount ; led ++ )
            {
                uint8   pin = GPIO[beginPin+led] ;
                uint8 state = localAspect.aspects[currentAspect][led] ; // left operand = row, right is COL  OUTPUT: ON, OFF or X
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
