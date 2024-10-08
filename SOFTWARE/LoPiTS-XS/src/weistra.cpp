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

#include "weistra.h"


Weistra::Weistra(uint8_t _pin, uint8_t _Fmin, uint8_t _Fmax ) 
{ // constructor
    trackPin1 = _pin;
    trackPin2 = 255 ;
    Fmin = _Fmin ;
    Fmax = _Fmax ;
    sensePin = 255 ;
}

Weistra::Weistra(uint8_t _pin1, uint8_t _pin2, uint8_t _Fmin, uint8_t _Fmax ) 
{ // constructor
    trackPin1 = _pin1;
    trackPin2 = _pin2;
    Fmin = _Fmin ;
    Fmax = _Fmax ;
    sensePin = 255 ;
} 


void Weistra::begin()
{
    pinMode(trackPin1, OUTPUT);
    pinMode(trackPin2, OUTPUT);

    uint8_t port1 = digitalPinToPort( trackPin1 );
    trackPin1     = digitalPinToBitMask( trackPin1 );
    portx_p1      = portOutputRegister( port1 );

    if( trackPin2 != 255 )
    {
        uint8_t port2 = digitalPinToPort( trackPin2 );
        trackPin2     = digitalPinToBitMask( trackPin2 );
        portx_p2      = portOutputRegister( port2 );
    }

    state = 1 ;
}

void Weistra::setCurrentSense( uint8_t _sensePin, uint8_t _currentLimit )
{
    sensePin = _sensePin ;
    currentLimit = _currentLimit ;
}

void Weistra::update() 
{
    if( portx_p1 || portx_p2 )
    {
        uint32_t currentTime = micros() ; 

        if( currentTime - prevTime >= intervalTime )
        { 
            prevTime = currentTime;

            if( counter == 0 && newDutyCycle > 0 )      // if counter reaches 100, reset it to 0 and enable the track power pin
            {
                if(dir) *portx_p1 |=  trackPin1 ;
                else    *portx_p2 |=  trackPin2 ;

                dutyCycle = newDutyCycle ;              // a new dutycucle can only be accepted on the beginning of a cycle, this prevents weird jumps of the trains
                intervalTime = newIntervalTime ;        // new speed is accepted at the beginning of a cycle
            }
            if( counter == dutyCycle /*&& dutyCycle < 100*/ ) // commented code seems buggy??
            {
                *portx_p1 &= ~trackPin1;
                *portx_p2 &= ~trackPin2;
            }
            if( ++counter > 100) counter = 0 ;
        }

        // if( sensePin != 255 )
        // {
        //     int current = analogRead( sensePin ) ;

        //     if( millis() - prevTime2 > interval )
        //     {     prevTime2 = millis() ;

        //         if( state ) // if on
        //         {
        //             if( current > currentLimit ) currentCounter -- ;
        //             else                         currentCounter = 5 ;

        //             if( currentCounter == 0 )
        //             {
        //                 setState( 0 ) ;
        //                 interval = 5000 ; // 5s power off
        //             }
        //         }
        //         else        // if off
        //         {
        //             setState( 1 ) ; // turn on again
        //             interval = sampleRate ; // set sample rate 
        //         }
        //     }
        // }
    }
}


void Weistra::setSpeed( int8_t speed)
{
    byte frequency;
    if( speed < 0 ) dir = 1 ; // 1 is reverse
    if( speed > 0 ) dir = 0 ;

    speed = abs( speed ) ;
    if( speed > 0 ) speed = map( speed, 0, 100, minSpeed, maxSpeed ) ; // if speed is not 0, map speed accoring to min and max settings.

    newDutyCycle = constrain( speed, 0, 100 ) ; // speed limit = 0 - 100

    if( newDutyCycle <= 10 ) { frequency = Fmin; }
    else                     { frequency = map( newDutyCycle, 10, 100, Fmin, Fmax ) ; }

    newIntervalTime = 10000 / frequency; // > between 100us and 500us
}

void Weistra::setState( uint8_t _state )
{ 
    state = _state ;
    
    if( !state )
    {
        *portx_p1 &= ~trackPin1 ;
        *portx_p2 &= ~trackPin2 ;
    }
    else
    {
        counter = 0 ;
    }
}

uint8_t Weistra::getState( ) 
{
    return state ;
}

uint8_t Weistra::kickStart( uint32_t _kickStartTime ) // sets a kickstart pulse for a certain duration
{
    if( state == 1 )
    {   state = 0 ;

        *portx_p1 &= ~trackPin1 ; // ensure they are both OFF
        *portx_p2 &= ~trackPin2 ;

        if(dir) *portx_p1 |=  trackPin1 ; // set one high
        else    *portx_p2 |=  trackPin2 ;

        prevTime2 = millis() ;          // load the time
    }

    if( millis() - prevTime2 > _kickStartTime // and wait...
    &&  state == 0 )
    {     
        state = 1 ;
        *portx_p1 &= ~trackPin1 ;   // kill both outputs
        *portx_p2 &= ~trackPin2 ;

        return 1 ;                  // flag ready
    }

    return 0 ;
}


void Weistra::setMin( uint8_t _minSpeed )
{
    minSpeed = constrain( _minSpeed, 0, maxSpeed-10 ) ;
}

void Weistra::setMax( uint8_t _maxSpeed )
{
    maxSpeed = constrain( _maxSpeed, minSpeed+10, 100 ) ;
}