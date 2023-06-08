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
Debounce        switches( 255 ) ;
const int       switchPin = A3 ;
const int       nSwitches = 4 ;
uint8_t         lastPressedSwitch ;
const uint32_t  R1 = 1000 ;
const uint32_t  R2 = 4700 ;
// const uint16_t reference[] =
// {   
//     ( 0 * 1024 * R1 / ( R2 + ( 0 * R1))) , // CHECK IF I AM ACCORDING TO ACTUAL MEASUREMENTS
//     ( 1 * 1024 * R1 / ( R2 + ( 1 * R1))) ,
//     ( 2 * 1024 * R1 / ( R2 + ( 2 * R1))) ,
//     ( 3 * 1024 * R1 / ( R2 + ( 3 * R1))) ,
// } ;

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

/****** LOCO SLOTS *********/

// NOTE: slots are not really needed, I use the mainy for better displaying function commands.
// as XpressNet sends 4-8 functions per message I can use these slots to see which bit changes
typedef struct Locos
{
    uint16_t address ;
    uint8_t  speed : 7 ;
    uint8_t  dir : 1 ;
    // uint8_t F0 : 1 ; uint8_t  F9 : 1 ; uint8_t F18 : 1 ; uint8_t F27 : 1 ;
    // uint8_t F1 : 1 ; uint8_t F10 : 1 ; uint8_t F19 : 1 ; uint8_t F28 : 1 ;
    // uint8_t F2 : 1 ; uint8_t F11 : 1 ; uint8_t F20 : 1 ;
    // uint8_t F3 : 1 ; uint8_t F12 : 1 ; uint8_t F21 : 1 ;
    // uint8_t F4 : 1 ; uint8_t F13 : 1 ; uint8_t F22 : 1 ;
    // uint8_t F5 : 1 ; uint8_t F14 : 1 ; uint8_t F23 : 1 ;
    // uint8_t F6 : 1 ; uint8_t F15 : 1 ; uint8_t F24 : 1 ;
    // uint8_t F7 : 1 ; uint8_t F16 : 1 ; uint8_t F25 : 1 ;
    // uint8_t F8 : 1 ; uint8_t F17 : 1 ; uint8_t F26 : 1 ;

    uint32_t function ;

} Loco ;

const int nLocos = 25 ;
Loco loco[nLocos] ;
/**** END OF VARIABLES ****/


void showFunction( uint16_t address, uint8_t group, uint8_t incFunctions )
{
    for( int  i = 0 ; i < nLocos ; i ++ )
    {
        if( loco[i].address == 0x0000 ) // if address 0 is found in the list before a match is found
        {                               // than we know the address was not used before and we may commission a slot by inserting an address
            loco[i].address = address ;
        }

        if( address == loco[i].address ) // if true -> slot found
        {
            uint8_t startFunc ;
            uint8_t nFunc ;

            switch( group )
            {
            // case F0_F4Event:
            //     if( (functions&0b10000) != loco[i].F0 ) {loco[i].F0 = (functions >> 4) ;} // bit cumbersome do it like this, but it should work
            //     if( (functions&0b00001) != loco[i].F1 ) {loco[i].F1 = (functions     ) ;}
            //     if( (functions&0b00010) != loco[i].F2 ) {loco[i].F2 = (functions >> 1) ;}
            //     if( (functions&0b00100) != loco[i].F3 ) {loco[i].F3 = (functions >> 2) ;}
            //     if( (functions&0b01000) != loco[i].F4 ) {loco[i].F4 = (functions >> 3) ;}
            //     break ;

            // case F5_F8Event:
            //     if( (functions&0b0001) != loco[i].F5 ) {loco[i].F5 = (functions     ) ;}
            //     if( (functions&0b0010) != loco[i].F6 ) {loco[i].F6 = (functions >> 1) ;}
            //     if( (functions&0b0100) != loco[i].F7 ) {loco[i].F7 = (functions >> 2) ;}
            //     if( (functions&0b1000) != loco[i].F8 ) {loco[i].F8 = (functions >> 3) ;}
            //     break ;

            // case F9_F12Event:
            //     if( (functions&0b0001) != loco[i].F9  ) {loco[i].F9  = (functions     ) ;}
            //     if( (functions&0b0010) != loco[i].F10 ) {loco[i].F10 = (functions >> 1) ;}
            //     if( (functions&0b0100) != loco[i].F11 ) {loco[i].F11 = (functions >> 2) ;}
            //     if( (functions&0b1000) != loco[i].F12 ) {loco[i].F12 = (functions >> 3) ;}
            //     break ;

            // case F13_F20Event:
            //     if( (functions&0b00000001) != loco[i].F13 ) {loco[i].F13 = (functions     ) ;}
            //     if( (functions&0b00000010) != loco[i].F14 ) {loco[i].F14 = (functions >> 1) ;}
            //     if( (functions&0b00000100) != loco[i].F15 ) {loco[i].F15 = (functions >> 2) ;}
            //     if( (functions&0b00001000) != loco[i].F16 ) {loco[i].F16 = (functions >> 3) ;}
            //     if( (functions&0b00010000) != loco[i].F17 ) {loco[i].F17 = (functions >> 4) ;}
            //     if( (functions&0b00100000) != loco[i].F18 ) {loco[i].F18 = (functions >> 5) ;}
            //     if( (functions&0b01000000) != loco[i].F19 ) {loco[i].F19 = (functions >> 6) ;}
            //     if( (functions&0b10000000) != loco[i].F20 ) {loco[i].F20 = (functions >> 7) ;}
            //     break ;

                case F0_F4Event:   startFunc =  0 ; nFunc = 5 ; break ; // used to calculate correct function number below
                case F5_F8Event:   startFunc =  5 ; nFunc = 4 ; break ;
                case F9_F12Event:  startFunc =  9 ; nFunc = 4 ; break ;
                case F13_F20Event: startFunc = 13 ; nFunc = 8 ; break ; 
            }

            for( int j = 0 ; j < nFunc ; j ++ )
            {
                uint8_t funcState = incFunctions >> j ;      // the incomming function state 
                uint8_t        Fx = startFunc +  j ;         // locomotive function number
                uint8_t locoState = loco[i].function >> Fx ; // current state of loco function
                
                if( funcState != locoState )                 // compare incomming state with loco state
                {
                    bitWrite( loco[i].function, Fx, locoState ) ; // update function number in locoslot and display
                    lcd.setCursor(0,1);
                    lcd.print("F");
                    lcd.print(Fx);
                    lcd.print(' ');
                    bitRead(loco[i].function, Fx) ? lcd.print("ON") : lcd.print("OFF") ;
                    clearln();
                    return ;
                }
            }
        }
    }
}

/****** helper functions ******/
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


/***** CORE FUNCTIONS ******
 
 * debounceSwitches
 * debounceLcdKeys
 * debounceSensors
*/

void debounceSwitches()
{
    REPEAT_MS( 20 )
    {
        uint16_t sample = analogRead( switchPin ) ;

        for( int i = 0 ; i < nSwitches ; i ++ )
        {
            uint16_t reference = i * 1024 * R1 / ( R2 + ( i * R1)) ; // NOTE. may consume too much process effor, not that much tho

            uint16_t lowerBound ;
            if( reference < 20 ) lowerBound = 0 ; 
            else                    lowerBound = reference - 20 ;
            uint16_t                upperBound = reference + 20 ;

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
    showFunction( Address, F0_F4Event, Func1 ) ;
}
void notifyXNetLocoFunc2( uint16_t Address, uint8_t Func2 )  //                  F8  F7  F6  F5
{ 
    program[currentProgram].storeEvent( F5_F8Event, Address, Func2 ) ; 
    showFunction( Address, F5_F8Event, Func2 ) ;
}
void notifyXNetLocoFunc3( uint16_t Address, uint8_t Func3 )  //                 F12 F11 F10  F9
{ 
    program[currentProgram].storeEvent( F9_F12Event, Address, Func3 ) ;
    showFunction( Address, F9_F12Event, Func3 ) ;
}
void notifyXNetLocoFunc4( uint16_t Address, uint8_t Func4 )  // F20 F19 F18 F17 F16 F15 F14 F13
{ 
    program[currentProgram].storeEvent( F13_F20Event, Address, Func4 ) ; 
    showFunction( Address, F13_F20Event, Func4 ) ;
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
    case F0_F4Event:     Xnet.setFunc0to4(   address, data ) ; showFunction( address, F0_F4Event,  data ) ;  break ;
    case F5_F8Event:     Xnet.setFunc5to8(   address, data ) ; showFunction( address, F5_F8Event,  data ) ;  break ;
    case F9_F12Event:    Xnet.setFunc9to12(  address, data ) ; showFunction( address, F9_F12Event, data ) ;  break ;
    case F13_F20Event:   Xnet.setFunc13to20( address, data ) ; showFunction( address, F13_F20Event,data ) ;  break ;

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