#include "src/io.h"
#include "src/macros.h"
#include <EEPROM.h>
#include "src/debounceClass.h"
#include "src/NmraDcc.h"
#include "src/ServoSweep.h"

enum
{
    initialize,
    F1_off,
    F2_off,
    speed_zero,

    configureServo,
} ;

uint8           index ;
const uint16    eeAddress = 0x50 ;
uint16          myAddress ;
uint8           waiting4address ;
uint8           state = initialize ;

NmraDcc dcc ;

const int nServos   = 8 ;
const int nSwitches = 5 ;

const int defaultMin =  80 ;
const int defaultMax = 100 ;

ServoSweep servo[nServos] =
{
    ServoSweep( servoPin1, defaultMin, defaultMax, 40, 1 ),
    ServoSweep( servoPin2, defaultMin, defaultMax, 40, 1 ),
    ServoSweep( servoPin3, defaultMin, defaultMax, 40, 1 ),
    ServoSweep( servoPin4, defaultMin, defaultMax, 40, 1 ),
    ServoSweep( servoPin5, defaultMin, defaultMax, 40, 1 ),
    ServoSweep( servoPin6, defaultMin, defaultMax, 40, 1 ),
    ServoSweep( servoPin7, defaultMin, defaultMax, 40, 1 ),
    ServoSweep( servoPin8, defaultMin, defaultMax, 40, 1 ),
} ;

Debounce switches[nSwitches] = 
{
    Debounce( switchPin0 ) ,
    Debounce( switchPin1 ) ,
    Debounce( switchPin2 ) ,
    Debounce( switchPin3 ) ,
    Debounce( switchPin4 ) ,
} ;

enum // switch order
{
    LEFT,
    UP,
    DOWN,
    RIGHT,
    SEL,
} ;


void processSwitches()
{
    static uint32 lastTime ;

    if( switches[LEFT].getState() == FALLING )
    {
        if( -- index == 255 ) index = 7 ;
        servo[index].setState( !servo[index].getState() ) ;
        blinkLed(1) ;
    }

    if( switches[RIGHT].getState() == FALLING )
    {
        if( ++ index == 8 ) index = 0 ;
        servo[index].setState( !servo[index].getState() ) ;
        blinkLed(4) ;
    }

    if( switches[DOWN].getState() == FALLING )
    {
        servo[index].decrement() ;
        blinkLed(3) ;
    }

    if( switches[UP].getState() == FALLING )
    {
        servo[index].increment() ;
        blinkLed(2) ;
    }

    if( switches[SEL].getState() == FALLING )
    {
        servo[index].setState( !servo[index].getState() ) ;
        blinkLed(5) ;
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

uint8_t     blinkCounter ;
uint8_t     blinkMax ;
uint8_t     flashing ;
uint32_t    blinkInterval = 500 ;

void blinkLed( uint8_t blinks ) 
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
              || state == speed_zero ) blinkInterval = 100 ;
        else if( state == configureServo )  blinkInterval = 1000 ;
        else    blinkInterval = 1 ; // just normal idle mode ;

        if( blinkInterval == 1 ) PORTB &= ~(1 << 5) ; // just be on, if there is nothing going on
        else                     PORTB ^=   1 << 5  ;    // toggle
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
    than F1 and F2 must be OFF and the speed must be set to 0
    than the servo follows the throttle. 
    With F1 you can store the CURVED position
    With F2 you can store the STRAIGH position
    If both F1 and F2 are set you can leave the mode by turning F0 OFF again.
*/
uint8 dccIndex = 0xFF ;
uint8 servoSetpoint  ;
uint8 F0 ;
uint8 F1 ;
uint8 F1_teach ;
uint8 F2 ;
uint8 F2_teach ;

void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output )
{
    if( output == 0 ) return ;

    if( waiting4address )
    {   waiting4address = 0 ;

        myAddress = address ;
        EEPROM.put( eeAddress, myAddress ) ;
        blinkLed(6);
    }

    else if( address >= myAddress && address < myAddress + 8 )
    {
        uint8 index = address - myAddress ;
        dccIndex = index ;

        if( direction >= 1 ) direction = 1 ;

        servo[index].manualRelease() ;
        servo[index].setState( direction ) ;
        blinkLed(3);
    }
    else dccIndex = 0xFF ;
}

// DCC extended directly controls a servo motor
void notifyDccSigOutputState( uint16_t address, uint8_t state )
{
    if( address >= myAddress && address < myAddress + 8 )
    {
        uint8 index = address - myAddress ;

        uint8 setpoint = map( state, 0, 255, 20,  160 ) ;
        servo[index].manualOverride( setpoint ) ;
    }
}



// CONFIG MODE BY DCC THROTTLE
void notifyDccSpeed( uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Speed, DCC_DIRECTION Dir, DCC_SPEED_STEPS SpeedSteps)
{
    if( Addr != 9999 ) return ;
    // is speed is negative (dir = 1?, whatever ) speed of 1-128 must be MAP'ed to 20-90 degrees
    // is speed is  postive (dir = 0?, whatever ) speed of 1-128 must be MAP'ed to 90-160 degrees

    if( Dir == 1 ) servoSetpoint = map( Speed, 1, 128, 90,  20 ) ;
    else           servoSetpoint = map( Speed, 1, 128, 90, 160 ) ;
}

void notifyDccFunc( uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{
    if( Addr    !=   9999 ) return ;
    if( FuncGrp != FN_0_4 ) return ;

    if( FuncState & FN_BIT_00 ) F0 = 1 ; else F0 = 0 ;
    if( FuncState & FN_BIT_01 ) F1 = 1 ; else F1 = 0 ;
    if( FuncState & FN_BIT_02 ) F2 = 1 ; else F2 = 0 ;   
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

            state ++ ;
        }
        break ;

    case F1_off:
        if( F1 == 0 ) state ++ ;
        break ;

    case F2_off:
        if( F2 == 0 ) state ++ ;
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

        if( F0 == 0 )
        {
            servo[dccIndex].manualRelease() ;
        }
        break ;
    }

    if( F0 == 0 ) state = initialize ; // leave the config mode
}
