#include <Arduino.h>

class Fader
{
public:
    Fader( uint8_t ) ;
    void    begin() ;
    void    setState( uint8_t ) ;
    void    update() ;
    uint8_t getState() ;

private:
    uint8_t     pin ;
    uint8_t     state ;
    uint8_t     pwm ;
    uint8_t     prevTime ;
} ;