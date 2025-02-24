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

#ifndef TRIGGER_H
#define TRIGGER_H

class R_trigger // Rising trigger, Q is true if IN becomes true
{
public:
    R_trigger( ) ;
    uint8_t  trigger( uint8 ) ;
    uint8     Q : 1 ;
    uint8   old : 1 ;
} ;

class F_trigger // Falling trigger, Q is true if IN becomes false
{
public:
    F_trigger( ) ;
    uint8_t  trigger( uint8 )  ;
    uint8     Q : 1 ;
    uint8   old : 1 ;
} ;

class C_trigger // Change trigger, Q is true if IN changes
{
public:
    C_trigger( ) ;
    uint8_t  trigger( uint8 )  ;
    uint8     Q : 1 ;
    uint8   old : 1 ;
} ;

#endif