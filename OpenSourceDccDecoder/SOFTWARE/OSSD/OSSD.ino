#include "src/macros.h"
#include "src/NmraDcc.h"
#include "config.h"
#include "src/Signal.h"
#include "src/CoilDrive.h"
#include <EEPROM.h>



const int   nCoils   = 8 ;    // always 8
const int   nSignals = 8 ;    // start with maximum amount, may be less depending on what mode is selected.

NmraDcc     dcc ;
CoilDrive   coil[nCoils] ;
Signal      signal[nSignals] ;

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
uint8_t     index ;
uint16_t    receivedAddress ;
uint16_t    myAddress ;


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
        pinMode( GPIO[i], OUTPUT ) ;
    }

    //loadEeprom() ;
    
    dcc.init( MAN_ID_DIY, 10, 0, 0 );
}

void loop()
{
    static uint8_t index = 0 ;
    dcc.process() ;

    config() ;
}
const int MODE_ADDRESS = 0 ;
void resetSignals()
{
    signalCount = 0 ; // reset this to 0
    uint8_t ledCount ;
    for( int i = 0 ; i < nSignals ; i ++ )
    {
        uint8_t nLeds = signal[i].getLedCount() ;
        if( nLeds + ledCount >= nGpio ) break ;  // IO is full, no more room for this signal, break
        else
        {
            ledCount += nLeds ;
            signal[i].setFirstIO( ledCount ) ;
        }
    }
}


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

    if( activeMode == 1 || activeMode == 2 )
    {
        //switchOutput( address, direction ) ;
    }
    else // for signals only 
    {
        uint8_t     nAddresses = getAspectAmount() / 2 + 1 ; // gets the amount of dcc addresses per signal
        uint8_t     nSignals   = 16 / getOutputAmount() ;
        uint16_t   endAddress  = myAddress + (nAddresses * nSignals) ;

        for (int i = 0; i < nSignals; i++ ) // NOTE: Despite this works, I must redo this part to take in acount different signal types.
        {
            if( address >= myAddress && address < endAddress )
            {
                uint8_t index  = (address - myAddress) / nAddresses ; // this should point to the correct signal.
                uint8_t aspect = ((address - myAddress) % nAddresses) * 2 + direction ; // TEST ME

                printNumberln("\r\nrecevied address: ", address) ;
                printNumberln("my address: ", myAddress) ;
                printNumberln("nAddresses: ", nAddresses) ;
                printNumberln("nSignals: ", nSignals) ;
                printNumberln("index: ", aspect) ;
                printNumberln("aspect: ", aspect) ;

                //signal[index].setState( aspect ) ;
            }
        }
        
        /* TODO /*
        first we need to now how many DCC addresses per signal are needed.
        This helps us to determen the index of the signal object.

        if we have the DCC addresses amount we can also determen which aspect 
        is to be set.

        notifyDccSigOutputState
        */
    }
}