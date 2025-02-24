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
 * 
 * modified for OSSD MK2 code
 */

#include "weistra.h"


Weistra::Weistra( ) 
{
}

void Weistra::begin( uint8_t pin, uint8_t _Fmin, uint8_t _Fmax )
{
    trackPin1 = pin;
    Fmin = _Fmin ;
    Fmax = _Fmax ;

    //pinMode(trackPin1, OUTPUT); done in application code 

    uint8_t port1 = digitalPinToPort( trackPin1 );
    trackPin1     = digitalPinToBitMask( trackPin1 );
    portx_p1      = portOutputRegister( port1 );

    state = 0 ;
}

void Weistra::setState( uint8_t _state )
{ 
    state = _state ;
    counter = 0 ;
    prevTime = micros() + intervalTime ; // enforce quick update
}

void Weistra::update() 
{
    if( portx_p1 )
    {
        uint32_t currentTime = micros() ; 

        if( currentTime - prevTime >= intervalTime )
        {   prevTime = currentTime ;

            if( !state )
            {
                *portx_p1 &= ~trackPin1 ;
            }
            else if( counter == 0 && newDutyCycle > 0 )      // if counter reaches 100, reset it to 0 and enable the track power pin
            {
                *portx_p1 |=  trackPin1 ;

                dutyCycle = newDutyCycle ;              // a new dutycucle can only be accepted on the beginning of a cycle, this prevents weird jumps of the trains
                intervalTime = newIntervalTime ;        // new speed is accepted at the beginning of a cycle
            }
            if( counter == dutyCycle && dutyCycle < 100 ) // 100% -> pin needs to remain on 
            {
                *portx_p1 &= ~trackPin1 ;
            }
            
            if( ++counter > 100) counter = 0 ;
        }
    }
}


void Weistra::setSpeed( int8_t speed) // parameter printed in coildriver
{
    byte frequency;

    speed = abs( speed ) ;

    newDutyCycle = constrain( speed, 0, 100 ) ; // speed limit = 0 - 100

    if( newDutyCycle <= 10 ) { frequency = Fmin; }
    else                     { frequency = map( newDutyCycle, 10, 100, Fmin, Fmax ) ; }

    newIntervalTime = 10000 / frequency; // > between 100us and 500us
}

uint8_t Weistra::getSpeed()
{
    return newDutyCycle ;
}

