#include "src/macros.h"
#include <Arduino.h>

extern uint8 mode ;


const int maxRegister = 12 ; // can change...
extern uint8   nRegister ;

extern void teachin() ;
extern void sendButton( uint8, uint8 ) ;

extern void notifyRoutePointSet( uint16, uint8 ) __attribute__((weak)) ;