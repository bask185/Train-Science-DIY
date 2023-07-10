#include "config.h"
#include "type.h"
#include "src/debounceClass.h"
#include "src/macros.h"
#include <EEPROM.h>

extern CONSTRUCT gpio[] ;

static uint8_t  servoPos = 90 ;
static uint16_t lastPointAddress ;
static uint8_t  currentIndex ;      // index of which of my IO is being edited  
static uint8_t  F0 ;
static uint8_t  F1 ;
static uint8_t  F2 ;
static uint8_t  longPress ;
static uint8_t  shortPress ;
static uint8_t  configMode ;

Debounce configBtn( 0xFF ) ;

Settings settings ;

enum states
{
    idle,
    getAddress,
    getServoMin,
    getServoMax,
    getCoilTime,
    getAspectType,
    getDCCext,
    getSwitchType,

} state = idle ;

enum aspectTypes
{
    DUTCH,          // normal green, yellow & red
    GERMAN,         // separate main and distant, in combination with diode matrices
    GERMAN_COMBO    // combines pre and distant on one pole
} ;

const int nAspectTypes ;

void send2config( uint16_t _address )
{
    lastPointAddress = _address ;
}

void setF1(uint8_t state ) { F1 = state ; }
void setF2(uint8_t state ) { F2 = state ; }
void setIndex( uint8_t idx ) 
{
    currentIndex = idx ;
}
void setServoPos( uint8_t newPos )
{
    servoPos = newPos ;
}


void saveEeprom() 
{
    EEPROM.put( 0, settings ) ;
    Serial.println(F("EEPROM saved"));
}

void loadEeprom()
{
    EEPROM.get( 0, settings ) ;
    Serial.println(F("EEPROM LOADED"));

    if( settings.defaults != 0xCC )     
    {   
        Serial.println(F("DEFAULTS ARE LOADED"));
        settings.defaults       =  0xCC ;       // DEFAULT VALUES
        settings.myAddress      =     1 ;
        settings.nGpio          = NGPIO ;
        settings.coilSwitchTime =   100 ;
        settings.DCC_type       =     0 ;

        for( int i = 0 ; i < NGPIO ; i ++ )
        {
            settings.servoMin[i]    =    82 ;
            settings.servoMax[i]    =    98 ;
            settings.aspectType[i]  = DUTCH ;

        }
        saveEeprom() ;
    }

    Serial.print(F("settings.myAddress ")) ;   Serial.println(settings.myAddress) ;
    Serial.print(F("settings.nGpio ")) ;   Serial.println(settings.nGpio) ;
    Serial.print(F("settings.coilSwitchTime ")) ;   Serial.println(settings.coilSwitchTime) ;
    Serial.print(F("settings.DCC_type ")) ;   Serial.println(settings.DCC_type) ;

    for( int i = 0 ; i < NGPIO ; i ++ )
    {

        Serial.print(F("servoMin: ")) ;   Serial.print(settings.servoMin[i]) ;
        Serial.print(F(", servoMax: ")) ;   Serial.print(settings.servoMax[i]) ;
        Serial.print(F(", aspectType: ")) ;   Serial.println(settings.aspectType[i]) ;

    }
}



void gotoState( uint8_t _newState )
{
    state               = _newState ;
    F0                  = 0 ; 
    F1                  = 0 ;
    longPress           = 0 ;
    shortPress          = 0 ;
    lastPointAddress    = 0 ;

    saveEeprom() ;
}


/*
Handles the press time of the configure button, 
short press, long press and factory reset are handled here
*/
void configButton()
{
    REPEAT_MS( 20 )
    {
        int sample = analogRead( A7 ) ;
        if( sample < 1  ) configBtn.debounce( 0 ) ;
        else              configBtn.debounce( 1 ) ;
    }
    END_REPEAT

    uint8_t         btnState = configBtn.getState() ;
    static uint8_t  discardRising ;
    static uint32_t lastTime ; 

    if( btnState == FALLING )
    {
        Serial.println(F("FALLING")) ;
        discardRising = false ;
        lastTime      = millis() ;
        longPress     = false ;
        shortPress    = false ;
    }
    if( btnState == LOW )
    {
        if( millis() - lastTime >= 2000 && discardRising == false )
        {
            discardRising = true ;
            longPress = true ;
            Serial.println(F("long pressed")) ;
        }
        if( millis() - lastTime >= 15000 )
        {
            // factory resets
            Serial.println(F(" factory reset!")) ;
            settings.defaults = 0x33 ;
            saveEeprom() ;
            loadEeprom() ;
            lastTime = millis() + 100000 ; // make sures this is only done once so EEPROM does not get fucked with
        }
    }
    if( btnState == RISING )
    {
        if( discardRising == true ) return ;

        shortPress = true ;
        Serial.println(F("RISING")) ;
        Serial.println(F("short pressed")) ;
    }
}

void configure()
{
    configButton() ;

    switch( state )
    {
    case idle:
        if( longPress == true 
        && (( servoPos == 90 && F1 == 0 && F2 == 0 ) // servo decoders need this to be true  // NEED TESTING
        || type != SERVO_DECODER )
        )
        {
            if( type == SERVO_DECODER ) { gotoState( getServoMin ) ; Serial.println(F("get servo min")) ; }
            if( type ==  COIL_DECODER ) { gotoState( getCoilTime ) ; Serial.println(F("get coil time")) ; }            
        }

        if( shortPress == true )
        {  
            Serial.println(F("getting address")) ;
            gotoState( getAddress ) ;
        }
        break ;

    // MY ADDRESS
    case getAddress:
        if( lastPointAddress > 0 && lastPointAddress < 2048 )
        {
            Serial.print(F("myAddress = ")) ;
            Serial.println(lastPointAddress) ;

            settings.myAddress = lastPointAddress ;
            gotoState( idle ) ;
        }
        if( shortPress ) gotoState( idle ) ;
        break ;

    // SERVO CODE
    case getServoMin: 
        gpio[currentIndex].setMin( servoPos ) ;                // straight servo position

        if( F1 == true )
        {
            Serial.println(F("min servo stored")) ;
            settings.servoMin[currentIndex] = servoPos ;
            gotoState( getServoMax ) ;
        }
        break ;

    case getServoMax:
        gpio[currentIndex].setMax( servoPos ) ;                // curved servo position

        if( F2 == true )
        {
            Serial.println(F("max servo stored")) ;
            settings.servoMax[currentIndex] = servoPos ;
            gotoState( idle ) ;
        }
        break ;


    // GET COIL SWITCH TIME
    case getCoilTime:
        if( lastPointAddress >= 50 && lastPointAddress <= 250 ) // N.B. perhaps use 1 to let unit switch permanently? This would also need to increase nGpio? make variable?
        {
            Serial.print(F("coilSwitchTime = "));
            Serial.println(lastPointAddress) ;
            settings.coilSwitchTime = lastPointAddress ;
            gotoState( idle ) ;
        }
        break ;

    // GET ASPECT TIME 
    case getAspectType:
        if( lastPointAddress >= 0 && lastPointAddress <= nAspectTypes )
        {
            Serial.print(F("aspectType = "));
            Serial.println(lastPointAddress) ;
            settings.aspectType[currentIndex] = lastPointAddress ;
            gotoState( idle ) ;
        }
        break ;

    // get DCC type, normal or extended (not yet sure how to enter this?
    case getDCCext:
        if( lastPointAddress > 0 )
        {
            Serial.print(F("DCC_type = ")) ;
            Serial.println(lastPointAddress-1) ;

            if( lastPointAddress == 1 ) settings.DCC_type = 0 ;  // normal
            if( lastPointAddress == 2 ) settings.DCC_type = 1 ;  // use dcc extended

            gotoState( idle ) ;
        }
        break ;

    // GET SWITCH MODE   control accessories or send occupancy over loconet
    case getSwitchType:
        if( lastPointAddress > 0 )
        {
            Serial.print(F("inputType = ")) ;
            Serial.println(lastPointAddress-1) ;

            if( lastPointAddress == 1 ) settings.inputType = 0 ;  // occupancy decoder
            if( lastPointAddress == 2 ) settings.inputType = 1 ;  // switch accessories

            gotoState( idle ) ;
        }
        break ;
    }
}

