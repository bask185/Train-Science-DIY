#include "src/macros.h"
#include "src/NmraDcc.h"
#include "config.h"
#include "CoilDriver.h"
#include "pwm.h"
#include "src/debounceClass.h"
#include "src/Trigger.h"
#include "src/Timers.h"
#include <EEPROM.h>
#include "src/LedBlink.h"



/******* SK NOTES **********
 
  Welcome to the new OSSD Mk2 source code.

LASTWORK:
I redid the PWM part to a more hardcoded approach. It need to be tested thoroughly.
coilClass now can handle it's own pulse timing. PWM for any pulsed type is therefor no longer possible.addressReceived


TODO:
The new PWM part need a solution to work with DORMENT outputs.

TEST EVERYTHING
rename yellow and blue led to leftLed and rightLed.  They're both red anyways  DONER

enforce faster update when an output is set. Especially a set coil has specific timing requirements. DONE, TEST ME!


BRAINWAVES:
We could also let coil class handle it's own pulse timer for pulsed outputs and reserve PWM especially for steady outputs DONE, TEST ME!
Perhaps that the coilClass should only do it's own pulse timer for double pulse timer, for single types it is not that important. DONE TEST ME

 
*/




const int   nCoils    = 8 ;
uint8       coilIndex = 0 ;

NmraDcc     dcc ;
CoilDriver  coil[nCoils] ;
Debounce    configButton ;
LedBlink    rightLed( rightLedPin) ;
LedBlink    leftLed(  leftLedPin ) ;
BlinkTimer  commitStates ;

const int DEADBEEF_EE_ADDRESS    = 1020 ; // 1020, 1021, 1022, 1023
const int CDU_RESET_EE_ADDRESS   = 1019 ;
const int MAX_VOLTAGE_EE_ADDRESS = 1018 ;

enum modeState
{
    idle,
    setBaseAddress,
    setIndex4Address,
    setIndex4Type,
    setUniqueAddress,
    setCoilType,
    configMode,
    setIndex4pulse,
    setPulseTime,
    setIndex4dutyCycle,
    setDutyCycle_,
    checkButton,
} ;

const int LOCO_FUNCTIONS_OFF = 0 ;
const int FANTASTIC_FOUR     = 1 ;
const int EIGHT_BALL         = 2 ;

struct
{
    uint16 uniqueAddresses :  1 ;
    uint16          dccExt :  1 ; 
    uint16   locoFunctions :  2 ; // enable the usage of locomotive function. this can be 1 address with 16 functions or 4 addresses with 4 functions
    uint16   maxCduVoltage : 12 ;
    uint16          hasCdu :  1 ; // needed? or autodetect? In essence with constant power supply, the software should ever switch.
    uint16          rcn213 :  1 ; // address offset for multimaus icw old roco boosters. Addresses are to be substracted with 4 when option is OFF.
    // I used double addresses so any system can turn on or off this setting. Address 1001 and 997 both turn it ON
    // addresses 1000 and 996 turn it OFF.
    // the default = ON
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
        coilSettings[i].type        = coil[i].getType() ;        /*Serial.print(F("type:      "));Serial.println(coilSettings[i].type);*/
        coilSettings[i].stateA      = coil[i].getState( 0 ) ;    /*Serial.print(F("stateA:    "));Serial.println(coilSettings[i].stateA);*/// A
        coilSettings[i].stateB      = coil[i].getState( 1 ) ;    /*Serial.print(F("stateB:    "));Serial.println(coilSettings[i].stateB);*/// B
        coilSettings[i].dutyCycle   = coil[i].getDutyCycle( );   /*Serial.print(F("dutycycle: "));Serial.println(coilSettings[i].dutyCycle);*/
        coilSettings[i].address     = coil[i].getAddress() ;     /*Serial.print(F("address:   "));Serial.println(coilSettings[i].address);*/
        coilSettings[i].pulseTime   = coil[i].getPulseTime() ;   /*Serial.print(F("pulse time:"));Serial.println(coilSettings[i].pulseTime);*/
    }

    // Serial.println(F("committing coil settings 2 EEPROM"));
    EEPROM.put( EE_COILS, coilSettings ) ;
}

void loadCoils()
{
    EEPROM.get( EE_COILS, coilSettings ) ; // fetch all data from EEPROM

    coil[0].setBuddyPins( GPIO[14], GPIO[15] ) ; // especially for preset 5, double coil w frog relay
    coil[1].setBuddyPins( GPIO[12], GPIO[13] ) ;
    coil[2].setBuddyPins( GPIO[10], GPIO[11] ) ;
    coil[3].setBuddyPins( GPIO[ 8], GPIO[ 9] ) ;

    for( int i = 0 ; i < nCoils ; i ++ )
    {
        coil[i].setType(      coilSettings[i].type ) ; 
        coil[i].setStates(    coilSettings[i].stateA, coilSettings[i].stateB ) ; // Serial.print("setting states: ") ; Serial.print(coilSettings[i].stateA);Serial.print(" <A  B> ") ; Serial.println(coilSettings[i].stateB);
        coil[i].setDutyCycle( coilSettings[i].dutyCycle ) ;
        coil[i].setAddress(   coilSettings[i].address   ) ;
        coil[i].setPulseTime( coilSettings[i].pulseTime ) ;
        coil[i].begin( GPIO[ 2*i ], GPIO[ (2*i)+1 ] ) ;
        coil[i].initializeCoils() ;
    }
}

uint8                   runMode = 1 ;
uint32                  lockoutTime ;

uint8                   state = idle ;
uint32                  beginTime ;
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
        case DOUBLE_COIL_PULSED:        rightLed.bleep(  1 ) ; leftLed.bleep(  1 ) ; break ; // both bleep
        case DOUBLE_COIL_CONTINUOUSLY:  rightLed.turn(  ON ) ; leftLed.turn(  ON ) ; break ; // both on
        case SINGLE_COIL_PULSED:        rightLed.turn( OFF ) ; leftLed.bleep(  1 ) ; break ; //  1 bleep
        case SINGLE_COIL_CONTINUOUSLY:  rightLed.turn( OFF ) ; leftLed.turn(  ON ) ; break ; //  1 on
        case DOUBLE_PULSE_W_FROG:       rightLed.turn(  ON ) ; leftLed.bleep(  2 ) ; break ; // 1 on and 2 bleeps
        default:                        rightLed.bleep(  1 ) ; leftLed.turn(  ON ) ; break ; // 1 bleep and 1 on
    }

    else switch(state)
    {
        //default:                    rightLed.turn( OFF ); leftLed.turn(        OFF ) ; break ; // where did I need this for?
        case checkButton:           rightLed.turn( OFF ) ;leftLed.turn( OFF ) ;        break ; // blue led does event blinking

        case setIndex4Address:      rightLed.bleep( 1 ) ; leftLed.turn( OFF ) ; break ;
        case setIndex4Type:         rightLed.bleep( 2 ) ; leftLed.turn( OFF ) ; break ;
        case setIndex4pulse:        rightLed.bleep( 3 ) ; leftLed.turn( OFF ) ; break ;
        case setIndex4dutyCycle:    rightLed.bleep( 4 ) ; leftLed.turn( OFF ) ; break ;
        case configMode:            rightLed.bleep( 5 ) ; leftLed.turn( OFF ) ; break ;

        case setBaseAddress:        rightLed.bleep( 1 ) ; leftLed.turn( ON ) ; break ;
        case setUniqueAddress:      rightLed.bleep( 1 ) ; leftLed.turn( ON ) ; break ; // perhaps blink slow, to separate from setBaseAddress
        case setCoilType:           rightLed.bleep( 2 ) ; leftLed.turn( ON ) ; break ;
        case setPulseTime:          rightLed.bleep( 3 ) ; leftLed.turn( ON ) ; break ;
        case setDutyCycle_:         rightLed.bleep( 4 ) ; leftLed.turn( ON ) ; break ;
    }
}

void saveStates()
{
   // if( settings.retainState == 0 ) return ; 

    // every so many seconds or so save the states of continous outputs to EEPROM.
    if( commitStates.update(1) )
    {        
       saveCoils() ;
    }
}

// checks if all coils are of the same type, If so an preset is active.
uint8_t getPreset()
{
    uint8_t preset = coil[0].getType() ;

    uint8_t testCoils = nCoils ;
    if( preset == DOUBLE_PULSE_W_FROG ) testCoils = nCoils / 2 ; //  this mode, has last 4 objects disabled

    for( int i = 1 ; i < testCoils ; i ++ )
    {
        if( coil[i].getType() != preset )
        {
            preset = 255 ; // no preset
            return preset ;
        }
    }
    return preset ;
}

void updateCduVoltage()
{
    // SK: todo, if the decoder sees a lower incomming voltage for a longer period of time (5 seconds orso, it should lower it's own maxCDU voltage


    static BlinkTimer timer( 2000 ) ;
    
    if( timer.update(1) )
    {    
        uint16 voltage = analogRead( coilVoltagePin) ;
        if( voltage > settings.maxCduVoltage + 3 )
        {
            settings.maxCduVoltage = voltage ;
            EEPROM.put( EE_SETTINGS, settings ) ;
        }
    }
}


void setup()
{
    // delay(1);
    // Serial.begin(115200);
    // delay(1);
    // Serial.println(F("booting OSSD MK2"));

    commitStates.setTime( 1000 ) ;
    configButton.begin( configPin ) ;

    for( int i = 0 ; i < 16 ; i ++ )
    {
        pinMode( GPIO[i], OUTPUT ) ;
    }

    rightLed.begin();
     leftLed.begin();

    uint32 DEADBEEF ;
    EEPROM.get( DEADBEEF_EE_ADDRESS, DEADBEEF ) ;

    if( DEADBEEF != 0xDEADBEEF )
    {   DEADBEEF  = 0xDEADBEEF ;

        EEPROM.put( DEADBEEF_EE_ADDRESS, DEADBEEF ) ;

        settings.dccExt          = 0 ;  
        settings.locoFunctions   = LOCO_FUNCTIONS_OFF ;
        settings.uniqueAddresses = 0 ;
        settings.maxCduVoltage   = 0 ;
        settings.rcn213          = 1 ;                         // default use normal address
        EEPROM.put( EE_SETTINGS, settings ) ;
        
        for( int i = 0 ; i < nCoils ; i ++ )
        {
            coil[i].setType( DOUBLE_COIL_PULSED ) ; 
            coil[i].setPulseTime( DEF_DOUBLE_PULSE_TIME ) ;
            coil[i].setAddress( i+1 ) ;                        // initialize all addresses in sequential order starting from 1
            coil[i].setDutyCycle( DEF_DUTYCYCLE ) ;            // 10 => just ON
            coil[i].setStates( 0, 0 ) ;
        }
        saveCoils() ;
    }

    EEPROM.get( EE_SETTINGS, settings ) ;

    loadCoils() ;
    
    dcc.pin( 2, 0 ) ;
    dcc.init( MAN_ID_DIY, 11, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );
}


void loop()
{
    determineLedPattern(); 
    rightLed.update();
    leftLed.update();
    
    updatePwm() ;

    dcc.process() ;

    // testRoutine() ; // simulate events to test things like menu and outputs

    config() ;

    // REPEAT_MS( 1 ) NEED TESTING
    // {
    //     int sample = analogRead( currentSensePin ) ;
    //     if( sample >= ADC_max )                 // if shortcircuit...
    //     {
    //         for( int i = 0 ; i < nGpio ; i ++ )
    //         {
    //             digitalWrite( GPIO[i], LOW ) ;  // kill all outputs
    //         }

    //         rightLed.setEventBleeps(5) ;            // go flash LED's
    //         leftLed.setEventBleeps(5) ;

    //         runMode = 0 ;                       // set lockout time of atleast 5 seconds.
    //         lockoutTime = millis() ;
    //     }
    // }
    // END_REPEAT
    

    if( runMode == 0 && (millis() - lockoutTime) >= 5001 ) { runMode = 1 ; } // after 5 seconds reinstate outputs.

    if( runMode == 1 )
    {
        static uint8 index = 0 ;
        
        // int voltage = analogRead( coilVoltagePin ) ; // NEED TESTING
        //if( voltage >= (settings.maxCduVoltage * 9 / 10) ) // above 90% is good to switch
        {
            
            if( coil[index].update() )
            {   
                iterate( index, nCoils ) ; // iterate to next coil if conditions allow for it.
            }
        }
    }

    saveStates() ;
    updateCduVoltage() ;
}


/* Dynamically allocate addresses 
To be called whenever a type is changed. The first address of the first object is starting point
returns the amount of used addresses;
*/
uint8_t squashAddresses()
{
    if( settings.uniqueAddresses ) return nCoils ;  // VERIVY ME 

    uint8  addressCount = nCoils ;
    uint16 riseAddress = coil[0].getAddress() ;

    for( int i = 0 ; i < nCoils-1 ; i ++ ) 
    {
        uint8 prevType = coil[i].getType() ;
        if( prevType == SINGLE_COIL_CONTINUOUSLY
        ||  prevType == SINGLE_COIL_PULSED ){ riseAddress += 2 ; addressCount ++ ; }
        else                                { riseAddress += 1 ; }

        coil[i+1].setAddress( riseAddress ) ;
    }
    return addressCount ;
}

uint8 addressReceived()
{
    if( newAddressSet )
    {   newAddressSet = false ;

          rightLed.setEventBleeps( 3 ) ; // bleep to show that an address has been received.
        leftLed.setEventBleeps( 3 ) ;

        return 1 ;
    }

    return 0 ;
}

/***** CONFIGURATION MENU ********/
void config()
{
    static uint8  nextState = 0 ;
    static uint32 prevTime  = 0 ;
    static R_trigger menu1 ; // -> get address OR get index for address
    static R_trigger menu2 ; // -> get index for type
    static R_trigger menu3 ; // -> get index for pulse length
    static R_trigger menu4 ; // -> enable PWM dutycycle
    static R_trigger menu5 ; // -> option menu

    REPEAT_MS( 20 )
    {
        configButton.debounceInputs() ;
    }
    END_REPEAT

    uint8_t btnState = configButton.getState() ;

    if( btnState == FALLING && state != idle ) // quit menu
    {
        state = idle ;
        btnState = LOW ; // prevents tripping below menu when we want to quit.
    }


    int i ;
    switch( state )
    {
    case idle:
        if( btnState == FALLING ) // button is pressed
        {
            beginTime = millis() ;
            state     = checkButton ;
            nextState = setBaseAddress ; // if released real quick, we go this state anyway
            newAddressSet = 0 ;
        }
        break ;

    case checkButton:
        if( menu1.trigger( millis() - beginTime >     1 ) ) { nextState = setBaseAddress     ; rightLed.setEventBleeps(1) ; }
        if( menu2.trigger( millis() - beginTime >  2000 ) ) { nextState = setIndex4Type      ; rightLed.setEventBleeps(2) ; }
        if( menu3.trigger( millis() - beginTime >  4000 ) ) { nextState = setIndex4pulse     ; rightLed.setEventBleeps(3) ; }
        if( menu4.trigger( millis() - beginTime >  6000 ) ) { nextState = setIndex4dutyCycle ; rightLed.setEventBleeps(4) ; }
        if( menu5.trigger( millis() - beginTime >  8000 ) ) { nextState = configMode         ; rightLed.setEventBleeps(5) ; }

        if( btnState == RISING ) { state = nextState ; /*Serial.println(F("transitioning")); */}
        break ;



// MENU 1
    case setBaseAddress:
        if( settings.uniqueAddresses )
        {
            state = setIndex4Address ; // if this flag is set, we do not use sequential address so we go to this state instead.
        }

        if( addressReceived() )
        {
            coil[0].setAddress( receivedAddress ) ;
            squashAddresses() ;
            saveCoils() ;
            state = idle ;
        }
        break ; 
    
    case setIndex4Address:
        if( addressReceived() )
        {
            coilIndex = constrain( receivedAddress, 1, nCoils ) - 1 ;    
            state = setUniqueAddress ;
        }
        break ;

    case setUniqueAddress:
        if( addressReceived() )
        {
            coil[coilIndex].setAddress( receivedAddress ) ;
            saveCoils() ;
            state = setIndex4Address ;
        }
        break ;



// MENU 2
    case setIndex4Type:
        if( addressReceived() )
        {
            coilIndex = constrain( receivedAddress, 1, nCoils ) - 1 ;
            state = setCoilType ;
        }
        break ;

    case setCoilType:               // setting type, also loads defaults for dutycycle and pulse lengths
        if( addressReceived() )
        {
            uint8 type = constrain( receivedAddress, 1, nTypes ) - 1 ;
            coil[coilIndex].setType( type ) ;

            coil[coilIndex].setDutyCycle( DEF_DUTYCYCLE ) ;

            if( type == SINGLE_COIL_PULSED )  coil[coilIndex].setPulseTime( DEF_SINGLE_PULSE_TIME ) ;
            if( type == DOUBLE_COIL_PULSED 
             || type == DOUBLE_PULSE_W_FROG ) coil[coilIndex].setPulseTime( DEF_DOUBLE_PULSE_TIME ) ;

            if( coilIndex < (nCoils/2)
            &&  type == DOUBLE_PULSE_W_FROG )  // this type require the one on the other side to be disabled. 
            {
                coil[ nCoils - 1 - coilIndex ].setType( DORMENT ) ;
                coil[ nCoils - 1 - coilIndex ].reset() ;
            }
            coil[ coilIndex ].reset() ;
            

            squashAddresses() ;
            saveCoils() ;
            state = setIndex4Type ;
        }
        break ; 


// MENU 3
    case setIndex4pulse:
        if( addressReceived() )
        {
            coilIndex = constrain( receivedAddress, 1, nCoils + 1 ) - 1 ; // note with address 9 we can set all coils at same frequency at once.
            state = setPulseTime ;
        }
        break ;

    case setPulseTime:
        if( addressReceived() )
        {
            receivedAddress = constrain( receivedAddress, 1, nDccAddresses ) ;

            if( coilIndex < 8 )                      
            {
                if( coil[coilIndex].getType() == SINGLE_COIL_PULSED ) coil[coilIndex].setPulseTime( receivedAddress * 1000 ) ; // steps of 1000ms - DCC address 30 -> 30s
                else                                                  coil[coilIndex].setPulseTime( receivedAddress *   10 ) ; // double pulse, have shorter pulses than single
            }
            else for( int i = 0 ; i < nCoils ; i ++ ) // index 8 -> adjust all coils
            {
                if( coil[i].getType() == SINGLE_COIL_PULSED ) coil[i].setPulseTime( receivedAddress * 1000 ) ;
                else                                          coil[i].setPulseTime( receivedAddress *   10 ) ;
            }

            saveCoils() ;
            state = setIndex4pulse ;
        }
        break ;


// MENU 4
    case setIndex4dutyCycle:
        if( addressReceived() )
        {
            coilIndex = constrain( receivedAddress, 1, nCoils + 1 ) - 1 ; // note with address 9 we can set all coils at same dutycycle at once.
            state = setDutyCycle_ ;
        }
        break ;

    case setDutyCycle_:
        if( addressReceived() )
        {
            receivedAddress = constrain( receivedAddress, 1, DEF_DUTYCYCLE ) ;
            if( coilIndex < nCoils )                       
            {
                coil[coilIndex].setDutyCycle( receivedAddress ) ; // set dutycycle of only one output
            }
            else for( int i = 0 ; i < nCoils ; i ++ ) 
            {
                coil[i].setDutyCycle( receivedAddress ) ;       // set all dutycycles
            }

            state = setIndex4dutyCycle ;
        }  
        break ;


// MENU 5
    case configMode:
        if( addressReceived() )
        {
            if( receivedAddress == 1 ) for( i = 0 ; i < nCoils ; i ++ ) // preset 1, all double pulse (default time 50ms)
            {
                coil[i].setType( DOUBLE_COIL_PULSED ) ;
                coil[i].setPulseTime( DEF_DOUBLE_PULSE_TIME ) ; // default to 50ms
                coil[i].setDutyCycle( DEF_DUTYCYCLE ) ;
                coil[i].reset() ;
            }

            if( receivedAddress == 2 ) for( i = 0 ; i < nCoils ; i ++ ) // preset 2, all double continously ( PM1 style motors of green red signals)
            {
                coil[i].setType( DOUBLE_COIL_CONTINUOUSLY ) ;
                coil[i].setDutyCycle( DEF_DUTYCYCLE ) ;
                coil[i].reset() ;
            }

            if( receivedAddress == 3 ) for( i = 0 ; i < nCoils ; i ++ )  // preset 3, single coils pulsed (16x) (default time set at 5s)
            {
                coil[i].setType( SINGLE_COIL_PULSED ) ;
                coil[i].setPulseTime( DEF_SINGLE_PULSE_TIME ) ;
                coil[i].setDutyCycle( DEF_DUTYCYCLE ) ;
                coil[i].reset() ;
            }

            if( receivedAddress == 4 ) for( i = 0 ; i < nCoils ; i ++ ) // preset 4, single Coil continously (16x ON or OFF)
            {
                coil[i].setType( SINGLE_COIL_CONTINUOUSLY ) ;
                coil[i].setDutyCycle( DEF_DUTYCYCLE ) ;
                coil[i].reset() ;
            }

            if( receivedAddress ==  5 ) for( i = 0 ; i < (nCoils/2) ; i ++ ) // preset 5, 4x double coil (right side) and 4x double relay (left side) for dual relay frog juicing
            {
                coil[i].setType( DOUBLE_PULSE_W_FROG ) ;
                coil[i].setPulseTime( DEF_DOUBLE_PULSE_TIME ) ; // default to 50ms
                coil[i].setDutyCycle( DEF_DUTYCYCLE ) ; // ON
                coil[i].reset() ;
                coil[i+4].setType( DORMENT ) ; // disable coil objects on the opposite side
                coil[i+4].reset() ;
            }

            if( receivedAddress == 20 ) { settings.uniqueAddresses = 0 ; } // default TESTED SUCCES
            if( receivedAddress == 21 ) { settings.uniqueAddresses = 1 ;
                                          settings.locoFunctions   = LOCO_FUNCTIONS_OFF ; } // unique addresses and loco functions don't go along, I think (I dont know really)
             
            if( receivedAddress == 30 ) { settings.dccExt = 0 ; }          // default
            if( receivedAddress == 31 ) { settings.dccExt = 1 ; }  // allow DCC EXTENDED TO CONTROL PULSE DURATION. (Note, this can perhaps be set via normal or ext instruction.)
            
            if( receivedAddress == 40 ) { settings.locoFunctions = LOCO_FUNCTIONS_OFF ; }
            if( receivedAddress == 41 ) { settings.locoFunctions =         EIGHT_BALL ; } // 16 points per address (for single mode, use F1 - F16
            if( receivedAddress == 42 ) { settings.locoFunctions =     FANTASTIC_FOUR ; } //  4 points per address (2 address per decoder) (special support for locomaus)

            // need CDU as option? cdu does happen to work out of the box so, we kinda need no setting for it
            // if( receivedAddress == 50 ) { settings.hasCdu  = 0 ; } // unsure if needed anymore..
            // if( receivedAddress == 51 ) { settings.hasCdu  = 1 ; }
            
            if( receivedAddress == 1000 || receivedAddress == 996 ) { settings.rcn213 = 0 ;  leftLed.setEventBleeps(5); } // OFF =  with offset
            if( receivedAddress == 1001 || receivedAddress == 997 ) { settings.rcn213 = 1 ; rightLed.setEventBleeps(5); } //  ON = 'normal' addressing (default)

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
    if( settings.dccExt == 0 )  return ;
    for( int i = 0 ; i < nCoils ; i ++ )
    {
        if( coil[i].setCoilExt( address, aspect ) )
        {
            rightLed.setEventBleeps(i+1) ;
             leftLed.setEventBleeps(i+1) ;
        }
    }
}

void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output ) // incomming DCC commands
{
    if( settings.rcn213 == 0 )
    {
        address = constrain( address - 4, 1, 2048 ) ;
    }

    if( millis() - lastTime >= LOCKOUT_TIME ) // create lockout time to prevent processing package more than 1x (used for config menu only)
    {   lastTime = millis() ;

        newAddressSet = 1 ;
        receivedAddress = address ;
    }

    if( settings.locoFunctions != LOCO_FUNCTIONS_OFF ) return ; // loco functions are used, disable conventional addresses
    if(  state                 != idle )               return ; // config menu is run, dont set output
    if( output                 ==    0 )               return ; // ancient DCC flag is a dont care

    if( direction > 0 ) direction = 1 ; 

    for( int i = 0 ; i < nCoils ; i ++ )
    {
        if( coil[i].setCoil( address, direction, 0 ) ) // if true, DCC address is found and LEDs will bleep.
        {
            rightLed.setEventBleeps( 1 ) ;  // give one bleep to confirm an address is received
             leftLed.setEventBleeps( 1 ) ;
        }
    }
}


// Locomotive function support
static uint16_t oldState = 0 ;
static uint16_t newState = 0 ;

void notifyDccFunc( uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState )
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

    if( settings.locoFunctions == EIGHT_BALL ) // 1 loco address 16 functions max (if single outputs are used.
    {
        if (Addr != dccBaseAddress) return ;

        if(      FuncGrp ==   FN_0_4 ) { bitOffset =  0 ; }
        else if( FuncGrp ==   FN_5_8 ) { bitOffset =  4 ; }
        else if( FuncGrp ==  FN_9_12 ) { bitOffset =  8 ; }
        else if( FuncGrp == FN_13_20 ) { bitOffset = 12 ; }
        else return ;
    }

    newState &= ~(0x0F << bitOffset) ;              // first clear the old bits
    newState |= (FuncState & 0x0F) << bitOffset ;   // than reload the newly received bits

    uint16_t changedBits = oldState ^ newState;     // if atleast 1 bit has changed, 
    oldState = newState ;
    if (changedBits == 0) return ;

    for( int i = 0 ; i < maxFunctions+1 ; i++ ) // loop through all changedBits to set the coil.
    {
        uint16_t andMask = 1 << i ;
        if(( changedBits & andMask ) == 0 ) continue ;

        uint8_t state = (newState >> i) & 0x1 ;

        uint16_t dccAddress = dccBaseAddress + i ; // calculate the appropiatate DCC address to be used in setCoil. 

        for( int j = 0 ; j < nCoils ; j++ )
        {
            coil[j].setCoil( dccAddress, state, 1 ) ; // and feed the changed state of this address into all coils, override lockout time active
        }
    }
}

static uint32_t prevtime = 0 ;
static uint8_t  state1 = 0 ;
uint8_t timeout( uint32_t interval ) 
{
    if( millis() - prevtime >= interval ) 
    {       prevtime = millis() ;
        
        Serial.println( state1++ ) ;

        return 1 ;
    }
    
    return 0 ;
}

void testRoutine()  // this routine simulates button presses and incomming dcc addresses.
{
    switch( state1 )
    {
    case 0: if( timeout( 50100 ))
        {   
            Serial.println( "sending 3x command to test lockout" ) ;


        }
        break ;

    case 1: if( timeout( 1000 ))
        {
            Serial.println( "sending 3x command to test lockout 1S later" ) ;

        }
        break ;
         

    case 2: if( timeout( 50100 ))
        {
            Serial.println( "sending 3x command to test lockout 5S later" ) ;


        }
        break ;
        
    case 3: if( timeout( 50100 ))
        {
        }
        break ;
        

    case 4: if( timeout( 50100 ))
        {
        }
        break ;
        
    case 5: if( timeout( 50100 ))
        {
        }
        break ;

    case 6: if( timeout( 600000 ))
        {

        }
        break ;
        
    case 7: if( timeout( 6000 ))
        {

        }
        break ;

    }
}
