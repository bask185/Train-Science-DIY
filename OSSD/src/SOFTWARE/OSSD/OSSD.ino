#include "src/macros.h"
#include "src/NmraDcc.h"
#include "config.h"
#include "Signal.h"
#include "src/debounceClass.h"
#include <EEPROM.h>



/******* SK NOTES **********
 * TODO
 V  The DCC bus must be verified
 v PROGMEM values must be verified
 V The actual signal control must be verified
 V The config menu 
 V EEPROM must be verified
 V real signal PCB must be made and ordered
*/

// bytes 0-15   hold types
// bytes 16-47  hold unique DCC addresses

const int   DEFAULT_ADDRESS = 1000 ;
const int   DEFAULT_VALUE   = 0xCC ;

const int   DCC_ADDRESS     = 1002 ;
const int   DEFAULT_DCC     = 0x01 ;

const int   CONFIG_ADDRESS  = 1004 ;
const int   DEFAULT_CONFIG  = 0x00 ;


// config bits
const int   DCC_EXTENDED        = 0 ;
const int   UNIQUE_ADDRESSES    = 1 ;
// const int   bit2         = 2 ;

uint8       configBits ;

const int   nCoils     = 8 ;    // always 8
const int   maxSignals = 16 ;   // 16 for single outputs

NmraDcc     dcc ;
Signal      signal[maxSignals] ;

Debounce    configButton ;

enum modeState
{
    idle,
    getBaseAddress,
    getIndex,
    getUniqueAddress,
    getSignalType,
    configMode,

    checkButton, // not a actual mode
} ;

uint8       state       = idle ;
uint32      beginTime ;
uint8       index ;
uint8       signalCount ;
volatile uint16      
            receivedAddress ;
volatile uint8       
            newAddressSet ;
uint16      myAddress ;
uint8       signalIndex ;
volatile static uint32 lastTime ;


void(* RESET) (void) = 0 ;

void statusLed()
{
    static uint16 blinkTime = 1000 ;

    REPEAT_MS(blinkTime )
    {
        switch( state )
        {
        default: 
            digitalWrite( ledPin, HIGH ) ;
            break ;

        case configMode:        blinkTime =  50 ; goto blink ;  //  125/2
        case getSignalType:     blinkTime = 125 ; goto blink ;  //  250/2
        case getUniqueAddress:  blinkTime = 190 ; goto blink ;  //  190/2
        case getIndex:          blinkTime = 250 ; goto blink ;  //  500/2
        case getBaseAddress:    blinkTime = 500 ; goto blink ;  // 1000/2
        
        blink:
            digitalWrite( ledPin, !digitalRead( ledPin ));
            break;
        }
    }
    END_REPEAT
}

void storeType( uint8 idx, uint8 val )  // type use 1 byte. (0-15)
{
    int eeAddress = idx ;
    EEPROM.write( eeAddress, val ) ;
}

uint8 loadType( uint8 idx )
{
    int eeAddress = idx ;
    return EEPROM.read( eeAddress ) ;
}

void storeAddress( uint8 idx, uint16 val )  // address use 2 byte. (16-47)
{
    int eeAddress = (idx * 2) + 16 ;
    EEPROM.put( eeAddress, val ) ;
}

uint16_t loadAddress( uint8 idx )
{
    int eeAddress = (idx * 2) + 16 ;
    uint16 val ;
    EEPROM.get( eeAddress, val ) ;
    return val ;
}


void setup()
{
    pinMode( 13, OUTPUT ) ;
    for( int i = 0 ; i < 16 ; i ++ )
    {
        pinMode( GPIO[i], OUTPUT ) ;
    }
    configButton.begin( configPin ) ;

    if( EEPROM.read(  DEFAULT_ADDRESS ) != DEFAULT_VALUE )   
    {   EEPROM.write( DEFAULT_ADDRESS,     DEFAULT_VALUE ) ;

        configBits = DEFAULT_CONFIG ;
        EEPROM.write( CONFIG_ADDRESS, configBits ) ;

        myAddress = DEFAULT_DCC ;
        EEPROM.put( DCC_ADDRESS, myAddress ) ; // initialize DCC address

        for( int i = 0 ; i < 16 ; i ++ )
        {
            storeType( i, 0 ) ;   // set all items to coil type. DEFAULT
            storeAddress(i, i+1 ) ; // set all unique addresses in incrementing order. Only used in unique address mode
        }
    }

    EEPROM.get( DCC_ADDRESS, myAddress ) ; // load dcc address from EEPROM
    for( int  i = 0 ; i < nGpio ; i ++ )
    {
        uint8_t type = loadType( i ) ;  // fetch from EEPROM
        signal[i].setType( type ) ;     // intialize the signals
    }

    dcc.pin(2, 0);
    dcc.init( MAN_ID_DIY, 11, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );

    initSignals() ;
    signalIndex = 0 ;
}

void loop()
{
    statusLed() ;

    dcc.process() ;

    config() ;

    if( signal[index].update() == 1 ) // if the signal is a coil instead, it will return 0 whilst being set. This prevents that more than 1 output can be activated
    {
        if( ++ index == signalCount ) index = 0 ;
    }
}

/* Dynamically allocate addresses and outputs to signal/point objects */
void initSignals()
{
    // Serial.println("\r\n\r\nSETTING SIGNALS");
    signalCount = 0 ; // reset this to 0

    // printNumberln(F("begin address: "), myAddress );

    uint8_t ledCount = 0 ;
    uint16_t addressCount = myAddress ;
    for( int i = 0 ; i < maxSignals ; i ++ )
    {
        uint8_t nLeds = signal[i].getLedCount() ;
        
        if( nLeds + ledCount > nGpio ) break ;  // IO is full, no more room for this signal, break
        else
        {
            if( bitRead( configBits, UNIQUE_ADDRESSES ))
            {
                addressCount = loadAddress( i ) ; // if unique addresses are used, fetch an address from EEPROM instead
            }

            signal[i].setFirstIO( ledCount ) ;
            signal[i].setAddress( addressCount ) ;

            // NOTE, if DCC extended is used, one address per item should be used
            if( bitRead( configBits, DCC_EXTENDED ) ) addressCount += 1 ;  // BUG, somehow dcc ext does not seem to work for index 2 or higher
            else                                      addressCount += signal[i].getAddressAmount() ;

            signalCount ++ ;

            ledCount += nLeds ;
        }
    }
    // printNumberln(F("amount ot devices: "), signalCount ) ;
}

/* TODO
need an option to enable extended commands
need an option to pick between solenoids, perhaps still implement as a signal
(it would solve soo many problems, and it is propably workable)
*/
void config()
{
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

    switch( state )
    {
    case idle:
        if( btnState == FALLING ) // button is pressed
        {
            beginTime = millis() ;
            state = checkButton ;
            newAddressSet = 0 ;
        }
        break ;

    case checkButton:
        if( millis() - beginTime >= 1000 ) state =   getIndex ; // long press
        if( btnState == RISING )                                // short press
        {
            if( bitRead( configBits, UNIQUE_ADDRESSES )) state = getIndex ;
            else                                         state = getBaseAddress ;
        }
        break ;

    case getBaseAddress:
        if( newAddressSet == 1 )
        {   newAddressSet = 0 ;

            myAddress = receivedAddress ;
            EEPROM.put( DCC_ADDRESS, myAddress ) ;
            initSignals() ;
            state = idle ;
            newAddressSet = 0 ;
        }
        break ; 

    case getIndex: // RESTRAIN VALUE TO ACCEPTABLE NUMBERS 
        if( btnState == LOW
        &&  (millis() - beginTime >= 4000) ) state = configMode ; // button held down for 4s

        if( newAddressSet == 1  )
        {   newAddressSet = 0 ;

            signalIndex = receivedAddress-1 ;
            if( bitRead( configBits, UNIQUE_ADDRESSES )) state = getUniqueAddress ;
            else                                         state = getSignalType ;
        }
        break ;

    case getUniqueAddress:
        if( newAddressSet == 1 ) // RESTRAIN VALUE TO ACCEPTABLE NUMBERS  
        {   newAddressSet = 0 ;

            storeAddress( signalIndex, receivedAddress ) ;
            initSignals() ;
            state = getIndex ;
        }
        break ;

    case getSignalType:
        if( newAddressSet == 1 ) // RESTRAIN VALUE TO ACCEPTABLE NUMBERS  
        {   newAddressSet = 0 ;

            signal[signalIndex].setType( receivedAddress-1 ) ; //(should go to EEPROM?)
            initSignals() ;
            state = getIndex ;
        }
        break ; 
    
    case configMode:
        if( newAddressSet == 1 )
        {   newAddressSet = 0 ;

            if( receivedAddress == 1 ) // use conventional DCC addresses
            {
                bitClear( configBits, DCC_EXTENDED ) ;
                EEPROM.write( CONFIG_ADDRESS, configBits ) ;
                initSignals() ;
                state = idle ;
            }
            if( receivedAddress == 2 ) // use DCC EXT addresses
            {
                bitSet( configBits, DCC_EXTENDED ) ;
                EEPROM.write( CONFIG_ADDRESS, configBits ) ;
                initSignals() ;
                state = idle ;
            }
            if( receivedAddress == 3 ) // use sequential addresses
            {
                bitClear( configBits, UNIQUE_ADDRESSES ) ;
                EEPROM.write( CONFIG_ADDRESS, configBits ) ;
                initSignals() ;
                state = idle ;
            }
            if( receivedAddress == 4 ) // use unique address per object
            {
                bitSet( configBits, UNIQUE_ADDRESSES ) ;
                EEPROM.write( CONFIG_ADDRESS, configBits ) ;
                initSignals() ;
                state = idle ;
            }
            if( receivedAddress == 10 ) // FACTORY RESET
            {
                EEPROM.write( DEFAULT_ADDRESS, ~DEFAULT_VALUE ) ;   // 'corrupt' default value to let decoder re-initialize the decoder's EEPROM
                Serial.println("FACTORY RESET in 2 seconds");
                delay(2000);
                RESET() ;                                           // reset the decoder
            }
        }
        break ;
    }
}


/* SK: NOTE
    The first signal type (= coils = 0) should always listen to conventional DCC addresses
    So even if DCC extended is enabled, coils will adhere to conventional addresses.

    DCC ext addresses are not yet handled well with nAddresses. 
    
    if DCC EXT is disabled it should never be used.
*/
void notifyDccSigOutputState( uint16_t address, uint8_t aspect ) // incomming DCC EXT commands
{
    if( !bitRead( configBits, DCC_EXTENDED ) ) return ;         // if DCC EXT is disabled, don't act

    for( int i = 0 ; i < signalCount ; i ++ )
    {
        signal[i].setAspectExt( address, aspect ) ; // only does something if address is correct. 
        return ;
    }
}

void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output ) // incomming DCC commands
{
    if( millis() - lastTime >= 500 ) // create lockout time of 100ms to prevent processing package
    {   lastTime = millis() ;

        newAddressSet = 1 ;
        receivedAddress = address ;
    }

    if(  state != idle ) return ;
    if( output ==    0 ) return ;
    if( direction > 0 ) direction = 1 ;

    for( int i = 0 ; i < signalCount ; i ++ )
    {
        uint8 type = signal[i].getType() ;
        if( bitRead( configBits, DCC_EXTENDED ) && type != 0 ) return ;

        signal[i].setAspect( address, direction ) ; // a signal objects checks this address and direction to see if he should do something with it
    }
}
