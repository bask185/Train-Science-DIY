#include "src/io.h"
#include "src/macros.h"
#include <EEPROM.h>
#include "src/debounceClass2.h"
#include "src/NmraDcc.h"
#include "src/ServoSweep.h"

enum
{
    initialize,
    F1_off,
    F2_off,
    F3_off,
    speed_zero,

    configureServo,
} ;

uint8           index ;
const uint16    eeAddress = 0x50 ;
uint16          myAddress ;
uint8           waiting4address ;
uint8           state = initialize ;
NmraDcc         dcc ;

uint8           dccIndex = 0xFF ;
uint8           servoSetpoint  ;
uint8           F0 ;
uint8           F1 ;
uint8           F1_teach ;
uint8           F2 ;
uint8           F2_teach ;
uint8           F3 ;
uint8           F3_prev ;


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

Debouncer switches[nSwitches] = 
{
    Debouncer(),
    Debouncer(),
    Debouncer(),
    Debouncer(),
} ;


enum
{
    SEL,
    TOGGLE,
    UP,
    DOWN,
} ;

uint8     blinkCounter ;
uint8     blinkMax ;
uint8     flashing ;
uint32    blinkInterval = 500 ;

void blinkLed( uint8 blinks ) 
{
    blinkMax = 2*blinks-1 ;
    blinkCounter = 0 ;
    blinkInterval = 100 ;
    flashing = 1 ;
}

void statusLed()
{
    if( blinkCounter > blinkMax ) flashing = 0 ;

    REPEAT_MS( blinkInterval )
    {
        if( flashing )  blinkCounter ++ ;
        else if( waiting4address ) blinkInterval = 333 ;
        else if( state == F1_off 
              || state == F2_off 
              || state == F3_off 
              || state == speed_zero ) blinkInterval = 100 ;
        else if( state == configureServo )  blinkInterval = 1000 ;
        else    blinkInterval = 1 ; // just normal idle mode ;

        if( blinkInterval == 1 ) PORTB &= ~(1 << 5) ; // just be on, if there is nothing going on
        else                     PORTB ^=   1 << 5  ;    // toggle
    }
    END_REPEAT
}

void processSwitches()
{
    uint8 stateDown = switches[DOWN].state ;
    uint8 stateUp   = switches[ UP ].state ;

// ************ FINE TUNING SERVO POSITION ****************
    REPEAT_MS( 200 )
    {
        if( switches[DOWN].state == LOW && switches[UP].state == LOW )
        {
            servo[index].manualOverride( 90 ) ;
        }
        else if( switches[DOWN].state == LOW )
        {
            waiting4address = 0 ;
            servo[index].decrement() ;
            servo[index].manualRelease() ;
            blinkLed(1) ;
        }
        else if( switches[UP].state   == LOW )
        {
            waiting4address = 0 ;
            servo[index].increment() ; 
            servo[index].manualRelease() ;
            blinkLed(2) ;
        }
    }
    END_REPEAT

    if( stateDown == RISING || stateUp == RISING ) // in either one of the 2 buttons is released, we release the signal.
    {
        servo[index].commitPos() ;
    }

// ************ TOGGLE BUTTON, SHORT = TOGGLE STATE, LONG = TOGGLE RELAY ****************
    uint8 time = switches[TOGGLE].pressTime( 2000, 0 ) ;
    if( time == SHORT )
    { 
        waiting4address = 0 ;
        blinkLed(3) ;
        servo[index].manualRelease() ;
        servo[index].setState( !servo[index].getState() ) ;
    }
    else if( time == LONG )
    {
        waiting4address = 0 ;
        servo[index].toggleRelay() ;
        blinkLed( 5 ) ; 
    }

// ************ SELECT, SHORT = SELECT MOTOR, LONG = GET NEW ADDRESS (OR ABORT)
    time = switches[SEL].pressTime( 2000, 0 ) ;

    if( time == SHORT)
    {
        waiting4address = 0 ;
        blinkLed( 4 ) ;
        if( ++ index == nServos ) index = 0 ;
        servo[index].setState( !servo[index].getState() ) ;
    }

    else if( time == LONG )
    {
        waiting4address ^= 1 ;
        blinkLed(6) ;
    }
}

void setup()
{
    initIO() ;

    dcc.pin( 2, 0 ) ; // TODO. add that I can get loco functions as well. need to alter library for that
    dcc.init( MAN_ID_DIY, 11, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );

    switches[   SEL  ].setPin(switchPin0) ;
    switches[ TOGGLE ].setPin(switchPin1) ;
    switches[    UP  ].setPin(switchPin2) ;
    switches[  DOWN  ].setPin(switchPin3) ;

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



    blinkLed( 5 ) ;
}

void loop()
{
    statusLed() ;
 
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

    dccConfiguration() ;
}



// Following code adds DCC methods to configure the device.
// the servo sweep library has been slightly altered to work with this method
// servo positions can now be taught in using loco address 9999.
/*  
    You first need to control a servo using it's DCC address
    than you need to enable F0
    than F1, F2 and F3 must be OFF and the speed must be set to 0
    than the servo follows the throttle. 
    With F1 you can store the CURVED position
    With F2 you can store the STRAIGH position
    With F3 you can toggle the relay if it moves in the wrong direction.
    If both F1 and F2 are set you can leave the mode by turning F0 OFF again.
*/

void notifyDccAccTurnoutOutput( uint16 address, uint8 direction, uint8 output )
{
    if( output == 0 ) return ;

    if( waiting4address )
    {   waiting4address = 0 ;

        myAddress = address ;
        EEPROM.put( eeAddress, myAddress ) ;
        blinkLed(6);
    }

    else if( address >= myAddress && address < myAddress + nServos )
    {
        index = address - myAddress ;
        dccIndex = index ;

        if( direction >= 1 ) direction = 1 ;

        servo[index].setState( direction ) ;
        blinkLed(3);
    }
    else dccIndex = 0xFF ;
}

// CONFIG MODE BY DCC THROTTLE
void notifyDccSpeed( uint16 Addr, DCC_ADDR_TYPE AddrType, uint8 Speed, DCC_DIRECTION Dir, DCC_SPEED_STEPS SpeedSteps)
{
    if( Addr != 9999 ) return ;
    // is speed is negative (dir = 1?, whatever ) speed of 1-128 must be MAP'ed to 20-90 degrees
    // is speed is  postive (dir = 0?, whatever ) speed of 1-128 must be MAP'ed to 90-160 degrees

    if( Dir == 1 ) servoSetpoint = map( Speed, 1, 128, 90,  20 ) ;
    else           servoSetpoint = map( Speed, 1, 128, 90, 160 ) ;
}

void notifyDccFunc( uint16 Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8 FuncState)
{
    if( Addr    !=   9999 ) return ;
    if( FuncGrp != FN_0_4 ) return ;

    if( FuncState & FN_BIT_00 ) F0 = 1 ; else F0 = 0 ;
    if( FuncState & FN_BIT_01 ) F1 = 1 ; else F1 = 0 ;
    if( FuncState & FN_BIT_02 ) F2 = 1 ; else F2 = 0 ;   
    if( FuncState & FN_BIT_03 ) F3 = 1 ; else F3 = 0 ;   
} ;


void dccConfiguration()
{
    if( dccIndex == 0xFF )
    {
        state = initialize ;
        return ;
    }

    switch( state )
    {
    case initialize:
        if( F0 == 1 )
        {   // initialize some variables.
            F1_teach = 1 ;
            F2_teach = 1 ;
            F3_prev  = 0 ;

            state ++ ;
        }
        break ;

    case F1_off:
        if( F1 == 0 ) state ++ ;
        break ;

    case F2_off:
        if( F2 == 0 ) state ++ ;
        break ;

    case F3_off:
        if( F3 == 0 ) state ++ ;
        break ;

    case speed_zero:
        if( servoSetpoint == 90 ) state ++ ;
        break ;
        
    case configureServo:
        REPEAT_MS( 50 ) // update servo position
        {
            servo[dccIndex].manualOverride( servoSetpoint ) ;
        }
        END_REPEAT

        if( F1 && F1_teach ) 
        {         F1_teach = 0 ; 

            servo[dccIndex].setMin( servoSetpoint ) ;
            blinkLed( 2 ) ;
        }
        if( F2 && F2_teach ) 
        {         F2_teach = 0 ; 

            servo[dccIndex].setMax( servoSetpoint ) ;
            blinkLed( 3 ) ;
        }
        if( F3_prev != F3 )
        {   F3_prev  = F3 ;

            servo[dccIndex].toggleRelay() ;
            blinkLed( 4 ) ;
        }
        if( F0 == 0 )
        {
            servo[dccIndex].manualRelease() ;
        }
        break ;
    }

    if( F0 == 0 ) state = initialize ; // leave the config mode
}