#include <Arduino.h>
#include "macros.h"

class DCC
{
public:
    DCC() ;
    void    begin() ;
    void    setAccessory(      uint16, uint8 ) ;
    void    setAccessoryExt(   uint16, uint8 ) ;
    void    generatePackets() ;
    
    static inline void timerISR() ;    

private:
    static  DCC *dccObject ;

    void    setAccessoryExt( uint16, uint8, uint8 ) ;
    void    setRepeatAmount() ;

    uint8   selectNewPacketTypeF() ;
    uint8   assemblePacketF() ;
    uint8   awaitPacketSentF() ;
} ;

// call back functions
extern void dccISR()    __attribute__((weak)) ; // use this to set the actual IO

// debug stuff
extern void oneBit()    __attribute__((weak)) ;
extern void zeroBit()   __attribute__((weak)) ;
extern void zeroBit2()  __attribute__((weak)) ;
extern void endBit()    __attribute__((weak)) ;

extern bool ACCES ;
