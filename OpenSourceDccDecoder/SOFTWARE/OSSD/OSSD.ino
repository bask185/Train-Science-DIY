#include "src/macros.h"
#include "src/NmraDcc.h"
#include "config.h"
#include "src/CoilDrive.h"
#include "src/Signal.h"
#include <EEPROM.h>

NmraDcc     dcc ;

const int   nCoils   = 8 ;    // always 8
int         nSignals = 8 ;    // start with maximum amount, may be less depending on what mode is selected.
uint32_t    beginTime ;
uint8_t     mode ;

CoilDrive   coil[nCoils] ;
Signal      signals[nSignals] ;

enum modeState
{
    idle,
    getAddress,
    setMode1,
    setMode2,
    setMode3,
    setMode4,

    checkButton, // not a actual mode
}
enum modes
{
    mode1,
    mode2,
    mode3,
    mode4,
    mode5,
}

uint8_t mode = idle ;
uint8_t state = idle ;

void statusLed()
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
    for( int i = 0 ; i < nGPIO ; i ++ )
    {
        pinMode( GPIO[i], OUTPUT ) ;
    }

    loadEeprom() ;
    
    dcc.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );
}

void loop()
{
    static uint8_t index = 0 ;
    dcc.process() ;

    configure() ;
}

void storeMode( uint8_t _mode )
{
    mode = _mode ;
    EEPROM.write( MODE_ADDRESS, mode ) ; // may be enough debounce time...this lasts 3ms iirc?
    delay( 10 ) ;  // extra debounce time for switch
    state = idle ;
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
        if( millis() - beginTime >= 2000 )      state = setMode1 ;
        break ;

    case getAddress:
        if(1/* address received*/)
        {
            storeMode( mode1 ) ;
        }
        break ; 

    case setMode1:
        if( digitalRead( configButton ) == 1 )  
        {
            storeMode( mode2 ) ;

        }
        if( millis() - beginTime >= 4000 ) state = setMode2 ;

        break ; 

    case setMode2:
        if( digitalRead( configButton ) == 1 )  
        {
            storeMode( mode3 ) ;
        }
        if( millis() - beginTime >= 6000 ) state = setMode3 ;

        break ; 

    case setMode3:
        if( digitalRead( configButton ) == 1 )  
        {
            storeMode( mode4 ) ;
        }
        if( millis() - beginTime >= 8000 ) state = setMode4 ;

        break ; 

    case setMode4:
        if( digitalRead( configButton ) == 1 )
        {
            storeMode( mode5 ) ;
        }
        break ; 
    }

}


void switchOutput( uint16_t address, uint8_t direction )            // interface abstract method to set output
{
    send2config( address ) ;

    for( int i = 0 ; i < settings.nGpio ; i ++ )
    {
        if( address == settings.myAddress + i )
        {
            setIndex( i ) ;
            Serial.println(i) ;
            return ;
        }
    }
}

void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output ) // incomming DCC commands
{
    if( output   != 0 ) return ;
    if( direction > 0 ) direction = 1 ;

    switchOutput( address, direction ) ;
}