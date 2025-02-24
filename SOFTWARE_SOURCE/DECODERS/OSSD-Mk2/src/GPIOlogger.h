#include <Arduino.h>
#include "Trigger.h"


class GPIOlogger
{
public:
    GPIOlogger() ;
    GPIOlogger& addGPIO( uint8 ) ; 
    void log() ;

private:
    uint8_t     nGpio = 0 ;
    C_trigger*  triggers = nullptr ;
    uint8_t*    pins     = nullptr ; 
} ;

extern void notifyLog( uint8_t pinNumber, uint8_t state ) __attribute__((weak)) ;