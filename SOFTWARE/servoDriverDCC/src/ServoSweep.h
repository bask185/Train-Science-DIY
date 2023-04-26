#include <Arduino.h>
#include "Servo8Bit.h"

class ServoSweep
{
public:
    ServoSweep() ;
    void sweep() ;
    void setState( uint8_t  );
    uint8_t setState();
    void setMin( uint8_t ) ;
    void setMax( uint8_t ) ;
    void begin( uint8_t, uint8_t, uint8_t, uint8_t ) ;


private:
    Servo8Bit servo ;
    uint32_t previousTime ;
    uint8_t pos ;
    uint8_t state ;
    uint8_t prevPos;
    uint8_t servoPin ;
    uint8_t servoSpeed = 100 ;
    uint8_t servoMin ;
    uint8_t servoMax  ;
} ;
