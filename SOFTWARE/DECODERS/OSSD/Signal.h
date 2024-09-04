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

uint8 getAspectAmount() ;
uint8 getOutputAmount() ;

class Signal {

public:
    Signal() ;
    void    begin( uint8, uint8, uint8 ) ; // used during boot
    uint8   update() ;

    uint8   getLedCount() ;
    void    setType( uint8 ) ;
    void    setFirstIO( uint8 ) ;
    void    setAddress( uint16 ) ;
    uint8   setAspectExt( uint16, uint8 ) ;
    uint8   setAspect(    uint16, uint8 ) ;
    uint8   getAspectAmount() ;
    uint8   get1stPin() ;
    uint16  getAddress() ;
    uint16  getAddressAmount() ;
    uint8   getType() ;
    void    saveAspect() ;
    void    loadAspect() ;

private:
    uint8   beginPin ;
    uint8   currentAspect ;
    uint16  eeAddress ;
    uint8   nAspects ;
    uint8   nAddresses ;
    uint8   type ;
    uint8   ledCount ;
    uint16  myAddress ;
    uint32  prevTime ;
    uint8   tglState ;
    uint8   sm ; // state machine

    const int interval = 1000 ; // may need to be variable in struct..

    uint8    updateCoils() ;
} ;