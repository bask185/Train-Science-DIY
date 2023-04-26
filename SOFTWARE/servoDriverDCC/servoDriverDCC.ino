#include "src/ServoSweep.h"
#include "src/DCC_Decoder.h"
#include <EEPROM.h>

/* TODOs
 * Test DCC bus
 * check if needed to modify low/high values
 * add method to inverse direction? if needed
 * add the config code to set myAddress
 * 
 * */

const int servoPin = 1 ;
const int relayPin = 2 ;
const int speed = 100 ;
const int MIN_VAL =  70 ;
const int MAX_VAL = 110 ; 

// EEPROM ADDRESSES
const int LAST_STATE = 0 ;
const int LOW_POS  = 1 ;
const int HIGH_POS = 2 ;
const int ADDRESS = 3 ;

// ServoSweep( uint8_t _servoPin, uint8_t _min, uint8_t _max, uint8_t _speed, uint8_t _turnOff, uint8_t _relayPin ) ;  // constructor 2
ServoSweep motor ;

uint16_t myAddress ;
uint8_t configMode ;

void setup()
{
    uint8_t lastKnownState  = EEPROM.read( LAST_STATE ) ;
    uint8_t lowPos          = EEPROM.read( LOW_POS ) ;
    uint8_t highPos         = EEPROM.read( HIGH_POS ) ;
    EEPROM.get( ADDRESS, myAddress ) ;

    if( lowPos < MIN_VAL ||  lowPos > MAX_VAL // chech if EEPROM data is within bounds
    || highPos < MIN_VAL || highPos > MAX_VAL
    || myAddress == 0 )
    {
        myAddress = 1 ;                      // if not ok, load defaut values
        lowPos  = 85 ;
        highPos = 95 ;

        EEPROM.write(  ADDRESS, myAddress ) ;
        EEPROM.write(  LOW_POS,  lowPos ) ;
        EEPROM.write( HIGH_POS, highPos ) ;
    }

    motor.begin( servoPin, lowPos, highPos, lastKnownState ) ; // fetch last pos from EEPROM
    DCC.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 ) ;

    /* PINB |= 1 << cnfButton ;*/
}

void loop() 
{
    DCC.process() ;
    motor.sweep() ;

    if( bitRead( PORTB, 1/*cfg pin*/) == 0 ) // perhaps use long press to configure position?
    {
        configMode = 1 ;
    }
}

void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output ) // incomming DCC commands
{
    if( output != 0 ) return ;

    switch( configMode )
    {
    case 0:
        if( address != myAddress ) return ;
        if( direction > 0 ) direction = 1 ;

        motor.setState( direction ) ;
        EEPROM.write( LAST_STATE, direction ) ;
        break ;

    case 1:
        myAddress = address ;
        EEPROM.write( ADDRESS, myAddress ) ;
        configMode = 0 ;
        break ;

    case 2:
        {                                                                       // extra scope needed for local variable ;
            uint8_t newPos = map( address, LOW_POS, HIGH_POS ) ;

            if( motor.getState() == 1 )
            {
                motor.setMax( newPos ) ;
                EEPROM.write( HIGH_POS, newPos ) ;
            }
            else
            {
                motor.setMin( newPos ) ;
                EEPROM.write( LOW_POS, newPos ) ;
            }
        }
        break ;
    }
}