#include "src/io.h"
#include "src/macros.h"
#include <EEPROM.h>
#include "src/debounceClass.h"
#include "src/NmraDcc.h"
#include "src/ServoSweep.h"

enum
{
    initialize,
    configureServo,
} ;

uint8           index ;
const uint16    eeAddress = 1000 ;
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
    ServoSweep( servoPin1, defaultMin, defaultMax, 40 ),
    ServoSweep( servoPin2, defaultMin, defaultMax, 40 ),
    ServoSweep( servoPin3, defaultMin, defaultMax, 40 ),
    ServoSweep( servoPin4, defaultMin, defaultMax, 40 ),
    ServoSweep( servoPin5, defaultMin, defaultMax, 40 ),
    ServoSweep( servoPin6, defaultMin, defaultMax, 40 ),
    ServoSweep( servoPin7, defaultMin, defaultMax, 40 ),
    ServoSweep( servoPin8, defaultMin, defaultMax, 40 ),
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
    static Timer pressTimer( TIMER_ON, 1500 ) ;

    uint8_t stateDown = switches[DOWN].getState() ;
    uint8_t stateUp   = switches[ UP ].getState() ;

    if( switches[LEFT].getState() == FALLING )
    {
        servo[index].manualRelease() ;
        if( -- index == 255 ) index = 7 ;
        servo[index].toggle() ;
        blinkLed(1) ;
    }

    if( switches[RIGHT].getState() == FALLING )
    {
        servo[index].manualRelease() ;
        if( ++ index == 8 ) index = 0 ;
        servo[index].toggle() ;
        blinkLed(4) ;
    }

    REPEAT_MS( 100 )
    {
        if( stateDown == LOW )
        {
            servo[index].decrement() ;
            servo[index].manualRelease() ;
        }

        if( stateUp == LOW )
        {
            servo[index].increment() ;
            servo[index].manualRelease() ;
        }
    }
    END_REPEAT

    if( stateDown == RISING || stateUp == RISING ) // in either one of the 2 buttons is released, we release the signal.
    {
        servo[index].commitPos() ;
        blinkLed(1) ;
    }

    if( switches[SEL].getState() == FALLING )
    {
        servo[index].manualRelease() ;
        servo[index].toggle() ;
        blinkLed(5) ;
    }

    if( pressTimer.update( switches[SEL].getState() == LOW )) 
    {
        waiting4address = 1 ;
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
        else if( waiting4address )          blinkInterval =  333 ;
        //else if( state == configureServo )  blinkInterval = 1000 ;
        else                                blinkInterval =     1 ; // just normal idle mode ;

        if( blinkInterval == 1 ) PORTB &= ~(1 << 5) ;    // just be on, if there is nothing going on
        else                     PORTB ^=   1 << 5  ;    // toggle
    }
    END_REPEAT
}


bool deadbeef()
{
    const int DEADBEEF_EE_ADDRESS = 1000 ; 

    uint32_t  deadbeef_ ;
    EEPROM.get(DEADBEEF_EE_ADDRESS, deadbeef_) ;

    if( deadbeef_ != 0xDEADBEEF )
    {   deadbeef_  = 0xDEADBEEF ;

        EEPROM.put(DEADBEEF_EE_ADDRESS, deadbeef_) ;
        return true;
    }

    return false;
}

void setup()
{
    initIO() ;

    for (int i = 0; i < nServos; i++) { servo[i].useEEPROM() ; }

    if( deadbeef() )
    {
        for( int i = 0; i < nServos; i++ ) servo[i].init() ; // load default values when EEPROM is corrupted
        myAddress = 1 ;
        EEPROM.put( eeAddress, myAddress ) ;
    }

    for (int i = 0; i < nServos; i++) { servo[i].begin() ; }

    EEPROM.get( eeAddress, myAddress ) ;

    dcc.pin( 2, 0 ) ;
    dcc.init( MAN_ID_DIY, 11, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );
}

uint8_t enableIndex = 0 ;

void loop()
{
    if( enableIndex < nServos )
    {
        REPEAT_MS( 500 )
        {
            servo[enableIndex++].enable() ;
        }
        END_REPEAT
    }


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

void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output )
{
    if( output == 0 ) return ;

    if( waiting4address )
    {   waiting4address = 0 ;

        myAddress = address ;
        EEPROM.put( eeAddress, myAddress ) ;
        blinkLed(6);
        return ;
    }

    if( address < myAddress || address >= myAddress + nServos )
    {
        dccIndex = 0xFF ;
        return ;
    } 
   
    uint8 index = address - myAddress ;
    dccIndex = index ;

    if( direction >= 1 ) direction = 1 ;

    servo[index].manualRelease() ;
    servo[index].setState( direction ) ;
    blinkLed(3); 
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
    if( Dir == 1 ) servoSetpoint = map( Speed, 1, 128, 90,  20 ) ;
    else           servoSetpoint = map( Speed, 1, 128, 90, 160 ) ;
}

Trigger F0 ;
Trigger F1 ;
Trigger F2 ;
Trigger F3 ;
Trigger F4 ;

void notifyDccFunc( uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{
    if( Addr    !=   9999 ) return ;
    if( FuncGrp != FN_0_4 ) return ;

    F0.arm() ; F0.update( (FuncState & FN_BIT_00) ? 1 : 0 ) ;
    F1.arm() ; F1.update( (FuncState & FN_BIT_01) ? 1 : 0 ) ;
    F2.arm() ; F2.update( (FuncState & FN_BIT_02) ? 1 : 0 ) ;
    F3.arm() ; F3.update( (FuncState & FN_BIT_03) ? 1 : 0 ) ;
    F4.arm() ; F4.update( (FuncState & FN_BIT_04) ? 1 : 0 ) ;
} ;


void dccConfiguration()
{
    switch( state )
    {
    case initialize:
        if( F0.rose()
        &&  dccIndex      != 0xFF
        &&  servoSetpoint == 90 ) 
        {
            state = configureServo ;
        }
        break ;

    case configureServo:
        servo[dccIndex].manualOverride( servoSetpoint ) ;

        if( F0.isLow()   ) { blinkLed(1); servo[dccIndex].manualRelease() ; state = initialize ; dccIndex = 0xFF ; }
        if( F1.toggled() ) { blinkLed(1); servo[dccIndex].setMin( servoSetpoint ) ; }
        if( F2.toggled() ) { blinkLed(2); servo[dccIndex].setMax( servoSetpoint ) ; }

        if(servoSetpoint == 90 )
        {
            if( F3.toggled() ) { blinkLed(3); if( -- dccIndex ==     255 ) dccIndex = nServos - 1 ; servo[dccIndex].toggle() ; }
            if( F4.toggled() ) { blinkLed(4); if( ++ dccIndex == nServos ) dccIndex =           0 ; servo[dccIndex].toggle() ; }
        }
        break ;
    }
}
