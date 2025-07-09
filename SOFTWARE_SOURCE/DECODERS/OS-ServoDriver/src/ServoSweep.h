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

#include <Arduino.h>
#include <Servo.h>
#include "ST.h"

const int NO_POS = 0xFF ;

class ServoSweep
{
public:
    ServoSweep( uint8_t _servoPin, uint8_t _min, uint8_t _max, uint8_t _speed  ) ;
    void sweep( ) ;
    void setState( uint8_t _state ) ;
    uint8_t getState() ;
    void begin( ) ;
    void init( ) ;
    void setMin( uint8_t _min) ;
    void setMax( uint8_t _max) ;
    void enable() ;
    void increment( ) ;
    void decrement( ) ;
    void commitPos( ) ;
    void useEEPROM( uint16_t _eeAddress ) ;
    void useEEPROM( ) ;
    void toggle( ) ;
    void manualOverride( uint8_t pos ) ;
    void manualRelease() ;

private:
    void        setEeAddress( uint16_t _eeAddress ) ;
    Servo       servo ;
    uint8_t     pos ;
    uint8_t     prevPos ;
    uint8_t     override    : 1;
    uint8_t     enabled     : 1;
    uint8_t     state       : 1;
    uint8_t     servoPin    : 6;
    uint8_t     startUp     : 1 ;
    uint8_t     servoSpeed ;
    uint8_t     servoMin ;
    uint8_t     servoMax  ;
    uint8_t     servoSetpoint ;
    uint16_t    eeAddress = 0xFFFF ;
    Trigger     startTrigger ;
    Timer       detachDelay ;
    Timer       timer ;
} ;