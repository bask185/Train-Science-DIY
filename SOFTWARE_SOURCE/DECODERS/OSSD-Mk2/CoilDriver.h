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


enum types
{
    DOUBLE_COIL_PULSED,
    DOUBLE_COIL_CONTINUOUSLY,
    SINGLE_COIL_PULSED,
    SINGLE_COIL_CONTINUOUSLY,
} ;

class CoilDriver {

public:
    CoilDriver() ;
    void    begin( uint8, uint8 ) ; // used during boot
    uint8   update() ;

    void    setCoilExt( uint16, uint8 ) ;
    void    setCoil(    uint16, uint8 ) ;

    void    setType( uint8 ) ;
    void    setAddress( uint16 ) ;
    uint8   getType() ;
    uint16  getAddress() ;
    void    setPulseTime( uint16 ) ;
    uint16  getPulseTime() ;

    uint8   isActive() ;


private:
    uint16 inputVoltage: 10 ;
    uint8   sm         :  3 ;
    uint8   active     :  1 ;
    uint8   type       :  3 ;
    uint8   has_CDU    :  1 ;
    uint8   state      :  1 ;
    uint8   switchPin  :  5 ;
    uint8   voltagePin :  5 ;
    uint8   pinA       :  5 ;
    uint8   pinB       :  5 ;
    uint32  pulseTime  : 32 ;
    uint32  prevTime   : 32 ;
    uint32  myPulseTime: 32 ;
    uint16  myAddress  : 16 ;

    void    pulse( uint8 _pin, uint32 _pulseTime ) ;
} ;