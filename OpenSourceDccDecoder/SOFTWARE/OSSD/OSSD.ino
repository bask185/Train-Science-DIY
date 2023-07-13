#include "src/macros.h"
#include "src/NmraDcc.h"
#include "config.h"
#include "src/Signal.h"
#include "src/CoilDrive.h"
#include <EEPROM.h>



const int   nCoils     = 8 ;    // always 8
const int   maxSignals = 8 ;    // start with maximum amount, may be less depending on what mode is selected.

NmraDcc     dcc ;
CoilDrive   coil[nCoils] ;
Signal      signal[maxSignals] ;

enum modeState
{
    idle,
    getAddress,
    getIndex,
    getType,

    checkButton, // not a actual mode
} ;

uint8_t     mode        = idle ;
uint8_t     state       = idle ;
uint32_t    beginTime ;
uint8_t     activeMode ;
uint8_t     signalCount ;
uint8_t     index ;
uint16_t    receivedAddress ;
uint16_t    myAddress ;

const int MODE_ADDRESS = 0 ;


// what are out variables?
// we are a type
// we have an address

void statusLed() // TESTME
{
    static uint8_t prevMode ;
    static uint8_t flashes ;

    if( mode != prevMode ) // if mode change
    {   prevMode = mode ;

        flashes = mode * 2 ;// set flashes
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



void setup()
{
    for( int i = 0 ; i < 16 ; i ++ )
    {
        //pinMode( GPIO[i], OUTPUT ) ;
    }

    //loadEeprom() ;
    
    //dcc.init( MAN_ID_DIY, 10, 0, 0 );

    Serial.begin(115200);
    printNumberln("mynumber = ", myAddress ) ;

    myAddress = 32 ;
    signal[0].setType(2) ; // 4 leds
    signal[1].setType(2) ; // 8 leds
    signal[2].setType(1) ; // 11 leds
    signal[3].setType(1) ; // 14 leds
    signal[4].setType(0) ; // 16 leds
    signal[5].setType(0) ; // 16 leds
    signal[6].setType(0) ; // 16 leds
    signal[7].setType(0) ; // 16 leds
    resetSignals() ;

    signal[1].setAspect( 36, 0) ; // NOTE: CONVENTIONAL ADDRESSING WORKS! VERIFIED!
    signal[1].setAspect( 36, 1) ; //
    signal[1].setAspect( 37, 0) ;
    signal[1].setAspect( 37, 1) ;
    signal[1].setAspect( 38, 0) ;
    signal[1].setAspect( 38, 1) ;
    signal[1].setAspect( 39, 0) ;
    signal[1].setAspect( 39, 1) ;
    signal[1].setAspect( 40, 0) ;
    signal[1].setAspect( 40, 1) ;
    signal[1].setAspect( 41, 0) ;
    signal[1].setAspect( 41, 1) ;
}

void loop()
{
    static uint8_t index = 0 ;
    dcc.process() ;

    config() ;

    if( activeMode == 1 ) for( int i = 0 ; i < nCoils ; i ++ )
    {
        coil[i].update() ;
    }
    
    if( activeMode == 3 ) for( int i = 0 ; i < signalCount ; i ++ )
    {
        signal[i].update() ;
    }
}


void resetSignals()
{
    Serial.println("\r\n\r\nSETTING SIGNALS");
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
            addressCount += signal[i].getAddressAmount() ;

            printNumber_(F("\r\nsignal #"),      i ) ;
            printNumberln(F("led amount: "),     ledCount);
            printNumberln(F("first IO pin: "),   signal[i].get1stPin());
            printNumberln(F("aspect amount: "),  signal[i].getAspectAmount());
            printNumberln(F("begin address: "),  signal[i].getAddress()) ;
            printNumberln(F("end address: "),    signal[i].getAddress() + signal[i].getAddressAmount() - 1 ) ;
            signalCount ++ ;
        }
    }
    printNumberln(F("amount ot signals: "), signalCount ) ;
}

/* TODO
need an option to enable extended commands
*/
void config()
{
    switch( state )
    {
    case idle:
        if( digitalRead( configButton ) == 0 ) 
        {
            beginTime = millis() ;
            mode = checkButton ;
            delay(10) ; // deounce time for button 
        }
        break ;

    case checkButton:
        if( digitalRead( configButton ) == 1 )  state = getAddress ;
        if( millis() - beginTime >= 2000 )      state = getIndex ;
        break ;

    case getAddress:
        if(1/* address received*/)
        {
            state = idle ;
        }
        break ; 

    case getIndex: // RESTRAIN VALUE TO ACCEPTABLE NUMBERS 
        if( receivedAddress )  
        {   receivedAddress = 0 ;
            state = getType ;
            index = receivedAddress ;
        }
        if( digitalRead( configButton ) == 0 ) 
        {
            delay(5) ; // debounce and wait until button is released again
            while(digitalRead( configButton ) == 0 ) {;}
            delay(5) ;
            state = idle ;
        }
        break ; 

    case getType:
        if( receivedAddress ) // RESTRAIN VALUE TO ACCEPTABLE NUMBERS  
        {   
            signal[index-1].setType( receivedAddress ) ;
            resetSignals() ;
            receivedAddress = 0 ;
            state = getIndex ;
        }
        break ; 
    }
}


// void switchOutput( uint16_t address, uint8_t direction )            // interface abstract method to set output
// {
//     for( int i = 0 ; i < settings.nGpio ; i ++ )
//     {
//         if( address == settings.myAddress + i ) // obsolete?
//         {
//             //setIndex( i ) ;
//             Serial.println(i) ;
//             return ;
//         }
//     }
// }

void notifyDccSigOutputState( uint16_t address, uint8_t aspect ) 
{
    //for( int i = 0 ; i < nGpio ; i ++ )
    {
        if( address == myAddress )
        {
            //setIndex( i ) ;
            //Serial.println(i) ;
            return ;
        }
    }
}

void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output ) // incomming DCC commands
{
    receivedAddress = address ;

    if( output   != 0 ) return ;
    if( direction > 0 ) direction = 1 ;

    if( activeMode == 1 )
    {
        if( address >= myAddress && address < (myAddress+16))
        {
            uint8_t index = address - myAddress ;
            coil[index].setState( direction ) ;
        }
    }
    else if( activeMode == 2 )
    {
        if( address >= myAddress && address < (myAddress+16))
        {
            uint8_t index = address - myAddress ;
            digitalWrite( GPIO[index], direction ) ;
        }
    }
    else // for signals only 
    {
        for( int i = 0 ; i < signalCount ; i ++ )
        {
            signal[i].setAspect( address, direction ) ; // a signal objects checks his own address (should add this to coil object?) nah...
        }
    }
}