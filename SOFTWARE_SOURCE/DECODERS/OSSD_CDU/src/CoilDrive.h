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
#include "macros.h"

class CoilDrive {

public:
    CoilDrive( ) ;
    void    setState( uint8 ) ;
    void    setPins( uint8, uint8 ) ;
    void    setPulseTime( uint32 ) ;
    uint8   update() ;
    void    begin() ;
    uint8   due() ;

private:
    uint32  switchTime ;
    uint32  prevTime ;
    uint8   pinA ;
    uint8   pinB ;
    uint8   state ;
    bool    set ;
    uint8   due_ ;
} ;