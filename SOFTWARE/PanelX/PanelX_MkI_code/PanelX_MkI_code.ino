#include "src/date.h"
#include "src/version.h"
#include "src/macros.h"
#include "src/XpressNetMaster.h"
#include "src/debounceClass.h"
#include "src/eeprom.h"



/************ VARIABLES & CONSTANTS ***********/
const int       rs485dir        =  2 ;
const uint8_t   pins[]   = 
{ 
     3,  4,  5,  6,  7,  8,  9, 10,
    11, 12, A0, A1, A2, A3, A4, A5
} ;
uint8_t flash ;
const uint8_t nInputs = sizeof(pins) / sizeof(pins[0]) ;

Debounce    input[ nInputs ] ;

#ifndef DEBUG
XpressNetMasterClass Xnet ;
#endif

enum modes
{
    TEACHIN,
    NORMAL,
} mode = NORMAL ;

uint16_t lastAddress = 0xFFFF ; // dummy value for testing
uint8_t  lastPos  ;


void debounce()
{
    REPEAT_MS( 50 )
    {
        for (int i = 0; i < nInputs; i++)
        {
            input[i].debounce() ;
        }
    }
    END_REPEAT
}

void readSwitches()
{
    for (int pin = 0; pin < nInputs ; pin ++ )
    {
        uint8_t btnState = input[pin].getState() ;

        if( btnState == FALLING || btnState == RISING )
        {
            flash = 4 ;

            if( mode == NORMAL)                                                 // set point
            {
                uint16_t address = loadPoint( pin ) ;                           // fetch address & state from EEPROM
        
                uint8_t state = address >> 15 ;                                  // stuff MSB in state
                address &= 0x03FF ;                                              // remove MSB from address 
                //address -- ;
                
                if( btnState == FALLING ) state ^= 1 ;

                #ifndef DEBUG
                Xnet.SetTrntPos( address+1, state, 1) ;
                delay(20) ;
                Xnet.SetTrntPos( address+1, state, 0) ;                               // maybe not needed? should check for this
                #else
                Serial.print("pin = " ) ;               Serial.println( pin ) ;
                Serial.print("address in EEPROM = ");   Serial.println(address) ;
                Serial.print("state = ");               Serial.println(state) ;
                #endif
            }
            else                                                                    // store point
            {   
                uint16_t toStore ;
                if( btnState ==  RISING ) toStore = lastAddress | (0x1 << 15) ;     // set state according to button position
                else                      toStore = lastAddress | (0x0 << 15) ;

                storePoint( pin, toStore ) ;                                         // store in EEPROM


                #ifdef DEBUG
                Serial.print("\nconnecting pin "); Serial.print(pin) ;
                Serial.print(" to address ") ; Serial.println( lastAddress ) ;
                Serial.print("with state ") ; Serial.println( toStore ) ;
                #endif                
            }
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
    debounce() ;
    delay( 100 ) ;
    debounce() ;

    Xnet.setup( Loco128 , rs485dir ) ;
    pinMode(13,OUTPUT) ;
    for (int i = 0; i < nInputs; i++)
    {
        input[i].begin(pins[i]) ;
    }
}


void loop()
{
    updateLED() ;

    debounce() ;
    readSwitches() ;
   
    #ifndef DEBUG
    Xnet.update() ;
    #endif

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