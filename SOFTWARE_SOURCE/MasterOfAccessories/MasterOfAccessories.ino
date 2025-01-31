#include "src/io.h"
#include "src/date.h"
#include "src/version.h"
#include "src/macros.h"
#include "src/DCC.h"
#include "src/DCCEX.h"
#include "routes.h"
#include <EEPROM.h>

/* TODOS
    // Delete the damage of that DCC EX missery and just implement bloody loconet. iTrain and DCC ex protocol was no succes
    
    process shift register IO                               CHECK should be OK, chatGPT checked it
    organize I2C eeprom , routes addresses etc.             PENDING,

    Serial configuration menu, must become some kind of mode. UNSURE IF NEEDED, going to use buttons for teach and config..
     - amount of IO module

    conventional configuration mode.
       config mode, cycle through button modes -> accessory or route button   TODO
       teach mode,  teach in routes soley using buttons                       TODO

       holding down da button enters condif mode. 
         1 bleep is configuration mode
         2 bleep is teachin mode, make routes n such
         3 bleep is adjusting amount of modules. The first 2 buttons are used for this. every first LED of every module must light up when module is active 

    In essence we can have like idk lets say 128 button/LEDs max. This may be more, big eeprom storage

    every button has a button ID which is essentially it's place in the list
    every button has a state, it is on or off           1 bit
    every button may be momentary or a toggle type      1 bit
    every button may need to have it's state inverted.  1 bit
    every button has default the same address as it's own ID. So button #7 controls DCC address 7 10 bits (1024) will suffice  UNSURE IF NEEDED
    Only the GUI is allowed to alter addresses of buttons (we simply have no other way to input data)
    
    A button can have a link to an entry in EEPROM for a route. So when a route button is pressed, a route in EEPROM must be set.
       I want to use a linked list, a table with 2 buttons { button 1, button 2 } of 256 entries (for now). The index dertermenes which 
       Eeprom address the route is going to be stored

       During teaching, when a route button or two route buttons are pressed follow by a point, I need to search in this table
       if this combination is already in use. If so, I can use that entry than otherwise I find me a new entry { 255, 255 }

       Than when eeprom section is located. It needs to be whiped and filled with addresses and states.

    States of 'memory allocater' can be:

       idle (wait 4 any route button action )

       get First button (likely be instant)

       during teach, there may be a 2nd button comming or a point

       during playing, either route must be laid or we wait for button 2. Lookup table desides who next, wait second button, set street

       set street

       add points to street


    EXTRA:  need potentiometer control with 1 or 2 push buttons (encoder)? for fine tuning servo decoders!
*/

DCCEX dccEx(Serial);

struct buttonStates 
{
    uint8   newState : 1 ;  
    uint8   oldState : 1 ;
    uint8   tglState : 1 ;
    uint8  momentary : 1 ;
    uint8    routBtn : 1 ;
} ;

buttonStates button[ maxRegister*8 ] ;

// DCCEXProtocol dccEx ; // Note. DUMBASS LIBRARY USES 986 bytes of goddamn RAM
// DCCEXProtocolDelegate delegate ; // unsure if the delegate is needed?

enum modes
{
    runMode,
    teachinMode,
    configurationMode,
    setNregisterMode,
} ;

DCC dcc ;

uint8   mode = runMode ;
uint8   shiftRegisterUpdated = 0 ;
uint8   incomingByte[ maxRegister ] ;
uint8   outgoingByte[ maxRegister ] ;

void updateShiftRegisters()
{
    toggleState ;

    if( shiftRegisterUpdated ) return ;

    REPEAT_MS(1) 
    {
        if( state )
        {
            digitalWrite( latch_pin, LOW ) ;
            for( int i = 0 ; i < nRegister ; i ++ )
            {
                shiftOut( data_out_pin, clock_pin, MSBFIRST, outgoingByte[i] ) ;
            }
        }

        else
        {
            digitalWrite( latch_pin, HIGH ) ;
            for( int i = 0 ; i < nRegister ; i ++ )
            {
                incomingByte[i] = shiftIn( data_in_pin, clock_pin, MSBFIRST ) ;
            }

            shiftRegisterUpdated = true ;
        }
    }
    END_REPEAT
}

void processShiftRegisterData()
{
    if( shiftRegisterUpdated == 0 ) return ;
    shiftRegisterUpdated = 0 ;

    for( int reg = 0 ; reg < nRegister ; reg ++ )
    for( int pin = 0 ; pin <         8 ; pin ++ )
    {
        uint8 index = (reg * 8) + pin ;
        button[index].newState    = bitRead( incomingByte[reg], pin ) ;  // read in 1 bit at the time for all shift registers

        if( button[index].newState != button[index].oldState )
        {   button[index].oldState  = button[index].newState ;

            runMode() ;
            /* TODO the button at this moment are debounced and their state is set
               now we need to do special things depending on the mode
               
               logic dictates that a function call per mode should be in order  */

            if( button[index].routBtn == 1 ) 
            {
                char buffer[100];
                sprintf(buffer, "route button %d pressed" );
                Serial.println(buffer);

                sendButton( index, button[index].newState ) ;             // handles first and second buttons. DONE, TEST ME
            }

            if( button[index].momentary && button[index].newState == 0 )  // I am a momentary switch and I react on pulse = low
            {   button[index].tglState ^= 1 ;                             // I toggle from state every flank

                char buffer[100];
                sprintf(buffer, "momentary pressed, toggling address: %d, new state = %d", index+1,  button[index].tglState ) ;
                Serial.println(buffer);

                dcc.setAccessory( index+1, button[index].tglState ) ;     // TODO? perhaps make address variable ''. As is now, address depends soleley on phyisical position 
                dccEx.setPoint(   index+1, button[index].tglState ) ;
            }

            if( button[index].momentary == 0 )                            // I am a toggle switch and my position is absolute
            {
                char buffer[100];
                sprintf(buffer, "toggle switch moved, setting address: %d to state %d", index+1,  button[index].newState ) ;
                Serial.println(buffer);

                dcc.setAccessory( index+1, button[index].newState ) ;
                dccEx.setPoint(   index+1, button[index].newState ) ;
            }
        }
    }
}

void getMode()
{
    REPEAT_MS( 333 )
    {
        mode = digitalRead( modePin2 ) << 1 | digitalRead( modePin1 ) ; 
    }
    END_REPEAT
}

static uint8_t printIt = 1 ;
void setup()
{
    initIO() ;

    dcc.begin() ;
    
    Serial.begin(115200);
    delay(1000);
    dccEx.begin();
    delay(1000);
    if(printIt)
    {
        for( int i = 0 ; i < 50 ; i ++ )
        {
            char b = EEPROM.read( i ) ;
            Serial.println(b);
        }
    }
}

void loop() 
{

    dcc.generatePackets() ;

    dccEx.process();

    updateShiftRegisters() ;
    processShiftRegisterData() ;
}

void dccISR() // DCC ISR callback, use this to set actual outputs
{
    // PORTB ^= 1 << x ;
    // PORTB ^= 1 << y ;
}
void notifyRoutePointSet( uint16 address, uint8 state ) // call back by route manager
{
    dcc.setAccessory( address, state ) ;
    dccEx.setPoint(   address, state ) ;
}

void NotifyDccExPointSet( uint16 address, uint8 state ) // call back by DCC EX parser
{
    dcc.setAccessory( address, state ) ;
    // TODO, perhaps add loco's in the future.
    pinMode(13,OUTPUT);
    PORTB^=1<<5;
}