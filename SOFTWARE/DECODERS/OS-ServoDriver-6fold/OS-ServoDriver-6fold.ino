#include "src/io.h"
#include "src/macros.h"
#include <EEPROM.h>
#include "src/debounceClass.h"
#include "src/NmraDcc.h"
#include "src/ServoSweep.h"


uint8           index ;
const uint16    eeAddress = 0x50 ;
uint16          myAddress ;
uint8           waiting4address ;

NmraDcc dcc ;

const int nServos   = 6 ;
const int nSwitches = 4 ;

const int defaultMin =  80 ;
const int defaultMax = 100 ;

ServoSweep servo[nServos] =
{
    ServoSweep( servoPin1, defaultMin, defaultMax, 40, 1, relayPin1 ),
    ServoSweep( servoPin2, defaultMin, defaultMax, 40, 1, relayPin2 ),
    ServoSweep( servoPin3, defaultMin, defaultMax, 40, 1, relayPin3  ),
    ServoSweep( servoPin4, defaultMin, defaultMax, 40, 1, relayPin4  ),
    ServoSweep( servoPin5, defaultMin, defaultMax, 40, 1, relayPin5  ),
    ServoSweep( servoPin6, defaultMin, defaultMax, 40, 1, relayPin6  ),
} ;

Debounce switches[nSwitches] = 
{
    Debounce( switchPin0 ) ,
    Debounce( switchPin1 ) ,
    Debounce( switchPin2 ) ,
    Debounce( switchPin3 ) ,
} ;



enum
{
    SEL,
    TOGGLE,
    UP,
    DOWN,
} ;


void processSwitches()
{
    static uint32 lastTime ;

    if(   switches[DOWN].getState() == FALLING ) { servo[index].decrement() ; }
    if(     switches[UP].getState() == FALLING ) { servo[index].increment() ; }
    if( switches[TOGGLE].getState() == FALLING ) { servo[index].setState( !servo[index].getState() ) ; }

    if( switches[SEL].getState() == FALLING )
    {
        if( ++ index == nServos ) index = 0 ;
        servo[index].setState( !servo[index].getState() ) ;
        lastTime = millis() ;
    }

    if( switches[SEL].getState() == LOW )
    {
        if( millis() - lastTime >= 1500 )
        {
            waiting4address = 1 ;
        }
    }
}

void blinkLED()
{
    REPEAT_MS( 500 )
    {
        if( waiting4address ) digitalWrite( ledPin, !digitalRead( ledPin ) ) ;
        else                  digitalWrite( ledPin, LOW ) ;
    }
    END_REPEAT
}


void setup()
{
    initIO() ;

    EEPROM.get( eeAddress, myAddress ) ;
    if( myAddress == 0xFFFF )
    {
        myAddress = 1 ;
        EEPROM.put( eeAddress, myAddress ) ;
    }


    for (int i = 0; i < nServos; i++)
    {
        servo[i].useEEPROM() ;
        servo[i].begin() ;
    }

    dcc.pin( 2, 0 ) ; // TODO. add that I can get loco functions as well. need to alter library for that
    dcc.init( MAN_ID_DIY, 11, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );
}

void loop()
{
    blinkLED() ;
 
    for (int i = 0; i < nServos; i++)
    {
        servo[i].sweep() ;
    }

    REPEAT_MS( 50 )
    {
        for (int i = 0; i < nSwitches; i++)
        {
            switches[i].debounce() ;
        }
    }
    END_REPEAT

    processSwitches() ;

    dcc.process() ;
}

void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output )
{
    if( output == 0 ) return ;

    if( waiting4address )
    {   waiting4address = 0 ;

        myAddress = address ;
        EEPROM.put( eeAddress, myAddress ) ;
    }

    else if( address >= myAddress && address < myAddress + 8 )
    {
        uint8 index = address - myAddress ;

        if( direction >= 1 ) direction = 1 ;

        servo[index].setState( direction ) ;
    }
}
/*
TODO
* add state machine to configure device by throttle.
if TOGGLE is pressed for 1.5s, the mode should be entered.
The devices should already have it's addresses.

step 1. TGL must be pressed to enter config mode.

Step 2. index must be picked, by manually setting one of it's addresses by point DCC command.

Step 3. loco 9999 should have F1 and F2 turned OFF and speed must be 0

step 4. servo is now slaved to the throttle negative speed is  10 - 89 degreen. Positive speed is 91 - 170 degrees
step 4a. setting F1 stores the curved position,
step 4b. setting F2 stores the curved position,

goto to step 2 again. until any button is pressed again, than exit the mode


*/