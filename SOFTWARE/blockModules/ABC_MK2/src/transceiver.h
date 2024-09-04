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

#include "macros.h"
#include <Arduino.h>

const int        ABC_MODULE =  0 ;
const int       EXIT_MODULE =  1 ;

class Transceiver
{
public:
    void    begin() ;

    void    setMyOccupancy_A( uint8 ) ; 
    void    setMyOccupancy_B( uint8 ) ; 
    void    setHisOccupancy( uint8 ) ; 
    uint8   getMyOccupancy() ; 
    uint8   getHisOccupancy_A() ; 
    uint8   getHisOccupancy_B() ; 

//private:
    uint8   pin ;
    uint8   myOccupancy_A  : 1 ;  
    uint8   myOccupancy_B  : 1 ;  
    uint8   hisOccupancy_A : 1 ;
    uint8   hisOccupancy_B : 1 ;
    uint32  lastPulse ;
    uint32  pulseLength ;
} ;

class Sender : public Transceiver
{
public:
    Sender( uint8, uint8 ) ;
    
    void    transmitt() ;
    void    setDeparture( uint8 ) ;

private:
    enum
    {
        startPulse,
        waitPulseSent,
        waitResponse,
        waitNewCycle,
    }
    state = startPulse ;

    uint8   type ;
    uint8   departure ;
} ;

class Receiver  : public Transceiver
{
public:
    Receiver( uint8 ) ; // pin

    void    receive() ;

private:
    enum
    {
        waitPulse,
        getPulseLength,
        sendResponse,
        responseSent,
    } 
    state = waitPulse ;
} ;