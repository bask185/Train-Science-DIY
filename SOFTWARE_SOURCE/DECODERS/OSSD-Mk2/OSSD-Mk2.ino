#include "src/macros.h"
#include "src/NmraDcc.h"
#include "config.h"
#include "CoilDriver.h"
#include "src/debounceClass.h"
#include "src/Trigger.h"
#include <EEPROM.h>
#include "src/LedBlink.h"



/******* SK NOTES **********
 * 
 * 
 * JE BENT BEZIG MET de LED BLINK CODE
 * 
 Welcome to the new Mk2 source code.
 The Mk2 will be able to work with an external CDU
 
 * TODO
  The capacitor level can be monitored. I want to measure atleast 16V before a point is set.
  I could make the decoder smart. If capacitor is found I can wait the first time to measure total voltage
  

  CONFIG MENU must be fixed.
  - all double pulse share the same pulse duration CHECK
  - unique address per coil should be added CHECK
  - loco functions for accessories.  UNSURE IF DESIRED 

  DEADBEEF CHECK and initialization values CHECK

*/

// config bits
const int   DCC_EXTENDED       = 0 ;
const int   UNIQUE_ADDRESSES   = 1 ;
// const int   bit2         = 2 ;

uint8       configBits ;
const int   nCoils    = 8 ;
uint8       coilIndex = 0 ;

NmraDcc     dcc ;
CoilDriver  coil[nCoils] ;
Debounce    configButton ;
LedBlink blueLed( blueLedPin) ;
LedBlink yellowLed( yellowLedPin ) ;

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

    checkButton, // not an actual mode
} ;

struct
{
    uint16 uniqueAddresses :  1 ;
    uint16          dccExt :  1 ;
    uint16   locoFunctions :  1 ; // enable the usage of locomotive function.
    uint16       eeAddress : 10 ;
} settings;


uint8                   state = idle ;
uint32                  beginTime ;
uint8                   index ;
volatile uint16         receivedAddress ;
volatile uint8          newAddressSet ;
volatile static uint32  lastTime ;

// calculuse max ADC value that correspond with 5A, 1023 adc pulses and 0.125R shunt resistor
static uint8 R_shunt  =  25 ;              // 0.025R
static uint8 I_max    =   5 ;              // ampere
static uint8 V_max    =  R_shunt * I_max ; //  /100V
static uint8 ADC_max  =  ((uint32)1023 * V_max) / ((uint32)5 * 10000) ;

uint8 preset = 0 ;  // 1-4 are used, 255, means no preset
uint8 blueBleep ;   // 0 is no bleep, 1, 2, 3, 4 bleeps are used. And 255 is ON
uint8 yellowF ;     // 0 is OFF, 255 is ON

// In deze functie stellen we de patronen in voor de 2 leds, gebaseerd op state en preset
void determineLedPattern()
{
    if(state == idle) switch(preset)
    {
        case DOUBLE_COIL_PULSED:        blueLed.bleep( 1 )  ; yellowLed.bleep( 1 ) ; break ;
        case DOUBLE_COIL_CONTINUOUSLY:  blueLed.turn(  ON ) ; yellowLed.turn( ON ) ; break ;
        case SINGLE_COIL_PULSED:        blueLed.turn( OFF ) ; yellowLed.bleep( 1 ) ; break ;
        case SINGLE_COIL_CONTINUOUSLY:  blueLed.turn( OFF ) ; yellowLed.turn( ON ) ; break ;
        default:                        blueLed.bleep( 1 )  ; yellowLed.turn( ON ) ; break ;
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

void getPreset()
{
    preset = coil[0].getType() ;
    for( int i = 1 ; i < nCoils ; i ++ )
    {
        if( coil[i].getType() != preset )
        {
            preset = 255 ; // no preset
            return ;
        }
    }
}


void setup()
{
    pinMode( 13, OUTPUT ) ;
    configButton.begin( configPin ) ;
    
    int eeAddress = 0 ;
    EEPROM.get( eeAddress, coil ) ; // load entire array from EEPROM

    for( int i = 0 ; i < 8 ; i ++ ) { coil[i].begin( GPIO[i*2], GPIO[i*2+1] ) ; }

      blueLed.begin();
    yellowLed.begin();

    uint32 DEADBEEF ;
    EEPROM.get( 1000, DEADBEEF ) ;
    if( DEADBEEF != 0xDEADBEEF )
    {   DEADBEEF  = 0xDEADBEEF ;
        EEPROM.put( 1000, DEADBEEF ) ;
        
        for( int i = 0 ; i < nCoils ; i ++ )
        {
            coil[i].setType( DOUBLE_COIL_PULSED ) ;
            coil[i].setPulseTime( 50 ) ; // default to 50ms
            coil[i].setAddress( i+1 ) ;
        }
        EEPROM.put( eeAddress, coil ) ;
    }
    getPreset() ; // determen if a preset is used (all coils same type)

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
            //killOutputs() ;
            delay( 5000 ) ;
        }
    }
    END_REPEAT

    
    static uint8 index = 0 ;
    if( coil[index].update() == 1 ) iterate( index, nCoils ) ;
}

/* Dynamically allocate addresses 
called whenever a type is changed. The first address of the first object is starting point
*/
void squashAddresses()
{
    uint16 startAddress = coil[0].getAddress() ;
    for( int i = 1 ; i < nCoils ; i ++ )
    {
        uint8 prevType = coil[i-1].getType() ;
        if( prevType == SINGLE_COIL_CONTINUOUSLY
        ||  prevType == SINGLE_COIL_PULSED ) startAddress += 2 ;
        else                                 startAddress += 1 ;
        coil[i].setAddress( startAddress ) ;
    }
}


/***** CONFIGURATION MENU ********/
void config()
{
    static uint8  nextState = 0 ;
    static uint32 prevTime  = 0 ;
    static R_trigger trigger_1s ; // -> get address OR get index for address
    static R_trigger trigger_2s ; // -> get index for type
    static R_trigger trigger_3s ; // -> get index for pulse length
    static R_trigger trigger_4s ; // -> option menu

    REPEAT_MS( 20 )
    {
        configButton.debounceInputs() ;
    }
    END_REPEAT

    uint8_t btnState = configButton.getState() ;

    if( btnState == FALLING && state != idle )
    {
        state = idle ;
        EEPROM.put( 0, coil ) ;
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
        if( trigger_1s.update( millis() - prevTime > 1000 ) ) { nextState = getBaseAddress ; blueLed.handleEvent(1) ; }
        if( trigger_2s.update( millis() - prevTime > 3000 ) ) { nextState = getIndex4Type ;  blueLed.handleEvent(2) ; }
        if( trigger_3s.update( millis() - prevTime > 5000 ) ) { nextState = getIndex4pulse ; blueLed.handleEvent(3) ; }
        if( trigger_4s.update( millis() - prevTime > 7000 ) ) { nextState = configMode ;     blueLed.handleEvent(4) ; }

        if( btnState == RISING ) state = nextState ;
        break ;


    case getBaseAddress:
        // if( settings.uniqueAddresses ) state = getIndex4Address ;
        if( newAddressSet == 1 )
        {   newAddressSet = 0 ;

            coil[0].setAddress( receivedAddress ) ;
            squashAddresses() ;
            EEPROM.put( 0, coil ) ;
            state = idle ;
        }
        break ; 

    case getIndex4Address:
        if( newAddressSet == 1  )
        {   newAddressSet = 0 ;

            coilIndex = receivedAddress-1 ;
            state = getUniqueAddress ;
        }
        break ;

    case getUniqueAddress:
        if( newAddressSet == 1 ) // RESTRAIN VALUE TO ACCEPTABLE NUMBERS  
        {   newAddressSet = 0 ;

            coil[coilIndex].setAddress( receivedAddress ) ;
            state = getIndex4Address ;
        }
        break ;

    case getIndex4Type:
        if( newAddressSet == 1  )
        {   newAddressSet = 0 ;

            coilIndex = receivedAddress-1 ;
            state = getCoilType ;
        }
        break ;

    case getCoilType:
        if( newAddressSet == 1 ) // RESTRAIN VALUE TO ACCEPTABLE NUMBERS  
        {   newAddressSet = 0 ;

            uint8 type = receivedAddress-1 ;

            coil[coilIndex].setType( type ) ;
            squashAddresses() ;
            getPreset() ;
            state = getIndex4Type ;
        }
        break ; 

    case getIndex4pulse:
        if( newAddressSet == 1  )
        {   newAddressSet = 0 ;

            coilIndex = receivedAddress-1 ;
            state = setPulseTime ;
        }
        break ;

    case setPulseTime:
        if( newAddressSet == 1 ) // RESTRAIN VALUE TO ACCEPTABLE NUMBERS  
        {   newAddressSet = 0 ;

            if( coil[coilIndex].getType() == DOUBLE_COIL_PULSED ) for( i = 0 ; i < nCoils ; i ++ ) // all double pulse coils, share the same pulse
            {
                if( coil[i].getType() == DOUBLE_COIL_PULSED ) coil[i].setPulseTime( receivedAddress * 10 ) ;
            }
            if( coil[coilIndex].getType() == SINGLE_COIL_PULSED ) // these don't share the same time
            {
                coil[i].setPulseTime( receivedAddress * 100 ) ;
            }
            state = getIndex4Type ;
        }
        break ; 

    case configMode:
        if( newAddressSet == 1 )
        {   newAddressSet = 0 ;

            if( receivedAddress == 1 ) for( i = 0 ; i < nCoils ; i ++ )
            {
                coil[i].setType( DOUBLE_COIL_PULSED ) ;
                coil[i].setPulseTime( 50 ) ; // default to 50ms
            }
            if( receivedAddress == 2 ) for( i = 0 ; i < nCoils ; i ++ )
            {
                coil[i].setType( DOUBLE_COIL_CONTINUOUSLY ) ;
            }
            if( receivedAddress == 3 ) for( i = 0 ; i < nCoils ; i ++ )
            {
                coil[i].setType( SINGLE_COIL_PULSED ) ;
                coil[i].setPulseTime( 5000 ) ; // default to 5 seconds
            }
            if( receivedAddress == 4 ) for( i = 0 ; i < nCoils ; i ++ )
            {
                coil[i].setType( SINGLE_COIL_CONTINUOUSLY ) ;
            }

            if( receivedAddress ==  5 ) ; // settings.uniqueAddresses = 1 ;
            if( receivedAddress ==  6 ) ; // settings.uniqueAddresses = 0 ;
            if( receivedAddress ==  7 ) ; // settings.dccExt = 1 ;
            if( receivedAddress ==  8 ) ; // settings.dccExt = 0 ;
            if( receivedAddress ==  9 ) ; // settings.locoFunctions = 1 ;
            if( receivedAddress == 10 ) ; // settings.locoFunctions = 0 ;

            squashAddresses() ;
            EEPROM.put( 0, coil ) ; // commit entire array
            //EEPROM.put(500, settings ) ;
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
    if( millis() - lastTime >= 500 ) // create lockout time of 500ms to prevent processing package more than 1x
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
