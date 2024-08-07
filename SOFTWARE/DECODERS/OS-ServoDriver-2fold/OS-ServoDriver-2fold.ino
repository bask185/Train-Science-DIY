
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

const int nServos   = 2 ;
const int nSwitches = 4 ;

const int defaultMin =  30 ;
const int defaultMax = 150 ;

ServoSweep servo[ nServos ] =
{`
    ServoSweep( servoPin1, relayPin1, relayPin2, defaultMin, defaultMax, 20 ),
    ServoSweep( servoPin2, relayPin4, relayPin3, defaultMin, defaultMax, 20 ),
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
    UP,
    DOWN,
    SEL,
    TGL,
} ;

void processSwitches()
{
    static uint32 lastTime ;

    if( switches[TGL].getState() == FALLING )
    {
        index ^= 1 ;
        servo[index].setState( !servo[index].getState() ) ;
    }

    if( switches[DOWN].getState() == FALLING )
    {
        servo[index].decrement() ;
    }

    if( switches[UP].getState() == FALLING )
    {
        servo[index].increment() ;
    }

    if( switches[SEL].getState() == FALLING )
    {
        servo[index].setState( !servo[index].getState() ) ;
    }

    if( switches[SEL].getState() == LOW )
    {
        if( millis() - lastTime >= 1500 )
        {
            waiting4address = 1 ;
        }
    }
    else
    {
        lastTime = millis() ;
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

    dcc.pin( 2, 0 ) ;
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

