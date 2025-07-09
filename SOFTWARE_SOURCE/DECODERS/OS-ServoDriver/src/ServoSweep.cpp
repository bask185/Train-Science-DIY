/*
 * Copyright (C) 2024 Sebastiaan Knippels, Train-Science
 *
 * To the extent possible under law, the person who associated CC0 with this work
 * has waived all copyright and related or neighboring rights to this work.
 *
 * This work is published from: The Netherlands
 *
 * You can copy, modify, distribute and perform the work, even for commercial purposes,
 * all without asking permission. See the full license for details at:
 * https://creativecommons.org/publicdomain/zero/1.0/
 */

/*
ServoSweep library written by S Knippels
Public Domain
*/

#include "ServoSweep.h"
#include "macros.h"
#include <EEPROM.h>

// use constructor 1 if you have no optional relay
// use the other if you 

const int STORE_POSITIONS   = 0b10000000 ;
const int DEFAULT_BITS      = 0b00111110 ;

const int SERVO_EE_SIZE = 3 ;
/**
 * @brief Construct a new Servo Sweep:: Servo Sweep object
 * 
 * @param _servoPin pin number of servo
 * @param _min      default position for the 0 state
 * @param _max      default position for the 1 state
 * @param _speed    time in ms between degrees
 * @param _turnOff  turn of the servo when in position
 */
ServoSweep::ServoSweep( uint8_t _servoPin, uint8_t _min, uint8_t _max, uint8_t _speed )                    // constructor 1
{
    servoPin = _servoPin ;
    servoSpeed = _speed ;
    servoMin = _min ;
    servoMax = _max ;
   
}

void ServoSweep::init()
{
    EEPROM.write( eeAddress+0, servoMin ) ;
    EEPROM.write( eeAddress+1, servoMax ) ;
    EEPROM.write( eeAddress+2, 0 ) ; // last state
}

void ServoSweep::begin()
{
    if( eeAddress != 0xFFFF ) // If EEPROM present
    {

        servoMin = EEPROM.read( eeAddress+0 ) ;
        servoMax = EEPROM.read( eeAddress+1 ) ;

        uint8 status = EEPROM.read(eeAddress+2) ;
        state = bitRead( status, 0 ) ;
        
        if( status & 1 ) { pos = servoMax ; state = 1 ; }
        else             { pos = servoMin ; state = 0 ; }
        

    }

    detachDelay.set( TIMER_ON, 500 ) ;
    timer.set( TIMER_BLEEP, servoSpeed ) ;
    startTrigger.arm() ;

    startUp = 1 ;
}

void ServoSweep::setState( uint8_t _state )
{
    state = _state ;

    uint8_t newState = EEPROM.read( eeAddress+2 ) ;
    bitWrite( newState, 0, state ) ;
    EEPROM.write( eeAddress+2, newState ) ;    
}

uint8_t ServoSweep::getState()
{
    return state ;
}

void ServoSweep::setMin( uint8_t _min)
{
    servoMin = _min ;
    EEPROM.write( eeAddress+0, servoMin ) ;
}

void ServoSweep::setMax( uint8_t _max)
{
    servoMax = _max ;
    EEPROM.write( eeAddress+1, servoMax ) ;
}

void ServoSweep::manualOverride( uint8_t pos)
{
    servoSetpoint = pos ;
    override = 1 ;
}

void ServoSweep::manualRelease()
{
    override = 0 ;
}

void ServoSweep::increment()
{
    if( state )  { if( servoMax <= 160 ) { servoMax ++ ; } }
    else {         if( servoMin <= 160 ) { servoMin ++ ; } }
}

void ServoSweep::decrement()
{
    if( state )  { if( servoMax >= 20 ) { servoMax -- ; } }
    else {         if( servoMin >= 20 ) { servoMin -- ; } }
}

void ServoSweep::commitPos()
{
    EEPROM.update( eeAddress+0, servoMin ) ;
    EEPROM.update( eeAddress+1, servoMax ) ;
}

void ServoSweep::toggle()
{
    setState( !state ) ;
}

void ServoSweep::enable()
{
    enabled = 1 ;
}


void ServoSweep::sweep ( )
{
    if( !enabled ) return ;

    if( timer.update( 1 ) ) 
    {
        uint8_t setPoint ;

        if( state )     setPoint = servoMax ; // get set point
        else            setPoint = servoMin ;
        if( override )  setPoint = servoSetpoint ;

        if( pos < setPoint ) pos ++ ;       // follow positon to setPoint
        if( pos > setPoint ) pos -- ;

        if( servo.attached() ) servo.write( pos ) ; // write the setpoint

        startTrigger.update( prevPos != pos ) ;
        detachDelay.update(  prevPos == pos ) ;

        if( startTrigger.rose() && !servo.attached() ) { servo.attach( servoPin ) ; servo.write( pos ) ; }
        if( detachDelay.Q       &&  servo.attached() ) { servo.detach() ; }

        prevPos  = pos ;
    }
}

void ServoSweep::setEeAddress( uint16_t _eeAddress )
{
    static uint16 firstAddress = 0xFFFF ; // delibarately used static to automatically increase addresses

    if( firstAddress == 0xFFFF )
    {
        firstAddress = _eeAddress ;
    }

    eeAddress     =  firstAddress ;      // set my own eeAddress
    firstAddress += SERVO_EE_SIZE ; // increment for next servoSweep object
}


void ServoSweep::useEEPROM( uint16_t _eeAddress ) // this one is needed for the very first servo object.
{
    setEeAddress( _eeAddress ) ;
}

void ServoSweep::useEEPROM( )                   // use this one for all the others
{
    setEeAddress( 0x0000 ) ;
}

