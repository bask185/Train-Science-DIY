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

