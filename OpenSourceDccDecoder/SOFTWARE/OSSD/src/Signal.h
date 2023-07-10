#include <Arduino.h>
#include "macros.h"

class Signal {

public:
    Signal() ;
    void    setState( uint8_t ) ;
    void    setMode( uint8_t ) ;
    uint8_t update() ;

private:
    uint8   pinA ;
    uint8   pinB ;
    uint8   pinC ;
    uint8   pinD ;
    uint8   state  ;
    uint8   aspect  ;
} ;