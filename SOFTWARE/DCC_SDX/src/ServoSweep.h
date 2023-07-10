#include <Arduino.h>
#include "ServoTimer2.h"

class ServoSweep {

public:
    ServoSweep() ;
    void setState( uint8_t _state ) ;
    uint8_t update( );
    // void turnOn() ;
    // void turnOff() ;
    void begin( uint8_t _servoPin, uint8_t _min, uint8_t _max, uint8_t _speed, uint8_t _relayPin );
    void setMin( uint8_t ) ;
    void setMax( uint8_t ) ;

private:
    ServoTimer2 servo ;
    unsigned long timeToRun ;
    uint8_t pos ;
    uint8_t state ;
    uint8_t prevPos;
    uint8_t servoPin ;
    uint8_t servoSpeed ;
    uint8_t servoMin ;
    uint8_t servoMax  ;
    uint8_t middlePosition ;
    uint8_t relayPresent ;
    uint8_t relayPin ;
    uint8_t turnOff ;
    unsigned long currentTime = millis() ;
} ;
