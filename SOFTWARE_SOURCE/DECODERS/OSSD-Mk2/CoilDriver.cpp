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

// Written By: Sebastiaan Knippels. This library is public domain

#include "CoilDriver.h"
#include <EEPROM.h>

// double coil pulsed
// double coil continously
// single coil pulsed
// single coil Continously


CoilDriver::CoilDriver()
{
}

void CoilDriver::begin( uint8_t _pinA, uint8_t _pinB ) // pins are fixed
// switchTime, lastState, type
{
    pinA = _pinA ;
    pinB = _pinB ;
    pinMode( pinA, OUTPUT ) ;
    pinMode( pinB, OUTPUT ) ;
}


enum coilStates 
{
    idle,
    waitForCharge,
    startingPulse,
    pulsing,
    lockout,
} ;

void CoilDriver::pulse( uint8 _pin, uint32 _pulseTime )
{
    if( sm == idle )
    {
        switchPin = _pin ;
        pulseTime = _pulseTime ;
        if( has_CDU ) { sm = waitForCharge ; }
        else          { sm = startingPulse ; }
    }
}

uint8 CoilDriver::update()
{
    switch( sm )
    {
    case idle:
        return 1 ;

    case waitForCharge:
        {
            int chargeLevel = analogRead( voltagePin ) ;

            if( chargeLevel >= (inputVoltage - 25) )
            {
                sm = startingPulse ;
            }
        }
        break ;

    case startingPulse:
        digitalWrite( switchPin , HIGH ) ;
        active = 1 ;
        prevTime = millis() ;
        sm ++ ;
        return 0 ;

    case pulsing:
        if( millis() - prevTime >= pulseTime )
        {
            digitalWrite( switchPin, LOW ) ;
            active = 0 ;
            sm ++ ;
        }
        return 0 ;

    case lockout:
        if( millis() - prevTime >= 150 ) sm = idle ; // wait for repetetive DCC messages to pass UNSURE IF NEEDED, but it should not bite
        return 1 ;
    }

    return 0;// noWaitPulseFlag ; // this is true, if there is no wait needed
}

/* SETTERS 
    If it is a continous mode, just set the output.
    continuous coils, do not except extended instructions
*/


void CoilDriver::setCoilExt( uint16 dccAddress, uint8 _aspect )
{
    if( type == DOUBLE_COIL_CONTINUOUSLY ) return ;
    if( type == SINGLE_COIL_CONTINUOUSLY ) return ;
    if( dccAddress != myAddress )          return ;

    uint16 beginAddress = myAddress ;
    uint16   endAddress = beginAddress ;
    uint32    pulseTime = _aspect * 100 ; // aspect number is used for pulse length.

    if( type == SINGLE_COIL_PULSED ) endAddress ++ ;

    if( dccAddress >= beginAddress 
    &&  dccAddress <= endAddress )
    {
        if( type == SINGLE_COIL_PULSED )
        {
            if( dccAddress == beginAddress ) pulse( pinA, pulseTime ) ;
            else                             pulse( pinB, pulseTime ) ;
        }
        else // double coil pulsed
        {
            if( state ) pulse( pinA, pulseTime ) ;
            else        pulse( pinB, pulseTime ) ;
        }
    }
}

void CoilDriver::setCoil( uint16 dccAddress, uint8 dir )
{
    uint16 beginAddress = myAddress ;
    uint16   endAddress = beginAddress ;
    state = dir ;
    if( type == SINGLE_COIL_CONTINUOUSLY 
    ||  type == SINGLE_COIL_PULSED ) endAddress ++ ; // if we are single, we have 2 addresses not 1

    if( dccAddress >= beginAddress 
    &&  dccAddress <= endAddress )
    {
        switch( type )
        {
        case SINGLE_COIL_CONTINUOUSLY:
            if( dccAddress == beginAddress ) digitalWrite( pinA, state ) ;
            else                             digitalWrite( pinB, state ) ;
            break ;

        case DOUBLE_COIL_CONTINUOUSLY:
            digitalWrite( pinA,  state ) ;
            digitalWrite( pinB, !state ) ;
            break ;

        case SINGLE_COIL_PULSED:
            if( dccAddress == beginAddress ) pulse( pinA, myPulseTime*100 ) ;
            else                             pulse( pinB, myPulseTime*100 ) ;
            break ;

        case DOUBLE_COIL_PULSED:
            if( state ) pulse( pinA, myPulseTime*10 ) ;
            else        pulse( pinB, myPulseTime*10 ) ;
            break ;
        }
    }
}

void CoilDriver::setType( uint8_t _type )
{
    type = _type ;
}

void CoilDriver::setAddress( uint16 _address )
{
    myAddress = _address ;
}

void CoilDriver::setPulseTime( uint16 _pulseTime )
{
    myPulseTime = _pulseTime ;
}

/* GETTERS */


uint16 CoilDriver::getAddress()
{
    return myAddress ;
}

uint8_t CoilDriver::getType()
{
    return type ;
}

uint16 CoilDriver::getPulseTime()
{
    return myPulseTime ;
}

uint8 CoilDriver::isActive()
{
    return active ;
}