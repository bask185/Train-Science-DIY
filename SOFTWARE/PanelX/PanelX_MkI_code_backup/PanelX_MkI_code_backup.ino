#include "config.h"
#include "src/macros.h"
#include "src/debounceClass.h"
#include "src/eeprom.h"


#include "src/XpressNetMaster.h"

XpressNetMasterClass Xnet ;


/************ VARIABLES & CONSTANTS ***********/
#ifdef XPRESSNET
const uint8_t pins[]   = 
{ 
     3,  4,  5,  6,  7,  8,  9, 10,
    11, 12, A0, A1, A2, A3, A4, A5
} ;
#else
const uint8_t pins[]   = 
{ 
     0,  1,  2,  3,  4,  5,  6,  7,  8, 
     9, 10, 11, 12, A0, A1, A2, A3, A4, A5
} ;
#endif

uint8_t flash, booted ;
const uint8_t nInputs = sizeof(pins) / sizeof(pins[0]) ;

Debounce    input[ nInputs ] ;

enum modes
{
    TEACHIN,
    NORMAL,
} mode = NORMAL ;

uint16_t lastAddress = 0xFFFF ; // dummy value for testing
uint8_t  lastPos  ;


void debounce()
{
    REPEAT_MS( 3 )
    {
        static byte index = 0 ; 
        if( ++ index == nInputs ) index = 0 ;
        
        input[index].debounce() ;
    }
    END_REPEAT
}

void readSwitches()
{
    static byte index = 0 ; 
    if( ++ index == nInputs ) index = 0 ;

    uint8_t btnState = input[index].getState() ;

    if( btnState == FALLING || btnState == RISING )
    {
        flash = 4 ;

        if( mode == NORMAL)                                                 // set point
        {
            uint16_t address = loadPoint( index ) ;                           // fetch address & state from EEPROM
    
            uint8_t state = address >> 15 ;                                  // stuff MSB in state
            address &= 0x07FF ;                                              // remove MSB from address 
            
            if( btnState == FALLING ) state ^= 1 ;


            Xnet.SetTrntPos( address, state, 1) ;
            delay(20) ;
            Xnet.SetTrntPos( address, state, 0) ;                               // maybe not needed? should check for this
        }
        else                                                                    // store point
        {   
            uint16_t toStore ;
            if( btnState ==  RISING ) toStore = lastAddress | (0x0 << 15) ;     // set state according to button position
            else                      toStore = lastAddress | (0x1 << 15) ;

            storePoint( index, toStore ) ;                                         // store in EEPROM

            // Serial.print("\nconnecting pin "); Serial.print(pin) ;
            // Serial.print(" to address ") ; Serial.println( lastAddress ) ;
            // Serial.print("with state ") ; Serial.println( toStore ) ;

            #ifdef DEBUG
            
            #endif                
        }
    }
    
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
    
    else if( mode == TEACHIN )
    {
        REPEAT_MS(500)
        {
            TGL13;
        }
        END_REPEAT
    }
    else digitalWrite(13, LOW ); 
}

/************* SETUP *************/
void setup()
{     
    pinMode(13,OUTPUT) ;

    for (int i = 0; i < nInputs; i++)
    {
        input[i].begin(pins[i]) ;
    }

    Xnet.setup( Loco128 , 2 ) ;
}


void loop()
{ 
    updateLED() ;

    debounce() ;
    readSwitches() ;
   
    Xnet.update() ;


}


void notifyXNetLocoFunc1( uint16_t address, uint8_t func )
{
    if( address != 9999 ) return ;
    if( func & 1 ) { mode = TEACHIN ; digitalWrite(13, HIGH ); }
    else           { mode = NORMAL  ; digitalWrite(13,  LOW ); }
}

void notifyXNetTrnt( uint16_t Address,  uint8_t data )
{
    if( bitRead( data, 3 ) == 1 )
    {
        flash = 10 ;
        if( mode == NORMAL ) return ;

        lastAddress  = Address;// & 0x03FF ;
    }
}
