#include <Arduino.h>
#include "macros.h"

class CoilDrive {

public:
    CoilDrive() ;
    void    setState( uint8_t ) ;
    void    begin( uint8_t, uint8_t, uint32 ) ;
    uint8_t update() ;

private:
    uint32  switchTime ;
    uint32  prevTime ;
    uint8   pinA ;
    uint8   pinB ;
    uint8   prevState : 1 ;
    uint8   state : 1 ;
    uint8   set : 1 ;
} ;