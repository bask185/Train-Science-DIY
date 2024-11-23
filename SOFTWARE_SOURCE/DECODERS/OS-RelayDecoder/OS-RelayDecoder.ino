#include "src/macros.h"
#include "src/NmraDcc.h"
#include "src/debounceClass2.h"
#include <EEPROM.h>
#include "relay.h"
#include "type.h"

/* TODO 
regel IO, houdt rekening met 4x en 8x
Doe LED functie
Doe knopje fixen. Kort is adressen zetten, lang is stand inverteren

Fix EEPROM
 - onthoud welke pinnen inverted zijn
 - onthoud wat mijn adres is
 - onthoud wat de laatste stand was.


*/

#ifdef _8_FOLD
const int   nRelay = 8 ;
const int   relayPins[] = {A1, A0, 10 , 9, 4, 3, 0 , A5 } ;

#elif defined _4_FOLD
const int   nRelay = 4 ;
const int   relayPins[] = {3, 5 ,A0, 4 } ;

#else
    #error defines are no bueno
#endif

const int   configPin = 7 ;

const int EEPROM_START_ADDRESS =   0 ;
const int        STATE_ADDRESS = 100 ;

struct Config
{
    uint32_t deadbeef ;
    uint16_t myAddress ;
    uint8_t  inverted[8] ; // just use all 8 for both 4 or 8 fold decoders. 
    uint8_t  lastState[8] ; 
};

Config defaultConfig = { 0xDEADBEEF, 1, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} } ;
Config currentConfig ;


Relay       relay[nRelay] ;
NmraDcc     dcc ;
uint16      myAddress ;
uint8       getAddress ; 
uint8       dccIndex ; 


Debouncer   configButton ;

void statusLed()
{
    // if getAddress  blink normal (linked to short press)
    // event: valid address received
    // event: long press, relay is toggled
}



void setup()
{
    pinMode( 13, OUTPUT ) ;

    EEPROM.get( EEPROM_START_ADDRESS, currentConfig) ;

    if (currentConfig.deadbeef != defaultConfig.deadbeef)
    {
        currentConfig = defaultConfig;
        EEPROM.put(EEPROM_START_ADDRESS, currentConfig ) ;
    }

    for( int i = 0 ; i < nRelay ; i ++ )
    {
        relay[i].invertPin( currentConfig.inverted[i] ) ;
        relay[i].setPin(    currentConfig.lastState[i]) ;
    }
    
    dcc.pin( 2, 0 ) ;
    dcc.init( MAN_ID_DIY, 11, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0 );

    configButton.setPin( configPin ).setFlank( FALLING ).setDebounceTime( 50 ) ;
    configButton.onShortPress = shortPress ;
    configButton.onLongPress  =  longPress ;
}


void loop()
{
    configButton.pressTime( 2000, 1 ) ;

    statusLed() ;

    dcc.process() ;
}

void shortPress()
{
    getAddress ^= 1 ; // toggle the mode
}

void longPress() 
{
    if( dccIndex == 0xFF ) return ;

    currentConfig.inverted[dccIndex] ^= 1 ;
    EEPROM.put(EEPROM_START_ADDRESS, currentConfig ) ; //update eeprom

    relay[dccIndex].invertPin( currentConfig.inverted[dccIndex]  ) ;
    relay[dccIndex].setState(  currentConfig.lastState[dccIndex] ) ; // update relay
}

void notifyDccAccTurnoutOutput( uint16_t address, uint8_t direction, uint8_t output ) // incomming DCC commands
{
    if( getAddress )
    {   getAddress = 0 ;

        currentConfig.myAddress = address ;
        EEPROM.put(EEPROM_START_ADDRESS, currentConfig ) ; // update eeprom
    }
    else
    {
        if( address >= myAddress && address < ( getAddress + nRelay ) ) 
        {
            dccIndex = address - myAddress ;
            currentConfig.lastState[dccIndex] ^= 1 ;
            EEPROM.put(EEPROM_START_ADDRESS, currentConfig ) ; // update eeprom
            relay[dccIndex].setState( currentConfig.lastState[dccIndex] ) ;
        }
        else
        {
            dccIndex = 0xFF ;
        }
    }
}
