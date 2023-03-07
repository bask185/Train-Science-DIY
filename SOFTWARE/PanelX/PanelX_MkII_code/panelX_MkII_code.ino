#include "src/XpressNetMaster.h"
#include "src/macros.h"
#include "src/debounceClass.h"
#include <EEPROM.h>

const int rs485dir  =   2 ;
const int nPoints   =  20 ;
const uint8_t pins[] = 
{ 
     3,  4,  5,  6,  7,  8,  9, 10,
    11, 12, A0, A1, A2, A3, A4, A5
} ;
const int nInputs = sizeof(pins) / sizeof( pins[0] ) ;

struct
{
    uint8_t size ;
    uint16_t address[ nPoints ] ;
} Point ;


Debounce    input[ nInputs ] ;
Debounce    config ;
uint8_t     state[ nInputs ] ;
uint8_t     configMode ;
uint8_t     F1state = 1 ;
uint8_t     flash ;
uint8_t     streetIndex ;
uint8_t     pointIndex ;
uint16_t    prevAddress ;
bool        settingStreet ;
bool        firstStreet ;

XpressNetMasterClass Xnet ;

void debounceInputs()
{
    REPEAT_MS( 20 )
    {
        for( int i = 0 ; i < nInputs ; i ++ )
        {
            input[i].debounce() ;
        }

        config.debounce( F1state ) ;    
    }
    END_REPEAT
}

void storeStreet()
{
    int eeAddress = streetIndex * sizeof( Point ) ;
    EEPROM.put( eeAddress, Point ) ;
}

void processInputs()
{
    uint8_t state = config.getState() ;

    if( state == FALLING )                  // start teaching
    { 
        configMode =  true ;
        firstStreet = true ;
    }

    if( state ==  RISING )                  // stop teaching
    {
        configMode = false ;
        storeStreet() ;
    }

    
    for( int i = 0 ; i < nInputs ; i ++ )
    {
        state = input[i].getState() ;

        if( state == FALLING )  // if any of the buttons is pressed...
        {
            flash = 4 ;

            if( configMode ) // TEACHIN IN MODE
            {
                if( firstStreet == false ) // if this was not the first street, commit the previous one to EEPROM
                {
                    storeStreet() ;
                }

                streetIndex   =      i ;
                pointIndex    =     -1 ;
                prevAddress   = 0xFFFF ;

                firstStreet = false ;
            }

            else // NORMAL MODE, reset index and start laying the street
            {
                int eeAddress = i * sizeof( Point ) ; 

                EEPROM.get( eeAddress, Point ) ;
                pointIndex    =    0 ;
                settingStreet = true ;
            }
        }
    }
}

void setStreet()
{
    REPEAT_MS( 500 )
    {
        if( settingStreet )
        {
            uint16_t address = Point.address[ pointIndex ] & 0x03FF ;
            uint16_t   state = Point.address[ pointIndex ] >> 15 ;

            if( state ) flash = 4 ;
            else        flash = 8 ;

            Xnet.SetTrntPos( address, state, 1 ) ;
            delay( 20 ) ;
            Xnet.SetTrntPos( address, state, 0 ) ;

            if( pointIndex ++ == Point.size ) // finished?
            {
                settingStreet = false ;
            }
        }
    }
    END_REPEAT
}


#define TGL13 PORTB ^=(1<<5)

void updateLED()
{
    if( flash )
    {
        REPEAT_MS(50)
        {
            TGL13;
            flash -- ;
        }
        END_REPEAT
    }
    
    else if( configMode == true )
    {
        REPEAT_MS(500)
        {
            TGL13;
        }
        END_REPEAT
    }
    else digitalWrite(13, LOW ); 
}

void setup()
{
    Xnet.setup( Loco128, rs485dir ) ;

    for( int i = 0 ; i < nInputs ; i ++ )
    {
        uint8_t pin = pins[i] ;
        input[i].begin( pin ) ;
    }
    debounceInputs() ;
    delay(50);
    debounceInputs() ;

    pinMode(13,OUTPUT);
}

void loop()
{
    updateLED(); 
    Xnet.update() ;
    debounceInputs() ;
    processInputs() ;
    setStreet() ;
}

void notifyXNetTrnt( uint16_t Address, uint8_t data ) // put an address and state
{
    if( bitRead( data, 3 ) == false ) return ;
    if( configMode         == false ) return ;
    
    Address &= 0x3FFF ;

    data &= 0b11 ; // would it not suffice to just and only bit 0?
    if( data >= 2 ) data -= 2 ;

    if( Address != prevAddress )
    {   prevAddress = Address ;

        pointIndex ++ ;
        Point.size = pointIndex ;
    }

    if( data) flash = 10 ;
    else      flash = 20 ;

    Point.address[ pointIndex ] = Address | (data << 15) ; // commit both address and state to the array.
}
void notifyXNetLocoFunc1( uint16_t address, uint8_t func )
{
    if( address != 9999 ) return ;
    if( func & 1 ) { F1state = 0 ; }
    else           { F1state = 1 ; }
}
