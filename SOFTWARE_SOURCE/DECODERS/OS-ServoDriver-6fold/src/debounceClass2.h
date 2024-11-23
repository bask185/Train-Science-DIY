#ifndef DEBOUNCECLASS2_H
#define DEBOUNCECLASS2_H

#include <Arduino.h>
#include "macros.h"


const int BOTH  = 1; 
const int SHORT = 1; 
const int LONG  = 2; 

class Debouncer
{
public:
    Debouncer();

    void (*onShortPress)( Debouncer* ) = nullptr;
    void (*onLongPress )( Debouncer* ) = nullptr;
    void (*onStateChange)( Debouncer*, uint8 ) = nullptr;

    // Set...() methods for method chaining
    Debouncer& setFlank(        uint8     _flank ) ;
    Debouncer& setPin(          uint8       _pin ) ;
    Debouncer& setDebounceTime( uint32 _interval ) ;

    uint8 pressTime( uint32 limit, uint8 useInternal = false );
    uint8 hasRisen();
    uint8 hasFallen();
    uint8 debounce();
    uint8 debounce( uint8 newSample );
    uint8 getState();

    uint16 state           : 2 ;
    uint16 oldSample       : 1 ;
    uint16 pin             : 6 ;
    uint16 statePrev       : 1 ;
    uint16 newState        : 2 ;
    uint16 flank           : 2 ;
    uint16 Q               : 1 ;
    uint16 hasLongPressed  : 1 ;
    uint16 hasShortPressed : 1 ;
    uint16 hasRisenFlag    : 1 ;
    uint16 hasFallenFlag   : 1 ;

private:
    uint32 prevTime ;
    uint32 interval ;
    uint32 pressStartTime ;
};

#endif // DEBOUNCECLASS2_H
