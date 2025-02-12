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
// double coil pulsed, with frog relay (buddy pins)


CoilDriver::CoilDriver()
{
}

void CoilDriver::setPin( uint8_t _pinA, uint8_t _pinB ) // pins are fixed
// switchTime, lastState, type
{
    pinA = _pinA ;
    pinB = _pinB ;

    buddyPinA = 0xFF ;
    buddyPinB = 0xFF ;
}

void CoilDriver::setBuddyPins( uint8 _A, uint8 _B )
{
    buddyPinA = _A ;
    buddyPinB = _B ;
}

void CoilDriver::begin()
{
    pwmA.begin( pinA, 30, 100 ) ;
    pwmB.begin( pinB, 30, 100 ) ;
} 
const int FREE  = 0 ; 
const int IN_POSESSION = 1 ; 
uint8 CoilDriver::update()
{
    static uint8_t sharedToken =  FREE ;    // shared token prevents from double pulse types from being activated simultaneously.

    if( sharedToken == FREE    // if shared token is not claimed, I am a double pulse type and I am to set one of my coils, I must obtain the token.
    && (type == DOUBLE_PULSE_W_FROG || type == DOUBLE_COIL_PULSED)
    && (stateA == 1 || stateB == 1) )
    {
        sharedToken   = IN_POSESSION ; 
        myToken       = IN_POSESSION ;
    }
    if( type != DOUBLE_PULSE_W_FROG && type != DOUBLE_COIL_PULSED ) { // if I am not double pulse, I may always have my own token, and leave the shared token for others to use
        myToken = IN_POSESSION;
    }

    if( type == DOUBLE_PULSE_W_FROG && myToken == IN_POSESSION )
    {
        startA.update( stateA ) ;  stopA.update( stateA ) ;
        startB.update( stateB ) ;  stopB.update( stateB ) ;

        if( startA.Q || startB.Q )  // if either state becomes true, kill the relay pins
        {
            digitalWrite( buddyPinA, LOW ) ;
            digitalWrite( buddyPinB, LOW ) ;
        }

        if( stopA.Q ) digitalWrite( buddyPinA, HIGH ) ; // if a state becomes low, the pulse has finished and a relay pin may be set.
        if( stopB.Q ) digitalWrite( buddyPinB, HIGH ) ;
    }
    
    if( myToken == IN_POSESSION 
                && 
    (   type    ==   SINGLE_COIL_PULSED
    ||  type    ==   DOUBLE_COIL_PULSED 
    ||  type    ==   DOUBLE_PULSE_W_FROG ) )
    {
        timerA.update( stateA ) ;       // timer starts when state become 1, 
        timerB.update( stateB ) ;

        if( timerA.Q ) { stateA = 0 ; } // when timer expires state becomes 0
        if( timerB.Q ) { stateB = 0 ; }

        if(stateA == 0 && stateB == 0 )  
        {  
            myToken     = FREE;  
            sharedToken = FREE;  
        }

        pwmA.setState( stateA ) ; // sets actual Outputs
        pwmB.setState( stateB ) ;
    }

    pwmA.update() ;
    pwmB.update() ;

    return 1 ;
}


void CoilDriver::setCoilExt( uint16 dccAddress, uint8 _aspect )
{
    if( type != SINGLE_COIL_PULSED ) return ;

    uint16    primaryAddress = myAddress ;
    uint16  secondaryAddress = primaryAddress + 1 ;
    uint32         pulseTime = _aspect * 1000 ; // aspect number is used for pulse length.
    
    if( dccAddress ==   primaryAddress ) timerA.setTime( pulseTime ) ; stateA = 1 ;
    if( dccAddress == secondaryAddress ) timerB.setTime( pulseTime ) ; stateB = 1 ;
}

void CoilDriver::setStates( uint8_t A, uint8_t B )
{
    stateA = A ;
    stateB = B ;
}

void CoilDriver::setCoil( uint16 dccAddress, uint8 dir )
{
    uint16 primaryAddress   = myAddress ;
    uint16 secondaryAddress = primaryAddress + 1 ;
   
    switch( type )
    {
    case SINGLE_COIL_PULSED:
    case SINGLE_COIL_CONTINUOUSLY:
        if( dccAddress ==   primaryAddress ) { stateA = dir ; }
        if( dccAddress == secondaryAddress ) { stateB = dir ; }
        break ;

    case DOUBLE_COIL_CONTINUOUSLY:
    case DOUBLE_PULSE_W_FROG:
    case DOUBLE_COIL_PULSED:
        if( dccAddress != primaryAddress ) break ;
        stateA =  dir ;
        stateB = !dir ;
        break ;
    }
}

uint8  CoilDriver::getState( uint8_t channel ) { if( channel == 0 ) return stateA ;
                                                 else               return stateB ; }

void   CoilDriver::setType( uint8_t _type ) { type = _type ; }
uint8  CoilDriver::getType() { return type ; }

void   CoilDriver::setAddress( uint16 _address ) { myAddress = _address ; }
uint16 CoilDriver::getAddress() { return myAddress ; }

void   CoilDriver::setPulseTime( uint16 _pulseTime ) { myPulseTime = _pulseTime ; }
uint16 CoilDriver::getPulseTime() { return myPulseTime ; }

void   CoilDriver::setDutyCycle( uint8_t dutycycle )
{
    pwmA.setSpeed( dutycycle ) ;
    pwmB.setSpeed( dutycycle ) ;
}
uint8_t CoilDriver::getDutyCycle() { return pwmA.getSpeed() ; }

uint8  CoilDriver::isActive() { return active ; }