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

#include "CoilDrive.h"

CoilDrive::CoilDrive( )
{
}

void CoilDrive::begin()
{
    pinMode( pinA, OUTPUT ) ;
    pinMode( pinB, OUTPUT ) ;
}

void CoilDrive::setPins( uint8 _pinA, uint8 _pinB )
{
    pinA = _pinA ;
    pinB = _pinB ;
}

void CoilDrive::setPulseTime( uint32 _switchTime )
{
    switchTime = _switchTime ;
}

void CoilDrive::setState( uint8 _state )
{
    due_ = 1 ;
    state = _state ;
    set = false ;
}

uint8 CoilDrive::update()
{
    if( set == false )
    { 
        if( state ) digitalWrite( pinA, HIGH ) ;
        else        digitalWrite( pinB, HIGH ) ;

        prevTime = millis() ;
        set = true ;
    }

    if( (digitalRead(pinA) || digitalRead(pinB) )
    &&  (millis() - prevTime) >= switchTime )
    {        
        digitalWrite( pinA, LOW ) ;
        digitalWrite( pinB, LOW ) ;
    }
}

uint8 CoilDrive::due()
{
    uint8 retVat = due_ ;
    due_ = 0 ;
    return due_ ;
}