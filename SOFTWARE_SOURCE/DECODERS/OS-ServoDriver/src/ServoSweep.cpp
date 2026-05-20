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


/**
 * @brief Construct a new Servo Sweep:: Servo Sweep object
 * 
 * @param _servoPin pin number of servo
 * @param _min      default position for the 0 state
 * @param _max      default position for the 1 state
 * @param _speed    time in ms between degrees
 * @param _turnOff  turn of the servo when in position
 */
ServoSweep::ServoSweep( uint8_t _servoPin  )                    // constructor 1
{
    servoPin   = _servoPin ;
    servoSpeed = 40 ;  
}

void ServoSweep::init()
{
    eedata = defaultSettings ;
    EEPROM.put( eeAddress, eedata ) ;
}

void ServoSweep::begin()
{
    if( eeAddress != 0xFFFF ) // If EEPROM present
    {

        EEPROM.get( eeAddress, eedata ) ;
        
        if( eedata.state ) { pos = eedata.servoMax ; }
        else               { pos = eedata.servoMin ; }
        

    }

    detachDelay.set( TIMER_ON, 500 ) ;
    timer.set( TIMER_BLEEP, servoSpeed ) ;
    startTrigger.arm() ;

    startUp = 1 ;
}

void ServoSweep::setState( uint8_t _state )
{
    eedata.state = _state ;
    EEPROM.put( eeAddress, eedata ) ;
}

uint8_t ServoSweep::getState()
{
    return eedata.state ;
}

void ServoSweep::setMin( uint8_t _min)
{
    eedata.servoMin = _min ;
    EEPROM.put( eeAddress, eedata ) ;
}

void ServoSweep::setMax( uint8_t _max)
{
    eedata.servoMax = _max ;
    EEPROM.put( eeAddress, eedata ) ;
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
    if( eedata.state )  { if( eedata.servoMax <= 160 ) { eedata.servoMax ++ ; } }
    else {                if( eedata.servoMin <= 160 ) { eedata.servoMin ++ ; } }
}

void ServoSweep::decrement()
{
    if( eedata.state )  { if( eedata.servoMax >= 20 ) { eedata.servoMax -- ; } }
    else {                if( eedata.servoMin >= 20 ) { eedata.servoMin -- ; } }
}

void ServoSweep::commitPos()
{
    EEPROM.put( eeAddress, eedata ) ;
}

void ServoSweep::toggle()
{
    setState( !eedata.state ) ;
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

        if( eedata.state )  setPoint = eedata.servoMax ; // get set point
        else                setPoint = eedata.servoMin ;
        if( override )      setPoint = servoSetpoint ;

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
    firstAddress += sizeof( eedata ) ; // increment for next servoSweep object
}


void ServoSweep::useEEPROM( uint16_t _eeAddress ) // this one is needed for the very first servo object.
{
    setEeAddress( _eeAddress ) ;
}

void ServoSweep::useEEPROM( )                   // use this one for all the others
{
    setEeAddress( 0x0000 ) ;
}

