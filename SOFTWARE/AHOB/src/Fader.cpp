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

#include "Fader.h"
#include "../config.h"

Fader::Fader( uint8_t _pin )
{
    pin = _pin ;
}

void Fader::begin()
{
    pinMode( pin, OUTPUT ) ;
}

void Fader::setState( uint8_t _state )
{
    state = _state ;
}

void Fader::update()
{
    uint32_t currTime = micros() ;

    if( currTime - prevTime >= fadeTime ) // toggle on last bit only
    {       prevTime = currTime ;

        if( state ) if( pwm < 255 ) pwm ++ ;
        else        if( pwm >   0 ) pwm -- ;

        analogWrite( pin, pwm ) ;
    }
}

uint8_t Fader::getState()
{
    return state ;
}

