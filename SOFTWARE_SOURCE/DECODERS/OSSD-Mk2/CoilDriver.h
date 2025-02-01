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

#include <Arduino.h>
#include "src/macros.h"
#include "Weistra.h"
#include "src/Trigger.h"
#include "src/Timers.h"

enum types
{
    DOUBLE_COIL_PULSED,
    DOUBLE_COIL_CONTINUOUSLY,
    SINGLE_COIL_PULSED,
    SINGLE_COIL_CONTINUOUSLY,
    DOUBLE_PULSE_W_FROG,
} ;

class CoilDriver {

public:
    CoilDriver() ;
    void    setPin( uint8, uint8 ) ; 
    void    setBuddyPins( uint8, uint8 ) ;
    void    begin() ;
    
    uint8   update() ;

    void    setCoilExt( uint16, uint8 ) ;
    void    setCoil(    uint16, uint8 ) ;

    void    setStates( uint8_t, uint8_t ) ;
    uint8   getState( uint8_t ) ;
    void    setType( uint8 ) ;
    uint8   getType() ;
    void    setAddress( uint16 ) ;
    uint16  getAddress() ;
    void    setPulseTime( uint16 ) ;
    uint16  getPulseTime() ;
    void    setDutyCycle( uint8_t ) ;
    uint8_t getDutyCycle( ) ;
    uint8   isActive() ;

private:
    uint16 inputVoltage: 10 ;
    uint8   sm         :  3 ; // obsolete
    uint8   active     :  1 ; // obsolete
    uint8   type       :  3 ;
    uint8   stateA     :  1 ;
    uint8   stateB     :  1 ;
    uint8   pinA       :  5 ;
    uint8   pinB       :  5 ;
    uint32  myPulseTime: 32 ;
    uint16  myAddress  : 16 ;
    uint8   buddyPinA  :  5 ;  // when frog Relay are used, these pins are used to set the relays
    uint8   buddyPinB  :  5 ;
    uint8   myToken    :  1 ; 

    R_trigger startA ;
    R_trigger startB ;

    F_trigger stopA ;
    F_trigger stopB ;

    TON_timer timerA ;
    TON_timer timerB ;

    Weistra   pwmA ; // general Idea is to use the Weistra class to handle software PWM. We may need as many as 16 objects, 2 per coil.
    Weistra   pwmB ; // we might as well stuff this inside the coil class..
} ;