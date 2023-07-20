#include "Signal.h"
#include "config.h"

const int nAspects = 30 ;
const static Aspect aspects[nAspects] PROGMEM =
{   // #1 DOUBLE COIL MODE
    {   2,                                  // nAspect
        2,                                  // nLeds
        // {   { C },                          // set coil
        //     { C },                          // set coil
        // },
    },

    // #2 SINGLE OUTPUT
    {   3,                                  // nAspect
        1,                                  // nLeds   SPECIAL single output only devices, 2nd address in use for optional blinking
        {   {  ON },                        // On
            { OFF },                        // OFF
            {   X },                        // toggle
        },
    },

    // #3 SPECIAL RAILWAY CROSSING
    {   4,// TREE  Bl2   Bl1                // nAspect
        3,                                  // nLeds
        {   {  OFF, OFF, OFF },              // AHOB OFF
            {    X,   X,  ON },              // AHOB ON  NOTE! need differentiating between the 2 blink leds
            {  OFF, OFF,   X },              // AKI  OFF
            {    X,   X, OFF },              // AKI ON   NOTE! need differentiating between the 2 blink leds
        },
    },

    // #4 GENERIC TWO TONE GREEN AND RED SIGNAL
    {   2, //  R    G                       // nAspect // Simple 2 tone green and red, no blinking. Good for many signals
        2,                                  // nLeds
        {   {  ON, OFF },                   // Green  // NEED FLASHING??
            { OFF,  ON },                   // Red
        },
    },

    // #4 DUTCH  STANDARD 3 TONE SIGNAL WITHOUT NUMBER DESIGNATOR
    {   5,//    G    Y    R                  // nAspect
        3,                                   // nLeds
        {   {   ON, OFF, OFF },              // Green
            {    X, OFF, OFF },              // Green flashing
            {  OFF,  ON, OFF },              // Yellow
            {  OFF,   X, OFF },              // Yellow flashing
            {  OFF, OFF,  ON },              // red
        },
    } ,       
    // #5  DUTCH STANDARD 3 TONE SIGNAL WITH NUMBER DESIGNATOR
    {   8, //   G    Y    R    W             // nAspect
        4,                                   // nLeds
        {   {   ON, OFF, OFF, OFF },         // Green
            {    X, OFF, OFF, OFF },         // Green flashing
            {    X, OFF, OFF,  ON },         // Green flashing w number
            {  OFF,  ON, OFF, OFF },         // Yellow
            {  OFF,  ON, OFF,  ON },         // Yellow w number
            {  OFF,   X, OFF, OFF },         // Yellow flashing
            {  OFF,   X, OFF,   X },         // Yellow flashing w number
            {  OFF, OFF,  ON, OFF },         // red (RED BLINKING NOT IMPLEMENTED, OVERKILL, saves an address)
        },
    } ,  

    // #6 UK UK 4-TONE SIGNALING SYSTEM 
    {   6, //  Y1    G    Y2  RED            // nAspect  
        4,                                   // nLeds
        {   {  OFF,  ON, OFF, OFF },         // green
            {   ON, OFF,  ON, OFF },         // double yellow
            {    X, OFF,   X, OFF },         // double yellow flashing
            {  OFF, OFF,  ON, OFF },         // single yellow
            {  OFF, OFF,   X, OFF },         // single yellow flashing
            {  OFF, OFF, OFF,  ON },         // red
        }, // NOTE: to add a white point indicator I recommend using a separate single output
    },

    // #7 GERMANY   GERMAN HP HAUPT SIGNALE ALSO IN SIGNALE OR CURVE SIGNAL.
    {   3,  //  G    Y     R                 // nAspect
        3,                                   // nLeds
        {   {   ON, OFF, OFF },              // green
            {   ON,  ON, OFF },              // green yellow
            {  OFF, OFF,  ON },              // red
        },
    },
    // #8 GERMANY   GERMAN HP HAUPT WITH VOR SIGNAL
    {   3,  //   G    R   Gv   Yv           // nAspect
        4,                                  // nLeds
        {   {    ON, OFF,  ON, OFF },       // green, expect green
            {    ON, OFF, OFF,  ON },       // green, expect red
            {   OFF,  ON, OFF, OFF },       // red,      off
        },
    }, // #9   GERMAN HP VOR SIGNALE
    {   3,   // G1  Y1   G2   Y2             // nAspect
        4,                                   // nLeds
        {   {   ON, OFF,  ON, OFF },         // green  green
            {   ON, OFF, OFF,  ON },         // yellow green
            {  OFF,  ON, OFF,  ON },         // yellow yellow
            {  OFF, OFF, OFF, OFF },         // None
        },
    }, // #10 GERMAN HP OUT SIGNAL WITH SHUNT
    {   5, //    G   Y   R1   R2    S        // nAspect
        5,                                   // nLeds
        {   {   ON, OFF, OFF, OFF, OFF },    // green
            {   ON,  ON, OFF, OFF, OFF },    // green yellow
            {  OFF, OFF,  ON, OFF, OFF },    // 1x red
            {  OFF, OFF,  ON, OFF,  ON },    // 1x red with shunt
            {  OFF, OFF,  ON,  ON, OFF },    // 2x red
        },
    },

    // #11 BELGIAN HOME SIGNAL WITH SHUNT LEFT SIDE
    {   6,  //  G    Y1    Y2   R   S        // nAspect
        5,                                   // nLeds
        {   {   ON, OFF, OFF, OFF, OFF },    // green
            {   ON,  ON, OFF, OFF, OFF },    // yellow yellow
            {  OFF,  ON,  ON, OFF, OFF },    // red
            {  OFF, OFF, OFF,  ON,  ON },    // red + shunt
            {   ON,  ON, OFF, OFF, OFF },    // green + yellow  GrGH
            {   ON, OFF,  ON, OFF, OFF },    // green + yellow  GrGV
        },
    },// #12  BELGIAN HOME SIGNAL WITH SHUNT OPPOSING SIDE
    {   6,  //  G    Y1    Y2   R   S        // nAspect
        5,                                   // nLeds
        {   {    X, OFF, OFF, OFF, OFF },    // green
            {    X,   X, OFF, OFF, OFF },    // yellow yellow
            {  OFF,  ON,   X, OFF, OFF },    // red
            {  OFF, OFF, OFF,   X,   X },    // red + shunt
            {    X,   X, OFF, OFF, OFF },    // green + yellow  GrGH
            {    X, OFF,   X, OFF, OFF },    // green + yellow  GrGV
        },
    },
    // #13 AUSTRIA MAIN SIGNAL
    {   4,  //   G    G    Y   R        // nAspect
        4,                              // nLeds
        {   {   ON, OFF, OFF, OFF },    // green
            {   ON,  ON, OFF, OFF },    // green green
            {   ON, OFF,  ON, OFF },    // green yellow
            {  OFF, OFF, OFF,  ON },    // red
        },
    },// #14    AUSTRIA MAIN SIGNAL WITH SHUNTING AND DEPARTURE LIGHTS
    {   7,  //  G,   G,  Y,  R   Gd    Ws   Ws         // nAspect  
        7,  //  p=permission, s=shunt d=depart         // nLeds
        {   {  ON, OFF, OFF, OFF, OFF, OFF , OFF },    // green
            {  ON,  ON, OFF, OFF, OFF, OFF , OFF },    // green green
            {  ON, OFF, OFF, OFF,   X, OFF , OFF },    // green + departure green
            {  ON, OFF,  ON, OFF, OFF, OFF , OFF },    // green yellow
            { OFF, OFF, OFF,  ON, OFF, OFF , OFF },    // red
            { OFF, OFF, OFF,  ON, OFF,  ON , OFF },    // red + double white shunting admitted
            { OFF, OFF, OFF,  ON, OFF, OFF ,   X },    // red + white permission to pass at stop (flashing white)
        },
    }, // #15 AUSTRIA DISTANT SIGNAL
    {   4,  // G    G     Y   Y         // nAspect    
        4,                              // nLeds
        {   {   ON,  ON, OFF, OFF },    // green green
            {   ON,  ON,  ON, OFF },    // green green yellow
            {   ON, OFF,  ON,  ON },    // green yellow yellow
            {  OFF, OFF,  ON,  ON },    // yellow yellow
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
    set         = true ;
}




uint8 Signal::updateCoils()
{
    if( currentAspect != aspectPrev && set == true ) // state has changed, turn on a coil
    {  
        currentAspect &= 1 ;
        aspectPrev = currentAspect ;

        prevTime = millis() ;
        set = false ;

        if( currentAspect ) digitalWrite( GPIO[beginPin]  , HIGH ) ;
        else                digitalWrite( GPIO[beginPin+1], HIGH ) ;
    } 

    if( set == false && (millis() - prevTime) >= 100 ) // if time has expired, kill coils and clear set flag
    {   set  = true ;
        
        digitalWrite( GPIO[beginPin]  , LOW ) ;
        digitalWrite( GPIO[beginPin+1], LOW ) ;
        currentAspect &= 1 ;
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
                uint8 state = localAspect.aspects[currentAspect&0x7F][led] ; // left operand = row, right is COL  OUTPUT: ON, OFF or X
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
        if( type == 0 ) currentAspect |= 0x80 ;
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
