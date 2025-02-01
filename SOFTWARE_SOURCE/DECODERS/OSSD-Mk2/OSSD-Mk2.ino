#include "src/macros.h"
#include "src/NmraDcc.h"
#include "config.h"
#include "CoilDriver.h"
#include "src/debounceClass.h"
#include "src/Trigger.h"
#include <EEPROM.h>
#include "src/LedBlink.h"



/******* SK NOTES **********
 
  Welcome to the new OSSD Mk2 source code.

 
 * TODO

    pwm speeds, must be fetched from EEPROM and send to the PWM controllers.
    This part may already work because the PWM speed is part of the coil array.
    In order to save the speed I merely have to set the speed once and store coil array and ofcourse 

    make helper functions to load en save data from EEPROM. As is now, EEPROM is too small for the entire coil array
    the array has grown too big

    go test things.
    The code does compile and should work

    Re-invent interlocking method. Only double pulse outputs may not be activated at the same time. I was thinking too add a 
    static token inside the update function. If a double pulse mode is active no other double pulse method may be activated.

    After an overcurrent, a lockout time should take place which keeps all outputs dorment for atleast 5 seconds or so.
*/



const int   nCoils    = 8 ;
uint8       coilIndex = 0 ;

NmraDcc     dcc ;
CoilDriver  coil[nCoils] ;
Debounce    configButton ;
LedBlink blueLed( blueLedPin) ;
LedBlink yellowLed( yellowLedPin ) ;

const int DEADBEEF_EE_ADDRESS ;

enum modeState
{
    idle,
    getBaseAddress,
    getIndex4Address,
    getIndex4Type,
    getUniqueAddress,
    getCoilType,
    configMode,
    getIndex4pulse,
    setPulseTime,
    getIndex4dutyCycle,
    setDutyCycle_,
    checkButton,
} ;

const int LOCO_FUNCTIONS_OFF    = 0 ;
const int FANTASTIC_FOUR        = 1 ;
const int EIGHT_BALL            = 2 ;

struct
{
    uint16 uniqueAddresses :  1 ;
    uint16          dccExt :  1 ; 
    uint16   locoFunctions :  2 ; // enable the usage of locomotive function. this can be 1 address with 16 functions or 4 addresses with 4 functions
    uint8      retainState :  1 ;
} settings;

struct // these struct is only used to save and load critical variable to and fom EEPROM
{
    uint16_t     type       :  3 ;
    uint16_t     stateA     :  1 ;
    uint16_t     stateB     :  1 ;
    uint16_t     dutyCycle  :  7 ;
    uint16_t     address    : 12 ;
    uint32_t     pulseTime ;
} coilSettings[nCoils] ;

const int EE_COILS = 0 ;
const int EE_SETTINGS = sizeof( coilSettings ) ;

void saveCoils()
{
    for( int i = 0 ; i < nCoils ; i ++ )
    {
        coilSettings[i].type       = coil[i].getType() ;
        coilSettings[i].stateA     = coil[i].getState( 0 ) ; // A
        coilSettings[i].stateB     = coil[i].getState( 1 ) ; // B
        coilSettings[i].dutyCycle  = coil[i].getDutyCycle( );
        coilSettings[i].address    = coil[i].getAddress() ;
        coilSettings[i].pulseTime  = coil[i].getPulseTime() ;
    }

    EEPROM.put( EE_COILS, coilSettings ) ;
}

void loadCoils()
{
    EEPROM.get( EE_COILS, coilSettings ) ;

    for( int i = 0 ; i < nCoils ; i ++ )
    {
        coil[i].setType(      coilSettings[i].type      ) ;
        if( settings.retainState ) 
            coil[i].setStates(coilSettings[i].stateA, coilSettings[i].stateB ) ;        
        coil[i].setDutyCycle( coilSettings[i].dutyCycle ) ;
        coil[i].setAddress(   coilSettings[i].address   ) ;
        coil[i].setPulseTime( coilSettings[i].pulseTime ) ;
    }
}

uint8                   state = idle ;
uint32                  beginTime ;
uint8                   index ;
volatile uint16         receivedAddress ;
volatile uint8          newAddressSet ;
volatile static uint32  lastTime ;

// calculuse max ADC value that correspond with 5A, 1024 adc pulses and 0.125R shunt resistor CHECK ME
static uint8 R_shunt  = 125 ;              // 0.0125R
static uint8 I_max    =   5 ;              // ampere
static uint8 V_max    =  R_shunt * I_max ; //  /100V
static uint8 ADC_max  =  ((uint32)1024 * V_max) / ((uint32)5 * 10000) ;

void determineLedPattern()
{
    uint8_t currentPreset = getPreset() ;

    if(state == idle) switch( currentPreset )
    {
        case DOUBLE_COIL_PULSED:        blueLed.bleep(  1 ) ; yellowLed.bleep(  1 ) ; break ;
        case DOUBLE_COIL_CONTINUOUSLY:  blueLed.turn(  ON ) ; yellowLed.turn(  ON ) ; break ;
        case SINGLE_COIL_PULSED:        blueLed.turn( OFF ) ; yellowLed.bleep(  1 ) ; break ;
        case SINGLE_COIL_CONTINUOUSLY:  blueLed.turn( OFF ) ; yellowLed.turn(  ON ) ; break ;
        case DOUBLE_PULSE_W_FROG:       blueLed.turn(  ON ) ; yellowLed.bleep(  2 ) ; break ;
        default:                        blueLed.bleep(  1 ) ; yellowLed.turn(  ON ) ; break ;
    }
    else switch(state)
    {
        default:               blueLed.turn( OFF ); yellowLed.turn( OFF ) ; break ;
        case getBaseAddress:   blueLed.bleep( 1 ) ; yellowLed.blinkSlow() ; break ;
        case getIndex4Address: blueLed.bleep( 1 ) ; yellowLed.blinkFast() ; break ;
        case getUniqueAddress: blueLed.bleep( 1 ) ; yellowLed.blinkSlow() ; break ;
        case getIndex4Type:    blueLed.bleep( 2 ) ; yellowLed.blinkFast() ; break ;
        case getCoilType:      blueLed.bleep( 2 ) ; yellowLed.blinkSlow() ; break ;
        case getIndex4pulse:   blueLed.bleep( 3 ) ; yellowLed.blinkFast() ; break ;
        case setPulseTime:     blueLed.bleep( 3 ) ; yellowLed.blinkSlow() ; break ;
        case configMode:       blueLed.bleep( 4 ) ; yellowLed.turn( OFF ) ; break ;
    }
}

// checks if all coils are of the same type, If so an preset is active.
uint8_t getPreset()
{
    uint8_t preset = coil[0].getType() ;

    for( int i = 1 ; i < nCoils ; i ++ )
    {
        if( coil[i].getType() != preset )
        {
            preset = 255 ; // no preset
            return preset ;
        }
    }
    return preset ;
}


void setup()
{
    configButton.begin( configPin ) ;

    for( int i = 0 ; i < 16 ; i ++ )
    {
        pinMode( GPIO[i], OUTPUT ) ;
    }
      blueLed.begin();
    yellowLed.begin();

    uint32 DEADBEEF ;
    EEPROM.get( DEADBEEF_EE_ADDRESS, DEADBEEF ) ;
    if( DEADBEEF != 0xDEADBEEF )
    {   DEADBEEF  = 0xDEADBEEF ;
        EEPROM.put( DEADBEEF_EE_ADDRESS, DEADBEEF ) ;
        
        for( int i = 0 ; i < nCoils ; i ++ )
        {
            coil[i].setType( DOUBLE_COIL_PULSED ) ;
            coil[i].setPulseTime( 50 ) ; // default to 50ms
            coil[i].setAddress( i+1 ) ;
            coil[i].setDutyCycle( 100 ) ;
        }
        saveCoils() ;

        settings.dccExt          = 0 ; 
        settings.locoFunctions   = LOCO_FUNCTIONS_OFF ;
        settings.uniqueAddresses = 0 ;
        settings.retainState     = 0 ;
        EEPROM.put( EE_SETTINGS, settings ) ;
    }

    EEPROM.get( EE_SETTINGS, settings ) ;
    loadCoils() ;

    for( int i = 0 ; i < nCoils ; i++ ) coil[i].begin() ; // initialize pwm controllers

    dcc.pin( 2, 0 ) ;
    dcc.init( MAN_ID_DIY, 11, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );
}


void loop()
{
    determineLedPattern(); 
    blueLed.update();
    yellowLed.update();

    dcc.process() ;

    config() ;

    REPEAT_MS( 1 )
    {
        int sample = analogRead( currentSensePin ) ;
        if( sample >= ADC_max ) // true => shortcircuit
        {
            for( int i = 0 ; i < nGpio ; i ++ )
            {
                digitalWrite( GPIO[i], LOW ) ;  // kill all outputs
            }

            blueLed.handleEvent(5) ;            // go flash LED's
            yellowLed.handleEvent(5) ;      
        }
    }
    END_REPEAT

    static uint8 index = 0 ;
    if( coil[index].update() == 1 ) iterate( index, nCoils ) ;
}

/* Dynamically allocate addresses 
To be called whenever a type is changed. The first address of the first object is starting point
returns the amount of used addresses;
*/
uint8_t squashAddresses()
{
    uint8  addressCount = nCoils ;
    uint16 startAddress = coil[0].getAddress() ;

    for( int i = 0 ; i < nCoils ; i ++ )
    {
        uint8 prevType = coil[i].getType() ;
        if( prevType == SINGLE_COIL_CONTINUOUSLY
        ||  prevType == SINGLE_COIL_PULSED ){ startAddress += 2 ; addressCount ++ ; }
        else                                { startAddress += 1 ; }
        coil[i+1].setAddress( startAddress ) ;
    }

    return addressCount ;
}


/***** CONFIGURATION MENU ********/
void config()
{
    static uint8  nextState = 0 ;
    static uint32 prevTime  = 0 ;
    static R_trigger trigger_1 ; // -> get address OR get index for address
    static R_trigger trigger_2 ; // -> get index for type
    static R_trigger trigger_3 ; // -> get index for pulse length
    static R_trigger trigger_4 ; // -> enable PWM dutycycle
    static R_trigger trigger_5 ; // -> option menu

    REPEAT_MS( 20 )
    {
        configButton.debounceInputs() ;
    }
    END_REPEAT

    uint8_t btnState = configButton.getState() ;

    if( btnState == FALLING && state != idle )
    {
        state = idle ;
        btnState = HIGH ;
    }


    int i ;
    switch( state )
    {
    case idle:
        if( btnState == FALLING ) // button is pressed
        {
            beginTime = millis() ;
            state     = checkButton ;
            nextState = getBaseAddress ;
            newAddressSet = 0 ;
        }
        break ;

    case checkButton:
        if( trigger_1.update( millis() - beginTime > 1000 ) ) { nextState = getBaseAddress     ; blueLed.handleEvent(1) ; }
        if( trigger_2.update( millis() - beginTime > 3000 ) ) { nextState = getIndex4Type      ; blueLed.handleEvent(2) ; }
        if( trigger_3.update( millis() - beginTime > 5000 ) ) { nextState = getIndex4pulse     ; blueLed.handleEvent(3) ; }
        if( trigger_4.update( millis() - beginTime > 7000 ) ) { nextState = getIndex4dutyCycle ; blueLed.handleEvent(4) ; }
        if( trigger_5.update( millis() - beginTime > 9000 ) ) { nextState = configMode         ; blueLed.handleEvent(5) ; }

        if( btnState == RISING ) state = nextState ;
        break ;


    case getBaseAddress:
        if( settings.uniqueAddresses ) state = getIndex4Address ; // if this flag is set, we do not use sequential address so we go to this state instead.

        if( newAddressSet == 1 )
        {   newAddressSet = 0 ;

            coil[0].setAddress( receivedAddress ) ;
            squashAddresses() ;
            saveCoils() ;
            state = idle ;
        }
        break ; 

    case getIndex4Address:
        if( newAddressSet == 1  )
        {   newAddressSet = 0 ;

            coilIndex = constrain( receivedAddress-1, 0, 7 ) ;
            state = getUniqueAddress ;
        }
        break ;

    case getUniqueAddress:
        if( newAddressSet == 1 ) // RESTRAIN VALUE TO ACCEPTABLE NUMBERS  
        {   newAddressSet = 0 ;

            coil[coilIndex].setAddress( receivedAddress ) ;
            saveCoils() ;
            state = getIndex4Address ;
        }
        break ;

    case getIndex4Type:
        if( newAddressSet == 1  )
        {   newAddressSet = 0 ;

            coilIndex = constrain( receivedAddress-1, 0, 7 ) ;
            state = getCoilType ;
        }
        break ;

    case getCoilType:
        if( newAddressSet == 1 ) // RESTRAIN VALUE TO ACCEPTABLE NUMBERS  
        {   newAddressSet = 0 ;

            uint8 type = receivedAddress-1 ;

            coil[coilIndex].setType( type ) ;
            squashAddresses() ;
            saveCoils() ;
            state = getIndex4Type ;
        }
        break ; 

    case getIndex4pulse:
        if( newAddressSet == 1  )
        {   newAddressSet = 0 ;

            coilIndex = constrain( receivedAddress-1, 0, 7 ) ;
            state = setPulseTime ;
        }
        break ;

    case setPulseTime:
        if( newAddressSet == 1 ) // RESTRAIN VALUE TO ACCEPTABLE NUMBERS  
        {   newAddressSet = 0 ;

            receivedAddress = constrain( receivedAddress, 3, 50 ) ; // constrain time to 5 seconds max 
            if( coil[coilIndex].getType() == DOUBLE_COIL_PULSED )   // the selected coil, has to be a double coil type for this
            for( i = 0 ; i < nCoils ; i ++ )                        // all double pulse coils, share the same pulse time
            {
                if( coil[i].getType() == DOUBLE_COIL_PULSED )       // check per coil, if the type is also double pulse and if so, adjust set the time.
                {
                    coil[i].setPulseTime( receivedAddress * 10 ) ;
                }
                saveCoils() ;
                state = getIndex4Type ;
                break ;
            }

            if( coil[coilIndex].getType() == SINGLE_COIL_PULSED ) // these don't share the same time
            {
                coil[coilIndex].setPulseTime( receivedAddress * 100 ) ;
            }
            state = getIndex4Type ;
        }
        break ;

    case getIndex4dutyCycle:
        if( newAddressSet == 1  )
        {   newAddressSet = 0 ;

            coilIndex = constrain( receivedAddress-1, 0, 8 ) ; // note with, 8 we can set all coils at same frequency at once.
            state = setDutyCycle_ ;
        }
        break ;

    case setDutyCycle_:
        if( newAddressSet == 1 )
        {   newAddressSet = 0 ;

            receivedAddress = constrain( receivedAddress, 1, 10 ) ; // 1 = 10%, 10 = 100%
            if( coilIndex < 8 )                       coil[coilIndex].setDutyCycle( receivedAddress ) ;
            else for( int i = 0 ; i < nCoils ; i ++ ) coil[i].setDutyCycle( receivedAddress ) ;
        }  
        break ;

    case configMode:
        if( newAddressSet == 1 )
        {   newAddressSet = 0 ;

            if( receivedAddress == 1 ) for( i = 0 ; i < nCoils ; i ++ ) // preset 1, all double pulse (default time 50ms)
            {
                coil[i].setType( DOUBLE_COIL_PULSED ) ;
                coil[i].setPulseTime( 50 ) ; // default to 50ms
                settings.retainState = 0 ;
            }

            if( receivedAddress == 2 ) for( i = 0 ; i < nCoils ; i ++ ) // preset 2, all double continously ( PM1 style motors of green red signals)
            {
                coil[i].setType( DOUBLE_COIL_CONTINUOUSLY ) ;
                settings.retainState = 1 ;
            }

            if( receivedAddress == 3 ) for( i = 0 ; i < nCoils ; i ++ )  // preset 3, single coils pulsed (16x) (default time set at 5s)
            {
                coil[i].setType( SINGLE_COIL_PULSED ) ;
                coil[i].setPulseTime( 5000 ) ; // default to 5 seconds
                settings.retainState = 0 ;
            }

            if( receivedAddress == 4 ) for( i = 0 ; i < nCoils ; i ++ ) // preset 4, single Coil continously (16x ON or OFF)
            {
                coil[i].setType( SINGLE_COIL_CONTINUOUSLY ) ;
                settings.retainState = 1 ;
            }

            if( receivedAddress ==  5 ) for( i = 0 ; i < (nCoils/2) ; i ++ ) // preset 5, 4x double coil (right side) and 4x double relay (left side) for dual relay frog juicing
            {
                coil[i].setType( DOUBLE_PULSE_W_FROG ) ;
                coil[i].setPulseTime( 50 ) ; // default to 50ms
                settings.retainState = 0 ;
            }

            if( receivedAddress == 20 ) settings.uniqueAddresses = 0 ; // default
            if( receivedAddress == 21 ) settings.uniqueAddresses = 1 ;
            
            if( receivedAddress == 30 ) settings.dccExt = 0 ;          // default
            if( receivedAddress == 31 ) settings.dccExt = 1 ;  // allow DCC EXTENDED TO CONTROL PULSE DURATION. (Note, this can perhaps be set via normal or ext instruction.)
            
            if( receivedAddress == 40 ) settings.locoFunctions = LOCO_FUNCTIONS_OFF ; // default
            if( receivedAddress == 41 ) settings.locoFunctions =         EIGHT_BALL ; // 16 points per address (for single mode, use F1 - F16
            if( receivedAddress == 42 ) settings.locoFunctions =     FANTASTIC_FOUR ; //  4 points per address (2 address per decoder) (special support for locomaus)

            if( receivedAddress == 50 ) settings.retainState  = 0 ; // default
            if( receivedAddress == 51 ) settings.retainState  = 1 ;

            squashAddresses() ;
            saveCoils() ; 
            EEPROM.put( EE_SETTINGS, settings ) ;
            state = idle ;
        }
        break ;
    }
}


/******** DCC CALLBACK FUNCTIONS **************/
void notifyDccSigOutputState( uint16_t address, uint8_t aspect ) // incomming DCC EXT commands
{
    // if( settings.dccExt == 0 )  return ;
    for( int i = 0 ; i < nCoils ; i ++ )
    {
        coil[i].setCoilExt( address, aspect ) ;
    }
}

void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output ) // incomming DCC commands
{
    if( millis() - lastTime >= 500 ) // create lockout time to prevent processing package more than 1x (used for config menu only)
    {   lastTime = millis() ;

        newAddressSet = 1 ;
        receivedAddress = address ;
    }

    if(  state != idle ) return ;
    if( output ==    0 ) return ;
    if( direction  > 0 ) direction = 1 ;

    for( int i = 0 ; i < nCoils ; i ++ )
    {
        coil[i].setCoil( address, direction ) ;
    }
}


// Locomotive function support
static uint16_t oldState = 0 ;
static uint16_t newState = 0 ;

void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{
    if( settings.locoFunctions == LOCO_FUNCTIONS_OFF ) return ;

    uint8_t  bitOffset       = 0 ;
    uint8_t  maxFunctions    = squashAddresses() ;      // check settings of all coils and determen how many addresses that are in use.
    uint16_t dccBaseAddress  = coil[0].getAddress() ;   // the address of the first coil determens to which loco address to listen to.
    uint8_t  neededAddresses = 1 ;                      // used for the Lok maus support. As we have 4 dcc addresses per loco address, we need more loco addresses per decoder 

    if( settings.locoFunctions == FANTASTIC_FOUR )      // lok maus support, F1-F4 of sequential loco addresses are used to control a decoder 
    {
        if( FuncGrp != FN_0_4 ) return ;

        neededAddresses = (maxFunctions - 1) / 4 + 1 ; 
        if (Addr < dccBaseAddress || Addr >= (dccBaseAddress + neededAddresses)) return ;

        bitOffset = (Addr - dccBaseAddress) * 4 ;
    }

    if( settings.locoFunctions == EIGHT_BALL ) // 1 loco address 16 functions max
    {
        if (Addr != dccBaseAddress) return ;

        if(      FuncGrp ==   FN_0_4 ) { bitOffset =  0 ; }
        else if( FuncGrp ==   FN_5_8 ) { bitOffset =  4 ; }
        else if( FuncGrp ==  FN_9_12 ) { bitOffset =  8 ; }
        else if( FuncGrp == FN_13_20 ) { bitOffset = 12 ; FuncState >>= 4 ; }
        else return ;
    }

    newState &= ~(0x0F << bitOffset) ;              // first clear the old bits
    newState |= (FuncState & 0x0F) << bitOffset ;   // than reload the newly received bits

    uint16_t changedBits = oldState ^ newState;     // if atleast 1 bit has changed, 
    oldState = newState ;
    if (changedBits == 0) return ;

    for( int i = 0 ; i < maxFunctions ; i++ ) // loop through all changedBits to set the coil.
    {
        uint16_t andMask = 1 << i ;
        if ((changedBits & andMask) == 0) continue ;

        uint8_t state = (newState >> i) & 0x1 ;

        uint16_t dccAddress = dccBaseAddress + i ; // calculate the appriopitate DCC address to be used in setCoil.

        for (int j = 0 ; j < nCoils ; j++ )
        {
            coil[j].setCoil( dccAddress, state ) ; // and feed the changed state of this address into all coils
        }
    }
}