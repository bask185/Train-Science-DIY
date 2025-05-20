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
#include "pwm.h"

// double coil pulsed
// double coil continously
// single coil pulsed
// single coil Continously
// double coil pulsed, with frog relay (buddy pins)

const int OFF = 0 ;
const int  ON = 1 ;
const int   A = 0 ;
const int   B = 1 ;

CoilDriver::CoilDriver()
{
    buddyPinA = 0xFF ;
    buddyPinB = 0xFF ;
}

void CoilDriver::setBuddyPins( uint8 _B, uint8 _A ) // B is in front to mirror outputs deliberately
{
    buddyPinA = _A ;
    buddyPinB = _B ;
}

void CoilDriver::begin( uint8 _A, uint8 _B )
{
    pinA = _A ;
    pinB = _B ;
    lockoutTimer.setTime( 1000 ) ; // 'debounce' lockout time for repetetive DCC messages.
} 

void CoilDriver::initializeCoils()
{
    if( type == DOUBLE_COIL_PULSED
    ||  type == SINGLE_COIL_PULSED
    ||  type == DOUBLE_PULSE_W_FROG )
    {
        turn( pinA, OFF ) ;
        turn( pinB, OFF ) ;
    }

    if( type == DOUBLE_PULSE_W_FROG )
    {
        digitalWrite( buddyPinA,  oldStateA ) ;
        digitalWrite( buddyPinB,  oldStateB ) ; 
    }

    if( type == SINGLE_COIL_CONTINUOUSLY
    ||  type == DOUBLE_COIL_CONTINUOUSLY )
    {
        state2beA = oldStateA ; // this will ensure that the ouput goes on
        state2beB = oldStateB ;
    }
}

void CoilDriver::reset()
{
    outputStateA = state2beA = oldStateA = 0 ;
    outputStateB = state2beB = oldStateB = 0 ;

    turn( pinA, OFF ) ;
    turn( pinB, OFF ) ;

    if( type == DOUBLE_PULSE_W_FROG )
    {
        digitalWrite( buddyPinA,  LOW ) ;
        digitalWrite( buddyPinB,  LOW ) ;
    }
}

const int         FREE = 0 ; 
const int IN_POSESSION = 1 ; 

void CoilDriver::turn( uint8 pin, uint8 newState )
{
    if( type == DOUBLE_COIL_PULSED      // double pulse only, directly set outputs for faster and more accurate response time
    ||  type == DOUBLE_PULSE_W_FROG )
    {
        digitalWrite( pin, newState ) ;
    }

    else                                // all other modes may make use of PWM
    {
        if( newState == 1 ) {   subscribePwm( pinA, dutycycle ) ; } 
        else                { unsubscribePwm( pinB ) ;            } 
    }
}

uint8 CoilDriver::update()
{
    if( type == DORMENT )  return 1 ;

    if( lockoutTimer.update( lockout ) ) { lockout = 1 ; }
    
    static uint8_t sharedToken =  FREE ;

    // a double pulsed output require posession of the shared token before the outputs may be turned on.
    // The token prevents from more than one output being activated at the same time. This protects CDU, power supply, decoder and ensures no overcurrent is tripped.
    // The shared token is static by design and shared beteen all 8 coil objects.

    if( sharedToken == FREE   
    && (type == DOUBLE_PULSE_W_FROG || type == DOUBLE_COIL_PULSED)
    && (state2beA == 1 || state2beB == 1) )
    {
        sharedToken    =  IN_POSESSION ; 
        myToken        =  IN_POSESSION ;

        outputStateA   =  state2beA ;
        outputStateB   =  state2beB ;
        
        state2beA      = 0 ;
        state2beB      = 0 ;
    }

    // if I am not double pulse, I can always have my own token, and leave the shared token free for others to use. Also update ouputs at once
    if( !((type == DOUBLE_PULSE_W_FROG) || (type == DOUBLE_COIL_PULSED)) )
    { 
        myToken        = IN_POSESSION ;      
        outputStateA   =    state2beA ;
        outputStateB   =    state2beB ;

        state2beA      = 0 ;
        state2beB      = 0 ;
    }
    
    // only used for pulsed modi. Timer is examined here to check when the pulse needs to to be stopped.
    timerA.update( outputStateA ) ;
    timerB.update( outputStateB ) ;

    if( myToken == IN_POSESSION 
                && 
    (   type    ==   SINGLE_COIL_PULSED
    ||  type    ==   DOUBLE_COIL_PULSED 
    ||  type    ==   DOUBLE_PULSE_W_FROG ) )
    {
        if( timerA.Q ) { outputStateA = 0 ; } // when timer expires states become 0
        if( timerB.Q ) { outputStateB = 0 ; }

        // a double pulsed coil must release the Token 
        if( (type == DOUBLE_COIL_PULSED || type == DOUBLE_PULSE_W_FROG)
        &&  outputStateA == 0 && outputStateB == 0 )  
        {  
            myToken     = FREE;  
            sharedToken = FREE;
        }
    }

    if( riseA.trigger( outputStateA ) ) { turn( pinA, ON ) ; } // rising flank -> set output
    if( riseB.trigger( outputStateB ) ) { turn( pinB, ON ) ; }

    if( fallA.trigger( outputStateA ) ) { turn( pinA, OFF ) ; } // falling flank -> kill output
    if( fallB.trigger( outputStateB ) ) { turn( pinB, OFF ) ; }

    // this part handles the buddy pins for DOUBLE_PULSE_W_FROG mode
    if( type == DOUBLE_PULSE_W_FROG ) 
    {
        if( riseA.Q || riseB.Q )  // if either state becomes true, kill both relay pins
        {
            digitalWrite( buddyPinA, LOW ) ;
            digitalWrite( buddyPinB, LOW ) ;
        }

        if( fallA.Q ) { digitalWrite( buddyPinA, HIGH ) ; } // the pulse has finished and one of the relay pins may be set.
        if( fallB.Q ) { digitalWrite( buddyPinB, HIGH ) ; }
    }

    if( type == DOUBLE_COIL_PULSED || type == DOUBLE_PULSE_W_FROG ) // double pulse coils, may only return true when deactivated.
    {
        if( outputStateA == 0 && outputStateB == 0 ) return 1 ;
        else                                         return 0 ;
    }

    else return 1 ;
}


uint8_t CoilDriver::setCoilExt( uint16 dccAddress, uint8 _aspect )
{
    if( type != SINGLE_COIL_PULSED ) return 0 ; // and 

    uint16    primaryAddress = myAddress ;
    uint16  secondaryAddress = primaryAddress + 1 ;
    uint32         pulseTime = _aspect * 1000 ; // aspect number is used for pulse length in seconds.

    if( dccAddress ==   primaryAddress ) { timerA.setTime( pulseTime ) ; outputStateA = 1 ; return 1 ; }
    if( dccAddress == secondaryAddress ) { timerB.setTime( pulseTime ) ; outputStateB = 1 ; return 1 ; }

    return 0 ;
}

// may become obsolete in favor of initialize IO.
void CoilDriver::setStates( uint8_t A, uint8_t B )
{
    oldStateA = A ;
    oldStateB = B ;
}

uint8 CoilDriver::setCoil( uint16 dccAddress, uint8 dir, uint8 override )
{
    if( lockout == 0 && ! override )    {  return 0 ; }
    if( type    == DORMENT )            {  return 0 ; }
    else                                {  lockout = 0 ; }
    
    uint16 primaryAddress   = myAddress ;
    uint16 secondaryAddress = myAddress + 1 ;
   
    switch( type )
    {
    
    case SINGLE_COIL_CONTINUOUSLY:
        oldStateA = state2beA ;
        oldStateB = state2beB ;
    case SINGLE_COIL_PULSED:
        if( dccAddress ==   primaryAddress ) { state2beA = dir ; return 1 ; }
        if( dccAddress == secondaryAddress ) { state2beB = dir ; return 1 ; }
        return 0 ;

    case DOUBLE_COIL_CONTINUOUSLY:
    case DOUBLE_PULSE_W_FROG:
    case DOUBLE_COIL_PULSED:
        if( dccAddress != primaryAddress ) break ;  
        state2beA = !dir ; oldStateA = state2beA ;
        state2beB =  dir ; oldStateB = state2beB ;
        return 1 ;
    
    default:
        return 0 ;
    }
}

uint8  CoilDriver::getState( uint8_t channel ) { if( channel == 0 ) return oldStateA ;
                                                 else               return oldStateB ; }

void   CoilDriver::setType( uint8_t _type ) { type = _type ; }
uint8  CoilDriver::getType() { return type ; }

void   CoilDriver::setAddress( uint16 _address ) { myAddress = _address ; }
uint16 CoilDriver::getAddress() { return myAddress ; }

uint16 CoilDriver::getPulseTime() { return myPulseTime ; }
void   CoilDriver::setPulseTime( uint32 _pulseTime )
{
    myPulseTime = _pulseTime ;
    timerA.setTime( myPulseTime ) ;
    timerB.setTime( myPulseTime ) ;
}

void   CoilDriver::setDutyCycle( uint8_t _dutycycle ) { dutycycle = _dutycycle ; }
uint8  CoilDriver::getDutyCycle() { return dutycycle ; }
