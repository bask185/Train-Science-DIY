#include "src/macros.h"
#include "src/NmraDcc.h"
#include "config.h"
#include "src/debounceClass.h"
#include "src/stateMachineClass.h"
#include "src/CoilDrive.h"
#include <EEPROM.h>

// todo/ Fix the button, use new bouncy class?
// todo fill in LED code
//   I want OFF for charging
//   I want blinking for startup delay
//   I want blinking for getting address
//   I want blinking for getting pulse time
// todo add the ability to finetune the chargelevel. Default is 90% charge.
// todo fix all IO
// todo check/test myDccAddress
// todo test new EEPROM code
// todo check if I want actual states for the 

StateMachine sm ;
enum sm_states
{
    wait4startUpDelay,
    wait4charge,
    wait4command,
    settingCoil,
    wait4address,
    wait4switchTime,
} ;

const int EEPROM_START_ADDRESS = 1000;

struct Config
{
    uint16_t defaultVal;
    uint8_t  myDccAddress;
    uint16_t pulseTime;
    uint8_t  chargeLevel;
};

Config defaultConfig =
{
    0xCC,  // default
    0x01,  // myDccAddress
    50,    // pulseTime
    90     // chargeLevel
};

Config currentConfig;


uint8       currentCoilIndex ;
uint16      switchVoltage ;
const int   nPoints = 8 ;

const int charger = A0 ;
const int voltageMeasurePin = A1 ;

NmraDcc     dcc ;
Debounce    configButton ;
CoilDrive   coilDrive[nPoints] ;

void setup()
{
    coilDrive[0].setPins(1,2); // need pin array, than a for-loop
    coilDrive[0].setPulseTime( 100 );
    coilDrive[0].begin() ;

    sm.setState( wait4startUpDelay ) ;

    configButton.begin( configPin ) ;

    EEPROM.get( EEPROM_START_ADDRESS, currentConfig ) ;

    // Check if EEPROM has default values, if not, write them
    if( currentConfig.defaultVal != defaultConfig.defaultVal )
    {
        currentConfig = defaultConfig;
        EEPROM.put( EEPROM_START_ADDRESS, currentConfig ) ;
    }

    dcc.pin( 2, 0 ) ;
    dcc.init( MAN_ID_DIY, 11, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );
}

void statusLed()
{
    
}


StateFunction( wait4startUpDelay )
{
    if( sm.entryState() )
    {
        uint32 startUpDelay = (currentConfig.myDccAddress / 8) * 600 ; // start up delay increment 600ms per 8-fold decoder
        sm.setTimeout( startUpDelay ) ;
    }
    if( sm.onState() )
    {
        if( sm.timeout() )
        {
            return 1 ;
        }

        int newV = map( sample, 0, 1023, 0, 235 ) ;

        if( newV > 100 ) // if atleast 10V is measured.. return sooner.
        {
            return 1 ;
        }
    }
    
    return sm.endState() ;
}

StateFunction( getVoltageLevel )
{
    static int newV ;
    static int oldV ;

    if( sm.entryState() )
    {
        oldV = 0 ;
        digitalWrite( charger, HIGH ) ; // begin initial charging of capacitors
    }
    if( sm.onState() )
    {
        if( sm.repeat( 50 ) )
        {
            int sample = analogRead( voltageMeasurePin ) ;
            int newV = map( sample, 0, 1023, 0, 235 ) ;

            if( newV > 75 ) // if atleast 7.5V is measured..
            {
                if( newV > oldV ) { oldV = newV ; } // keep track of voltage rise
                else              {  sm.exit() ;  }
            }
        }
    }
    if( sm.exitState() )
    {
        switchVoltage = newV * 9 / 10 ; // 90%
    }

    return sm.endState() ;
}

StateFunction( wait4charge )
{
    if( sm.repeat( 50 ) )
    {
        int sample = analogRead( voltageMeasurePin ) ;
        int newV = map( sample, 0, 1023, 0, 235 ) ;
        if( newV >= switchVoltage ) { return 1 ; }
    }

    return 0 ;
}

StateFunction( wait4command )
{

    for( int i = 0 ; i < nPoints ; i ++ )
    {
        if( coilDrive[i].due() )
        {
            currentCoilIndex = i ;
            return 1 ;
        }
    } 
    
    return 0 ;
}

StateFunction( settingCoil )
{
    if( coilDrive[currentCoilIndex].update() )
    {
        return 1 ;
    }

    return 0 ;
}


StateFunction( wait4address ) // should this be states?
{
    if( sm.entryState() )
    {

    }
    if( sm.onState() )
    {

    }
    if( sm.exitState() )
    {

    }

    return sm.endState() ;
}

StateFunction( wait4switchTime )
{
    if( sm.entryState() )
    {

    }
    if( sm.onState() )
    {

    }
    if( sm.exitState() )
    {

    }

    return sm.endState() ;
}


void loop()
{
    statusLed() ;
    dcc.process() ;

    STATE_MACHINE_BEGIN( sm )
    {
        State( wait4startUpDelay ) {
            sm.nextState( wait4charge, 0 ) ; }


        State( wait4charge ) {
            sm.nextState( wait4command, 0 ) ; }

        State( wait4command ) {
            sm.nextState( settingCoil, 0 ) ; }

        State( settingCoil ) {
            sm.nextState( wait4charge, 0 ) ; }


        State( wait4address ) { // short button press -> wait to receive new address
            sm.nextState( wait4charge, 0 ) ; }

        State( wait4switchTime ) { // long button press -> wait to receive new switch activation time
            sm.nextState( wait4charge, 0 ) ; }
    }
    STATE_MACHINE_END( sm )
}


void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output ) // incomming DCC commands
{
    if( output == 0 ) return ;
    if( direction  > 0 ) direction = 1 ;
    
    if( sm.getState() == wait4address )
    {
        currentConfig.myDccAddress = constrain( address, 1,  2048 ) ;

        EEPROM.put( EEPROM_START_ADDRESS, currentConfig ) ;
        return ;
    }

    if( sm.getState() == wait4switchTime )
    {
        currentConfig.pulseTime = constrain( address, 30, 100 ) ;

        EEPROM.put( EEPROM_START_ADDRESS, currentConfig ) ;
        return ;
    }

    for( int i = 0 ; i < nPoints ; i ++ )
    {
        if( address >= currentConfig.myDccAddress 
        &&  address < (currentConfig.myDccAddress + nPoints) ) // if the address belong to me
        {
            coilDrive[i].setState( direction ) ;
        }
    }
}
