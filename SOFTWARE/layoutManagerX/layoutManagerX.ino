#include "src/macros.h"
#include "src/event.h"
#include "src/XpressNetMaster.h"
#include "src/debounceClass.h"
#include <LiquidCrystal.h>

/* NOTES
most work is done, I only need to add analog reference values for my own buttons. 

I might wanna relocate LCD functions to separate file, but so far I am good.
The screen is not big enough to show everything. 
During recording I want to show free memory and the current mode that we ca see that we are still recording.

On the other hand during recording it may also be fruitfull what we are recording. It may help spotting errors with feedback n such.

During playing, I want to show issued loco and accessory commands. 
I also think it is fruitfull to indicate that we are waiting on a feedback sensor/switch.

This should also be shown when we are navigating through the programs.
So if I scroll thru the programs I also want to see if I am waiting for a feedback sensor or not

XXXXXXXXXXXXXXX <-- LINE 1 
XXXXXXXXXXXXXXX <-- LINE 2 
P8: RECORDING   // or playing, or finishing or idle
STAT: WAIT S1   // S1-8 are sensors/switches 
OR

NEXT COM: 1234  // next command and time  kinda useless? You can only see a timer run

OR

POINT #         // 4 digits for an address followed by 2 custom chars for straight - or  curved -/
SPEED 1234, 123 
F0-F4     xxxxx // no room 4 address, so be it
F5-F8      xxxx
F9-F12     xxxx
F13-20 xxxxxxxx
*/

/******* LCD *******/
const int backlight = 10 ; //(perhaps not use) due to voltage regulator temperature
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/* print some spaces to clear the remainder on the lcd */


/******* Xnet *******/
const int _485dir ;
XpressNetMasterClass Xnet ;


/***** DEBOUNCE STUFF *****/

// lcd switches -> addresses 4-7
Debounce    switches( 255 ) ;
const int   nSwitches =  4 ;
const int   switchPin = A3 ;
uint8_t     lastPressedSwitch ;
uint16_t    reference[4] ; /* = NOTE to be filled in 
{
    0,
    val,
    val,
    val,
} ; */

// LCD switches
Debounce    lcdKeys( 255 ) ;
uint8_t     lastPressedKey ;
const int   lcdKeyPin = A0 ;
const int   lcdKeyValues[] = { 0,       // right TESTME!
                             144,       // up
                             329,       // down
                             504,       // left
                             741 } ;    // select
const int nLcdKeys = sizeof( lcdKeyValues ) / sizeof( lcdKeyValues[0] ) ;
enum {
    RIGHT,
    UP,
    DOWN,
    LEFT,
    SELECT,
} ;

// sensors -> addresses 0-3
Debounce sensor[] =
{
    Debounce( 10 ) ,
    Debounce( 11 ) ,
    Debounce( 12 ) ,
    Debounce( A3 ) ,
} ;
const int nSensors = sizeof( sensor ) / sizeof( sensor[0] ) ;


/***** EVENT HANDLER *****/
const int programSize = 0x2000;
EventHandler program[] =
{
    EventHandler( 0*programSize, programSize, 0x28) , // check I2C addressed and EEPROM address, i2c was a guess
    EventHandler( 1*programSize, programSize, 0x28) ,
    EventHandler( 2*programSize, programSize, 0x28) ,
    EventHandler( 3*programSize, programSize, 0x28) ,

    EventHandler( 0*programSize, programSize, 0x29) ,
    EventHandler( 1*programSize, programSize, 0x29) ,
    EventHandler( 2*programSize, programSize, 0x29) ,
    EventHandler( 3*programSize, programSize, 0x29) ,
} ;
const int   nPrograms = sizeof( program ) / sizeof( program[0] ) ;
int8_t      currentProgram ;
uint8_t     prgState ;

enum customEvents
{
    accessoryEvent,
    speedEvent,
    F0_F4Event,
    F5_F8Event,
    F9_F12Event,
    F13_F20Event,
} ;

/**** END OF VARIABLES ****/

void clearln()
{
    for( int i = 0 ; i < 16 ; i++ )
    {
        lcd.write(' ') ;
    }
}

void displayStatus()
{ // UPPER LINE
    lcd.setCursor( 0, 0 ) ;
    lcd.print('P') ;
    lcd.print( currentProgram + 1 ) ;
    lcd.print(F(": ")) ;

    #define showState(x) case x: lcd.print(F(#x)); break ;
    prgState = program[currentProgram].getState() ;
    switch( prgState )
    {
        showState( idle ) ;
        showState( playing ) ;
        showState( finishing ) ;
        showState( recording ) ;
    }
    clearln() ;

  // LOWER LINE
    lcd.setCursor( 0, 1 ) ;
    lcd.print(F("STAT: ")) ;
}

void debounceSwitches()
{
    REPEAT_MS( 20 )
    {
        uint16_t sample = analogRead( switchPin ) ;

        for( int i = 0 ; i < nSwitches ; i ++ )
        {
            uint16_t lowerBound ;
            if( reference[i] < 20 ) lowerBound = 0 ; 
            else                    lowerBound = reference[i] - 20 ;
            uint16_t                upperBound = reference[i] + 20 ;

            if( sample > lowerBound 
            &&  sample < upperBound )
            {
                lastPressedSwitch = i + 4 ; // 4-7
                switches.debounce( 0 ) ;
                break ;
            }
            else
            {
                lastPressedSwitch = 0 ;
                switches.debounce( 1 ) ;
            }
        }
    }
    END_REPEAT

    uint8_t state = switches.getState() ;
    if( state == FALLING )
    {
        program[currentProgram].storeEvent( FEEDBACK, lastPressedSwitch, 0 ) ; // 4-7
        for( int i = 0 ; i < nPrograms ; i ++ )
        {
            program[i].sendFeedbackEvent( lastPressedSwitch ) ;
        }
    }
}

void debounceLcdKeys()
{
    REPEAT_MS( 20 )
    {
        uint16_t sample = analogRead( switchPin ) ;

        for( int i = 0 ; i < nSwitches ; i ++ )
        {
            uint16_t lowerBound ;
            if( lcdKeyValues[i] < 20 ) lowerBound = 0 ; 
            else                       lowerBound = lcdKeyValues[i] - 20 ;
            uint16_t                   upperBound = lcdKeyValues[i] + 20 ;

            if( sample > lowerBound 
            &&  sample < upperBound )
            {
                lastPressedKey = i + 3 ;
                switches.debounce( 0 ) ;
                break ;
            }
            else
            {
                switches.debounce( 1 ) ;
            }
        }
    }
    END_REPEAT

    uint8_t state = switches.getState() ;
    
    if( state == FALLING )
    {
        switch( lastPressedKey )
        {
        case SELECT: // PLAY
            program[currentProgram].startPlaying() ;
            break ;

        case LEFT: // RECORD
            program[currentProgram].startRecording() ;
            break ;

        case RIGHT:  // STOP
            program[currentProgram].stop() ;
            break ;

        case UP:
            if( prgState == recording ) break ; // during recording, current program must not be changed
            if( ++ currentProgram == nPrograms ) currentProgram = 0 ;
            break ;

        case DOWN:
            if( prgState == recording ) break ;
            if( -- currentProgram == -1 ) currentProgram = nPrograms-1 ;
            break ;
        }

        displayStatus() ;
    }
}


void debounceSensors()
{
    REPEAT_MS( 20 )
    {
        for( int i = 0 ; i < nSensors ; i ++ )
        {
            sensor[i].debounce() ;
        }
    }
    END_REPEAT

    for( int i = 0 ; i < nSensors ; i ++ )
    {
        uint8_t state = sensor[i].getState() ;

        if( state == FALLING || state == RISING )
        {
            for( int i = 0 ; i < nPrograms ; i ++ )
            {
                program[i].sendFeedbackEvent( lastPressedSwitch ) ; // send both rising and falling flanks
            }
            if( state == FALLING )
            {
                program[currentProgram].storeEvent( FEEDBACK, i, 0 ) ; // 0-3 // NOTE: is the 0 or 1 needed as final argument? I think not
            }
            else
            {
                program[currentProgram].storeEvent( FEEDBACK, i, 1 ) ;
            }
        }
    }
}

void setup()
{
    Xnet.setup( Loco128, _485dir ) ;

    for( int i = 0 ; i < nPrograms ; i ++ ) 
    {
        program[i].begin() ; // initializes EEPROM
    }

    lcd.begin( 16, 2 ) ; 
}

void loop()
{
    Xnet.update() ;

    for( int i = 0 ; i < nPrograms ; i ++ ) 
    {
        program[i].update() ;
    }

    debounceSwitches() ;
    debounceLcdKeys() ;
    debounceSensors() ;
}


// XpressNet Callback functions, called when you do something with your multimaus
void notifyXNetLocoDrive128( uint16_t Address, uint8_t Speed )                   
{
    program[currentProgram].storeEvent( speedEvent, Address, Speed ) ;

    // int8_t speed ;
    // speed = Speed & 0x7F ;

    // if( speed > 0 ) speed -- ;
    // if( Speed & 0x80 ) speed = -speed ;
   
    // if(         speed <  -100                ) knob = 4 ; // for future use to to repurpose loco function to set DCC accessories.
    // else if(    speed >= -100 && speed < -20 ) knob = 3 ;
    // else if(    speed >   -20 && speed <  20 ) knob = 2 ;
    // else if(    speed <=  100 && speed >  20 ) knob = 1 ;
    // else if(    speed >   100                ) knob = 0 ;
}

void notifyXNetLocoFunc1( uint16_t Address, uint8_t Func1 )  //              F0  F4  F3  F2  F1
{ 
    program[currentProgram].storeEvent( F0_F4Event, Address, Func1 ) ; 
}
void notifyXNetLocoFunc2( uint16_t Address, uint8_t Func2 )  //                  F8  F7  F6  F5
{ 
    program[currentProgram].storeEvent( F5_F8Event, Address, Func2 ) ; 
}
void notifyXNetLocoFunc3( uint16_t Address, uint8_t Func3 )  //                 F12 F11 F10  F9
{ 
    program[currentProgram].storeEvent( F9_F12Event, Address, Func3 ) ; 
}
void notifyXNetLocoFunc4( uint16_t Address, uint8_t Func4 )  // F20 F19 F18 F17 F16 F15 F14 F13
{ 
    program[currentProgram].storeEvent( F13_F20Event, Address, Func4 ) ; 
}

void notifyXNetTrnt(uint16_t Address, uint8_t data ) // call back from Xnet lib to indicate a point is set. 
{
    if( bitRead(data,3) == 1 )                      // ignore the active bit.
    { 
        data &= 0x1 ;
        program[currentProgram].storeEvent( accessoryEvent, Address, data ) ;
    }   
}

#define POINT_DELAY( interval ) uint32_t prevTime = millis() ; \
                                while( millis() - prevTime <= interval ) { Xnet.update(); }

void notifyEvent( uint8 type, uint16 address, uint8 data )                            // CALL BACK FUNCTION FROM EVENT.CPP
{
    switch( type )
    {
    case speedEvent:     Xnet.setSpeed(      address, Loco128, data ) ;  break ; // TODO add lcd messages to indicate what is happening?
    case F0_F4Event:     Xnet.setFunc0to4(   address, data ) ;           break ;
    case F5_F8Event:     Xnet.setFunc5to8(   address, data ) ;           break ;
    case F9_F12Event:    Xnet.setFunc9to12(  address, data ) ;           break ;
    case F13_F20Event:   Xnet.setFunc13to20( address, data ) ;           break ;

    case accessoryEvent: 
            Xnet.SetTrntPos( address, data, 1 ) ; // check if dis works properly
            POINT_DELAY( 20 ) ;
            Xnet.SetTrntPos( address, data, 0 ) ; // TODO: show message
            break ;

    case START: displayStatus() ;
    case STOP:  displayStatus() ; 
        break ;
     }
}

void displayFreeMemory( uint16 space ) // NOTE may be smart to use so one can see how much space is left.
{
    lcd.setCursor( 0, 0 ) ;
    lcd.print(F("FREE MEM: ")) ;
    lcd.print(space) ;
    clearln() ;
}

void memoryFull() // <-- NOTE may be good idea to use this one.
{
    lcd.clear() ;
    lcd.print(F("PROGRAM MEMORY FULL"));
    lcd.setCursor(0,1);
    lcd.print(F("reset arduino"));
    while(1) ;
}

/*

void displayStoreMemory( uint16 space )
{

}

void displayGetMemory( uint16 space )
{

}
void notifyI2cErrror( uint8_t )
{

}
*/