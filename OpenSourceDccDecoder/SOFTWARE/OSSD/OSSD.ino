#include "src/macros.h"
#include "src/NmraDcc.h"
#include "config.h"
#include "Signal.h"
#include "src/debounceClass.h"
#include <EEPROM.h>



/******* SK NOTES **********
 * PROJECT IS BUILT TO BE USED WITH PROGMEM, IT DOES NOT YET COMPILE
 * The config menu should be equiped with serial texts
 * Serial input should be used to insert DCC addresses. This allows me to debug the config menu
 *  

*/

const int   DEFAULT_ADDRESS = 1000 ;
const int   DEFAULT_VALUE   = 0xCC ;

const int   DCC_ADDRESS     = 1002 ;
const int   DEFAULT_DCC     = 0x01 ;

const int   CONFIG_ADDRESS  = 1004 ;
const int   DEFAULT_CONFIG  = 0x00 ;


// config bits
const int   DCC_EXTENDED    = 0 ;
// const int   bit1    = 1 ;
// const int   bit2    = 2 ;

uint8       configBits ;

const int   nCoils     = 8 ;    // always 8
const int   maxSignals = 16 ;   // 16 for single outputs

NmraDcc     dcc ;
Signal      signal[maxSignals] ;

Debounce    configButton ;

enum modeState
{
    idle,
    getAddress,
    getIndex,
    getSignalType,
    getMode,

    checkButton, // not a actual mode
} ;

uint8       mode        = idle ;
uint8       state       = idle ;
uint32      beginTime ;
uint8       index ;
uint8       signalCount ;
uint16      receivedAddress ;
uint16      myAddress ;


void(* RESET) (void) = 0 ;

void statusLed() //
{
    static uint8_t prevMode ;
    static uint8_t flashes ;

    if( state != prevMode ) // if mode change
    {   prevMode = state ;

        flashes = state * 2 ;// set flashes NEEDED?
    }

    if( flashes )
    {   flashes -- ;

        REPEAT_MS( 200 )
        {
            digitalWrite( ledPin, !digitalRead( ledPin )) ;
        } END_REPEAT
        
        if( flashes == 0 ) delay(1000) ;
    }  

    else if( state == idle )  digitalWrite( ledPin, HIGH ) ;
    else if( state == getAddress )
    {
        REPEAT_MS( 1000 )
        {
            digitalWrite( ledPin, !digitalRead( ledPin )) ;
        } END_REPEAT
    }
}

void storeType( uint8 idx, uint8 val )
{
    int eeAddress = idx ;
    EEPROM.write( eeAddress, val ) ;
}


uint8 loadType( uint8 idx )
{
    int eeAddress = idx ;
    return EEPROM.read( eeAddress ) ;
}


void setup()
{
    for( int i = 0 ; i < 16 ; i ++ )
    {
        //pinMode( GPIO[i], OUTPUT ) ; // ENABLE FOR FINAL PRODUCT
    }
    configButton.begin( configPin ) ;

    if( EEPROM.read(  DEFAULT_ADDRESS ) != DEFAULT_VALUE )   
    {   EEPROM.write( DEFAULT_ADDRESS,     DEFAULT_VALUE ) ;

        configBits = DEFAULT_CONFIG ;
        EEPROM.write( CONFIG_ADDRESS, config ) ;

        myAddress = DEFAULT_DCC ;
        EEPROM.put( DCC_ADDRESS, myAddress ) ; // initialize DCC address

        for( int i = 0 ; i < 16 ; i ++ )  storeType( i, 0 ) ; // set all items to coil type.
    }

    EEPROM.get( DCC_ADDRESS, myAddress ) ; // load dcc address from EEPROM
    for( int  i = 0 ; i < nGpio ; i ++ )
    {
        uint8_t type = loadType( i ) ; // fetch from EEPROM
        signal[i].setType( type ) ;
    }
    
    //dcc.init( MAN_ID_DIY, 10, 0, 0 );

    Serial.begin(115200);
    printNumberln("mynumber = ", myAddress ) ;

    signal[0].setType(0) ; // 4 leds
    signal[1].setType(0) ; // 8 leds
    signal[2].setType(0) ; // 11 leds
    signal[3].setType(0) ; // 14 leds
    signal[4].setType(0) ; // 16 leds
    signal[5].setType(0) ; // 16 leds
    signal[6].setType(0) ; // 16 leds
    signal[7].setType(0) ; // 16 leds
    signal[8].setType(0) ; // 16 leds
    signal[9].setType(0) ; // 16 leds
    signal[10].setType(0) ; // 16 leds
    signal[11].setType(0) ; // 16 leds
    signal[12].setType(0) ; // 16 leds
    signal[13].setType(0) ; // 16 leds
    signal[14].setType(0) ; // 16 leds
    signal[15].setType(0) ; // 16 leds

    initSignals() ;
}

void loop()
{
    dcc.process() ;

    config() ;

    if( signal[index].update() == 1 ) // if the signal is a coil instead, it will return 0 whilst being set. This prevents that more than 1 output can be activated
    {
        if( ++ index == signalCount ) index = 0 ;
    }
}


void initSignals()
{
    // Serial.println("\r\n\r\nSETTING SIGNALS");
    signalCount = 0 ; // reset this to 0

    printNumberln(F("begin address: "), myAddress );

    uint8_t ledCount ;
    uint16_t addressCount = myAddress ;
    for( int i = 0 ; i < maxSignals ; i ++ )
    {
        uint8_t nLeds = signal[i].getLedCount() ;
        
        if( nLeds + ledCount > nGpio ) break ;  // IO is full, no more room for this signal, break
        else
        {
            signal[i].setFirstIO( ledCount ) ;
            signal[i].setAddress( addressCount ) ;

            ledCount += nLeds ;

            // NOTE, if DCC extended is used, one address per item should be used
            if( bitRead( configBits, DCC_EXTENDED ) ) addressCount += 1 ;
            else                                      addressCount += signal[i].getAddressAmount() ;

            printNumber_(F("\r\nsignal #"),      i ) ;
            printNumberln(F("led amount: "),     ledCount);
            printNumberln(F("first IO pin: "),   signal[i].get1stPin());
            printNumberln(F("aspect amount: "),  signal[i].getAspectAmount());
            printNumberln(F("begin address: "),  signal[i].getAddress()) ;
            printNumberln(F("end address: "),    signal[i].getAddress() + signal[i].getAddressAmount() - 1 ) ;
            signalCount ++ ;
        }
    }
    printNumberln(F("amount ot devices: "), signalCount ) ;
}

/* TODO
need an option to enable extended commands
need an option to pick between solenoids, perhaps still implement as a signal
(it would solve soo many problems, and it is propably workable)
*/
void config()
{
    static uint8 signalIndex ;

    REPEAT_MS( 20 )
    {
        configButton.debounceInputs() ;
    }
    END_REPEAT

    uint8_t btnState = configButton.getState() ;

    switch( state )
    {
    case idle:
        if( btnState == FALLING ) // button is pressed
        {
            beginTime = millis() ;
            state = checkButton ;
        }
        break ;

    case checkButton:
        if( btnState == LOW
        &&  millis() - beginTime >= 2000 ) state =   getIndex ; // long press, configure output type
        if( btnState == RISING )           state = getAddress ; // btn released before 2 seconds
        break ;

    case getAddress:
        if( receivedAddress )
        {
            myAddress = receivedAddress ;
            EEPROM.put( DCC_ADDRESS, myAddress ) ;
            state = idle ;
        }
        if( btnState == FALLING )  state = idle ; // if button is pressed before address is received, action is aborted.
        break ; 

    case getIndex: // RESTRAIN VALUE TO ACCEPTABLE NUMBERS 
        if( btnState == FALLING )          state = idle ;    // if button is pressed before address is received, action is aborted.
        if( btnState == LOW
        &&  millis() - beginTime >= 4000 ) state = getMode ; // button held down for 4s

        if( receivedAddress <= nSignalTypes ) // if valid address is received... set the index.
        {
            signalIndex = receivedAddress-1 ;
            state = getSignalType ;
        }
        
        break ; 

    case getSignalType:
        if( receivedAddress ) // RESTRAIN VALUE TO ACCEPTABLE NUMBERS  
        {   
            signal[signalIndex].setType( receivedAddress ) ;
            initSignals() ;
            state = getIndex ;
        }
        break ; 
    
    case getMode:
        if( btnState  == FALLING ) state = idle ; // if button is pressed before address is received, action is aborted.
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
        if( receivedAddress == 10 )
        {
            EEPROM.write( DEFAULT_ADDRESS, ~DEFAULT_VALUE ) ;   // 'corrupt' default value to let decoder re-initialize the decoder's EEPROM
            RESET() ;                                           // reset the decoder
        }
        break ;
    }
    receivedAddress = 0 ;
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
    receivedAddress = address ;

    if( output   == 0 ) return ;
    if( direction > 0 ) direction = 1 ;

    for( int i = 0 ; i < signalCount ; i ++ )
    {
        uint8 type = signal[i].getType() ;
        if( bitRead( configBits, DCC_EXTENDED ) && type != 0 ) return ;

        signal[i].setAspect( address, direction ) ; // a signal objects checks this address and direction to see if he should do something with it
    }
}