#include "src/macros.h"
#include "src/event.h"
#include "src/XpressNetMaster.h"
#include "src/debounceClass.h"
#include <LiquidCrystal.h>

/******* LCD *******/
const int backlight = 10 ; //(perhaps not use) due to voltage regulator temperature
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/* print some spaces to clear the remainder on the lcd */
void clearln()
{
    for (int i = 0; i < 16; i++)
    {
        lcd.write(' ') ;
    }
}


/******* Xnet *******/
const int _485dir ;
XpressNetMasterClass Xnet ;


/***** DEBOUNCE STUFF *****/

// lcd switches -> addresses 4-7
Debounce    switches( 255 ) ;
const int   nSwitches =  4 ;
const int   switchPin = A3 ;
uint8_t     lastPressedSwitch ;
uint16_t    reference[4] ; // to be filled in 

// LCD switches
Debounce    lcdKeys( 255 ) ;
uint8_t     lastPressedKey ;
const int   lcdKeyPin = A0 ;
const int   lcdKeyValues[] = { 0,       // right
                             144,       // up
                             329,       // down
                             504,       // left
                             741 } ;    // select
const int   nLcdKeys = sizeof( lcdKeyValues ) / sizeof( lcdKeyValues[0] ) ;
enum 
{
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

/**** END OF VARIABLES ****/

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

        lcd.setCursor( 0, 0 ) ;
        lcd.print(F("Program #")) ;
        lcd.print( currentProgram + 1 ) ;
        clearln() ;

    #define showState(x) case x: lcd.print(F(#x)); break ;
        lcd.setCursor( 0, 1 ) ;
        prgState = program[currentProgram].getState() ;
        switch( prgState )
        {
            showState( idle ) ;
            showState( playing ) ;
            showState( finishing ) ;
            showState( recording ) ;
            clearln() ;
            break ;
        }
    } // end if( state == falling )
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
void notifyXNetLocoDrive128(uint16_t Address, uint8_t Speed)
{

}
void notifyXNetLocoFunc1(uint16_t Address, uint8_t Func1) // //Gruppe1 0 0 0 F0 F4 F3 F2 F1
{
    
}
void notifyXNetLocoFunc2(uint16_t Address, uint8_t Func2) // //Gruppe2 0000 F8 F7 F6 F5
{

}
void notifyXNetLocoFunc3(uint16_t Address, uint8_t Func3) // //Gruppe3 0000 F12 F11 F10 F9
{

}
void notifyXNetLocoFunc4(uint16_t Address, uint8_t Func4) // //Gruppe4 F20-F13
{

}
void notifyXNetLocoFunc5(uint16_t Address, uint8_t Func5) // //Gruppe5 F28-F21
{

}
void notifyXNetTrnt(uint16_t Address, uint8_t data)
{

}

// event handler callback functions
void notifyEvent( uint8 type, uint16 address, uint8 arg )
{

}

void displayFreeMemory( uint16 space )
{

}

void displayStoreMemory( uint16 space )
{

}n

void displayGetMemory( uint16 space )
{

}

void memoryFull()
{

}

void notifyI2cErrror( uint8_t )
{

}
