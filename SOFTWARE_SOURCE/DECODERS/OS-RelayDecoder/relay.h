#include "src/macros.h"
#include <Arduino.h>

class Relay
{
public:
    Relay() ;

    setPin( uint8 ) ;
    setState( uint8 ) ;
    invertPin( uint8 ) ;


private:
    uint8      pin : 8 ;
    uint8 inverted : 1 ;
    uint8    state : 1 ;
} ;