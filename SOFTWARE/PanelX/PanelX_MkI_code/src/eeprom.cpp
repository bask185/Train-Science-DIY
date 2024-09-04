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

#include <EEPROM.h>
#include "eeprom.h"
#include "macros.h"

void storePoint( uint8_t pin, uint16_t address )
{
    uint16_t eeAddress = pin * 2 ;

    EEPROM.put( eeAddress, address ) ;
}

uint16_t  loadPoint( uint8_t pin )
{
    uint16_t address ;
    uint16_t eeAddress = pin * 2 ;

    EEPROM.get( eeAddress, address ) ;
    
    return address ;
}