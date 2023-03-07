#include "src/date.h"
#include "src/version.h"
#include "src/macros.h"
#include "src/debounceClass.h"
#include "src/eeprom.h"
#include "src/LocoNet.h"

#define LN_TX_PIN     2

const int CURVED   = 1 ;
const int STRAIGHT = 0 ;

lnMsg  *LnPacket;          // pointer to a received LNet packet

// Construct a Loconet packet that requests a turnout to set/change its state
void sendOPC_SW_REQ( int address, byte dir, byte on ) {
    lnMsg SendPacket ;
    
    int sw2 = 0x00;
    //if (dir == TURNOUT_NORMAL) { sw2 |= B00100000; }
    if (on) { sw2 |= B00010000; }
    sw2 |= (address >> 7) & 0x0F;
    
    SendPacket.data[ 0 ] = OPC_SW_REQ ;
    SendPacket.data[ 1 ] = address & 0x7F ;
    SendPacket.data[ 2 ] = sw2 ;
    
    LocoNet.send( &SendPacket );
}

void setLNTurnout(int address, byte dir)
{
    sendOPC_SW_REQ(address - 1, dir, 1);
    sendOPC_SW_REQ(address - 1, dir, 0);
}




/************ VARIABLES & CONSTANTS ***********/
const int       nInputs         = 16 ;
const int       configPin       = A5 ;
const uint8_t   pins[nInputs]   = 
{ 
     3,  4,  5,  6,  7,  8,  9, 10,
    11, 12, 13, A0, A1, A2, A3, A4
} ;

Debounce    input[ nInputs ] ;
Debounce    config ;

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
        config.debounce() ;

    } END_REPEAT
}

void readSwitches()
{
    for (int pin = 0; pin < nInputs-1 ; pin ++ )
    {
        uint8_t btnState = input[pin].getState() ;

        if( btnState == FALLING || btnState == RISING )
        {
            if( mode == NORMAL)                                                 // set point
            {
                uint16_t address = loadPoint( pin ) ;                           // fetch address & state from EEPROM
        
                uint8_t state = address >> 15 ;                                  // stuff MSB in state
                address &= 0x03FF ;                                              // remove MSB from address 
                //address -- ;
                
                if( btnState == FALLING ) state ^= 1 ;

                setLNTurnout( address, state ) ;

                Serial.print("pin = " ) ;               Serial.println( pin ) ;
                Serial.print("address in EEPROM = ");   Serial.println(address) ;
                Serial.print("state = ");               Serial.println(state) ;
            }
            else                                                                    // store point
            {   
                uint16_t toStore ;
                if( btnState ==  RISING ) toStore = lastAddress | (0x1 << 15) ;     // set state according to button position
                else                      toStore = lastAddress | (0x0 << 15) ;

                storePoint( pin, toStore ) ;                                         // store in EEPROM          
            }
        }
    }
}
void setup()
{
    debounce() ;
    delay( 100 ) ;
    debounce() ;

    LocoNet.init(LN_TX_PIN);
}


void loop()
{
    debounce() ;
    readSwitches() ;
   
    LnPacket = LocoNet.receive() ;
    if( LnPacket )
    {   
        LocoNet.processSwitchSensorMessage(LnPacket);
    }

}

void notifySensor( uint16_t address, uint8_t state )
{    
}

void notifySwitchRequest( uint16_t address, uint8_t output, uint8_t dir )
{
}

void notifySwitchReport( uint16_t address, uint8_t output, uint8_t dir )
{
}

void notifySwitchState( uint16_t address, uint8_t output, uint8_t dir )
{
    if( mode == NORMAL ) return ;

    lastAddress  = address ;// & 0x03FF ;
}
