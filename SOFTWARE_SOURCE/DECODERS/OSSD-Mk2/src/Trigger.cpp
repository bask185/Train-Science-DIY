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

#include "Trigger.h"

R_trigger::R_trigger() // rising trigger
{
}

uint8_t R_trigger::trigger( uint8 IN )
{
    Q = 0 ;

    if( IN != old )
    {
        Q   = IN ;
        old = IN ;
    }

    return Q ;
} 

F_trigger::F_trigger() // falling trigger
{
}

uint8_t F_trigger::trigger( uint8 IN )
{
    Q = 0 ;

    if( IN != old )
    {
        Q   = IN ^ 1 ;
        old = IN ;
    }

    return Q ;
}

C_trigger::C_trigger() // change trigger
{
}

uint8_t C_trigger::trigger( uint8 IN )
{
    Q = 0 ;
    
    if( IN != old )
    {
        Q = 1 ;
        old = IN ;
    }

    return Q ;
}