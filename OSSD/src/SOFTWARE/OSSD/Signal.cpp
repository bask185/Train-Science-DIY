#include "Signal.h"
#include "config.h"

const int       maxAspect       = 30 ;
const int       maxLeds         =  8 ;

const int       OFF             = 0b00 ;
const int        ON             = 0b01 ;
const int         X             = 0b10 ; // flash
typedef struct Aspects 
{
    uint8   nAspect ;
    uint8   nLeds ;
    uint32  blinkInterval ;
    uint8 aspects[maxAspect][maxLeds] ;
} Aspect ;

const int nAspects  = 30 ;
const int NA        = 0 ;

/* NOTE ABOUT BLINK FREQUENCIES
The blink interval value is in x10ms. That means that a value of 10 result in 100ms blinking time.
The blink interval is the amount of time that a led is either high or low. 
This means that a full cycle takes 2x blink time.

If you want to adjust the blinking interval to a specific frequency you can calculate as follows

Dutch AKI example. Railway crossing has 45 blinks per minute. 
The frequency in Hz = 45/60s is 0.75Hz  ( = blinks per second)
The total cycle time T = 1 / 0.75 = 1.333s
The blink time should therefor be 1.333s / 2 = 0.667s
A value of 1 = 10ms or 0.01s.
So the blink time value should be 0.667 / 0.01 = 67

Simplified formula for blink time:

blinkTime = 3000 / Blinks_per_minute
*/


const static Aspect aspects[nAspects] PROGMEM =
{   // #1 DOUBLE COIL MODE
    {   2,                                  // nAspect
        2,                                  // nLeds
        // NA,                              // blink interval 
        // {   { NA },                      // set coil
        //     { NA },                      // set coil
        // },
    },
    // #2 SINGLE OUTPUT
    {   3,                                  // nAspect
        1,                                  // nLeds   SPECIAL single output only devices, 2nd address in use for optional blinking
        100,
        {   {  ON },                        // On
            { OFF },                        // OFF
            {   X },                        // toggle
        },
    },
    // #3 SPECIAL RAILWAY CROSSING
    {   2, //   B2   B1    W           // nAspect
        3,                                   // nLeds
        67,                                  // blink interval
        {   {  OFF, OFF,   X },              // AKI  OFF
            {    X,   X, OFF },              // AKI ON   NOTE! need differentiating between the 2 blink leds
        },
    },
    // #4 GENERIC TWO TONE GREEN AND RED SIGNAL
    {   2, //  R    G                       // nAspect // Simple 2 tone green and red, no blinking. Good for many signals
        2,                                  // nLeds
        NA,                                 // blink interval
        {   {  ON, OFF },                   // Green  // NEED FLASHING??
            { OFF,  ON },                   // Red
        },
    },
    // #5 DUTCH  STANDARD 3 TONE SIGNAL WITHOUT NUMBER DESIGNATOR
    {   5,//    G    Y    R                  // nAspect
        3,                                   // nLeds
        50,                                  // blink interval
        {   {   ON, OFF, OFF },              // Green
            {    X, OFF, OFF },              // Green flashing
            {  OFF,  ON, OFF },              // Yellow
            {  OFF,   X, OFF },              // Yellow flashing
            {  OFF, OFF,  ON },              // red
            {  OFF, OFF,   X },              // red flashing
        },
    } ,       
    // #6  DUTCH STANDARD 3 TONE SIGNAL WITH NUMBER DESIGNATOR
    {   9, //   G    Y    R    W             // nAspect
        4,                                   // nLeds
        50,                                  // blink interval
        {   {   ON, OFF, OFF, OFF },         // Green
            {    X, OFF, OFF, OFF },         // Green flashing
            {    X, OFF, OFF,  ON },         // Green flashing w number
            {  OFF,  ON, OFF, OFF },         // Yellow
            {  OFF,   X, OFF, OFF },         // Yellow flashing
            {  OFF,  ON, OFF,  ON },         // Yellow w number
            {  OFF,   X, OFF,   X },         // Yellow flashing w number
            {  OFF, OFF,  ON, OFF },         // red 
            {  OFF, OFF,   X, OFF },         // 
        },
    } ,  
    // #7 UK UK 4-TONE SIGNALING SYSTEM 
    {   6, //  Y1    G    Y2  RED            // nAspect  
        4,                                   // nLeds
        50,                                  // blink interval
        {   {  OFF,  ON, OFF, OFF },         // green
            {   ON, OFF,  ON, OFF },         // double yellow
            {    X, OFF,   X, OFF },         // double yellow flashing
            {  OFF, OFF,  ON, OFF },         // single yellow
            {  OFF, OFF,   X, OFF },         // single yellow flashing
            {  OFF, OFF, OFF,  ON },         // red
        }, // NOTE: to add a white point indicator I recommend using a separate single output
    },

    // #8 GERMAN HP OUT SIGNAL WITH SHUNT
    {   5, //    G   Y   R1   R2    S        // nAspect
        5,                                   // nLeds
        50,                                 // blink interval
        {   {   ON, OFF, OFF, OFF, OFF },    // green
            {   ON,  ON, OFF, OFF, OFF },    // green yellow
            {  OFF, OFF,  ON, OFF, OFF },    // 1x red
            {  OFF, OFF,  ON,  ON, OFF },    // 2x red
            {  OFF, OFF,  ON, OFF,  ON },    // 1x red with shunt
        },
    },

    // #9 GERMANY   GERMAN HP HAUPT SIGNALE ALSO IN SIGNALE OR CURVE SIGNAL.
    {   3,  //  G    Y     R                 // nAspect
        3,                                   // nLeds
        50,                                  // blink interval
        {   {   ON, OFF, OFF },              // green
            {   ON,  ON, OFF },              // green yellow
            {  OFF, OFF,  ON },              // red
        },
    },
    
    // #10 GERMAN HP HAUPT WITH VOR SIGNAL
    {   4,  //   G    R   G1v  Y1v  G2v Y2v     // nAspect
        6,                                      // nLeds
        NA,                                     // blink interval
        {   {    ON, OFF,  ON, OFF,  ON, OFF }, // green, expect green
            {    ON, OFF,  ON, OFF, OFF,  ON }, // green, expect yellow
            {    ON, OFF, OFF,  ON, OFF,  ON }, // green, expect red
            {   OFF,  ON, OFF, OFF, OFF, OFF }, // red,      off
        },
    },
    // #11   GERMAN HP VOR SIGNALE
    {   3,   // G1  Y1   G2   Y2             // nAspect
        4,                                   // nLeds
        50,                                 // blink interval
        {   {   ON, OFF,  ON, OFF },         // green  green
            {   ON, OFF, OFF,  ON },         // yellow green
            {  OFF,  ON, OFF,  ON },         // yellow yellow
        },
    }, 
   
    // #12 BELGIAN HOME SIGNAL WITH SHUNT LEFT SIDE
    {   6,  //  G    Y1    Y2   R   S        // nAspect
        5,                                   // nLeds
        50,                                 // blink interval
        {   {   ON, OFF, OFF, OFF, OFF },    // green
            {   ON,  ON, OFF, OFF, OFF },    // yellow yellow
            {  OFF,  ON,  ON, OFF, OFF },    // red
            {  OFF, OFF, OFF,  ON,  ON },    // red + shunt
            {   ON,  ON, OFF, OFF, OFF },    // green + yellow  GrGH
            {   ON, OFF,  ON, OFF, OFF },    // green + yellow  GrGV
        },
    },
    // #13  BELGIAN HOME SIGNAL WITH SHUNT OPPOSING SIDE
    {   6,  //  G    Y1    Y2   R   S        // nAspect
        5,                                   // nLeds
        50,                                  // blink interval
        {   {    X, OFF, OFF, OFF, OFF },    // green
            {    X,   X, OFF, OFF, OFF },    // yellow yellow
            {  OFF,  ON,   X, OFF, OFF },    // red
            {  OFF, OFF, OFF,   X,   X },    // red + shunt
            {    X,   X, OFF, OFF, OFF },    // green + yellow  GrGH
            {    X, OFF,   X, OFF, OFF },    // green + yellow  GrGV
        },
    },
    // #14 AUSTRIA MAIN SIGNAL
    {   4,  //   G    G    Y   R        // nAspect
        4,                              // nLeds
        50,                             // blink interval
        {   {   ON, OFF, OFF, OFF },    // green
            {   ON,  ON, OFF, OFF },    // green green
            {   ON, OFF,  ON, OFF },    // green yellow
            {  OFF, OFF, OFF,  ON },    // red
        },
    },
    // #15    AUSTRIA MAIN SIGNAL WITH SHUNTING AND DEPARTURE LIGHTS
    {   7,  //  G,   G,  Y,  R   Gd    Ws   Ws         // nAspect  
        7,  //  p=permission, s=shunt d=depart         // nLeds
        50,                                            // blink interval
        {   {  ON, OFF, OFF, OFF, OFF, OFF , OFF },    // green
            {  ON,  ON, OFF, OFF, OFF, OFF , OFF },    // green green
            {  ON, OFF, OFF, OFF,   X, OFF , OFF },    // green + departure green
            {  ON, OFF,  ON, OFF, OFF, OFF , OFF },    // green yellow
            { OFF, OFF, OFF,  ON, OFF, OFF , OFF },    // red
            { OFF, OFF, OFF,  ON, OFF,  ON , OFF },    // red + double white shunting admitted
            { OFF, OFF, OFF,  ON, OFF, OFF ,   X },    // red + white permission to pass at stop (flashing white)
        },
    }, 
    // #16 AUSTRIA DISTANT SIGNAL
    {   4,  // G    G     Y   Y             // nAspect    
        4,                                  // nLeds
        50,                                 // blink interval
        {   {   ON,  ON, OFF, OFF },        // green green
            {   ON,  ON,  ON, OFF },        // green green yellow
            {   ON, OFF,  ON,  ON },        // green yellow yellow
            {  OFF, OFF,  ON,  ON },        // yellow yellow
        },
    },
    /*
     G1
        Y1
     G2 Y3, R
     G4 Y2 
     G3
     */
    
    // # 17 SWISS HOME
    {   6,  //  G1   G2   G3   G4   Y1   Y2   Y3   R      // nAspect    
        7,                                                // nLeds
        NA,                                               // blink interval
        {   {   ON, OFF, OFF, OFF, OFF, OFF, OFF, OFF },  // Aspect 1 : The track ahead is clear.
            {   ON, OFF, OFF, OFF, OFF, OFF,  ON, OFF },  // Aspect 2 : Max speed 40 km/u
            {   ON, OFF, OFF,  ON, OFF, OFF, OFF, OFF },  // Aspect 3 : Max speed 60 km/h 
            {   ON,  ON,  ON, OFF, OFF, OFF, OFF, OFF },  // Aspect 5 : Max speed 90 km/h for the points in diverging position. 
            {  OFF, OFF, OFF, OFF,  ON,  ON, OFF, OFF },  // Aspect 6 : Max speed 40 km/h, the next signal shows 'stop' and is at reduced distance.
            {  OFF, OFF, OFF, OFF, OFF, OFF, OFF,  ON },  // ‘Stop’ Aspect
        },
    },
    // #18 SWISS DISTANT
    {   6,  //  G1   G2   Y1   Y2   Y3           // nAspect    
        7,                                       // nLeds
        NA,                                      // blink interval
        {   {   ON,  ON, OFF, OFF, OFF },        // Aspect 1 : The track ahead is clear.
            {   ON, OFF,  ON, OFF, OFF },        // Aspect 2 : Max speed 40 km/u
            {   ON,  ON,  ON, OFF, OFF },        // Aspect 3 : Max speed 60 km/h 
            {   ON,  ON, OFF,  ON, OFF },        // Aspect 5 : Max speed 90 km/h for the points in diverging position. 
            {   ON, OFF,  ON, OFF, OFF },        // Aspect 6 : Max speed 40 km/h, the next signal shows 'stop' and is at reduced distance.
            {  OFF, OFF, OFF,  ON,  ON },        // ‘Stop’ Aspect
        },
    },
    // 19 French
    {   8,  //  G,   Y1,  Y2,  R1, R2,           // nAspect    
        5,                                       // nLeds
        NA,                                      // blink interval
        {   {   ON, OFF, OFF, OFF, OFF },        // Green    
            {  OFF,  ON, OFF, OFF, OFF },       // YELLOW
            {  OFF,   X, OFF, OFF, OFF },       // YELLOW FLASH
            {  OFF,  ON,  ON, OFF, OFF },       // 2x YELLOW
            {  OFF,   X,   X, OFF, OFF },       // 2x YELLOW FLASH
            {  OFF, OFF, OFF,  ON, OFF },       // RED
            {  OFF, OFF, OFF,   X, OFF },       // RED FLASH
            {  OFF, OFF, OFF,  ON,  ON },       // 2x RED
        },
    },
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


enum coilStates 
{
    idle,
    setting,
    waiting,
    lockout,
    reset,

} ;

uint8 Signal::updateCoils()
{
    switch( sm )
    {
    case idle:
        if( currentAspect & 0x80 ) sm ++ ;
        return 1 ;

    case setting:
        if( currentAspect & 0x1 ) digitalWrite( GPIO[beginPin]  , HIGH ) ;
        else                      digitalWrite( GPIO[beginPin+1], HIGH ) ;

        prevTime = millis() ;
        sm ++ ;
        return 0 ;

    case waiting:
        if( millis() - prevTime >= 100 )
        {
            digitalWrite( GPIO[beginPin]  , LOW ) ;
            digitalWrite( GPIO[beginPin+1], LOW ) ;
            sm ++ ;
            prevTime = millis() ;
        }
        return 0 ;

    case lockout:
        if( millis() - prevTime >= 500 ) sm ++ ; // wait for repetive DCC messages to pass
        return 0 ;

    case reset:
        currentAspect &= 1 ;
        sm = idle ;
        return 0 ;
    }
}


uint8 Signal::update() // TODO: type 2 needs differentiating for blinking led 0 and led 1
{
    if( type > 0 )
    {
        memcpy_P( &localAspect, &aspects[type], sizeof( localAspect ) ) ; // keep or copy content to local signal variables

        uint32 currTime = millis() ;
        if( currTime - prevTime >= (10*localAspect.blinkInterval) )  // CHANGE IN CONSTANT OR VARIABLE...
        {   prevTime = currTime ;

            tglState ^= 1 ; // ensure leds keep in face

            for( int led = 0 ; led <ledCount ; led ++ )
            {
                uint8   pin = GPIO[beginPin+led] ;
                uint8 state = localAspect.aspects[currentAspect&0x7F][led] ; // left operand = row, right is COL  OUTPUT: ON, OFF or X

                switch( state )
                {
                    case  ON: digitalWrite( pin, HIGH ) ; break ;
                    case OFF: digitalWrite( pin,  LOW ) ; break ;
                    case   X: if( led == 1 && type == 2 )   digitalWrite( pin, tglState^1 ) ; // SK: in case of AKI/AHOB ensure that LEDs blink contra
                              else                          digitalWrite( pin, tglState )   ; break ;
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
