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
    uint8   getHisOccupancy() ; 

//private:
    uint8   pin ;
    uint8   myOccupancy_A ;  
    uint8   myOccupancy_B ;  
    uint8   hisOccupancy : 1 ;
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