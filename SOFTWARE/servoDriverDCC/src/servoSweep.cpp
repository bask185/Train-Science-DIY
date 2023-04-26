#include "ServoSweep.h"


ServoSweep::ServoSweep()
{
}

void ServoSweep::setMin( uint8_t newPos )
{
    servoMin = newPos ;
}

void ServoSweep::setMax( uint8_t newPos )
{
    servoMax = newPos ;
}

void ServoSweep::begin( uint8_t _servoPin, uint8_t _min, uint8_t _max, uint8_t beginState )
{
    servoMin = _min ;
    servoMax = _max ;

    if( beginState ) pos = servoMax ;
    else             pos = servoMin ;
}

void ServoSweep::setState( uint8_t _state )
{
    state = _state ;
}

uint8_t ServoSweep::getState()
{
    return state ;
}


void ServoSweep::sweep ( )
{
    if( millis() - previousTime >= servoSpeed )
    {        previousTime = millis() ;

        if( state ) {
            if( pos < servoMax ) pos ++ ;
            if( pos > servoMax ) pos -- ;
        }
        else {
            if( pos > servoMin ) pos -- ;
            if( pos < servoMin ) pos ++ ;
        }

        if( prevPos != pos ) {
            prevPos  = pos ;

            if( pos == servoMax || pos == servoMin ) servo.detach() ;
            else                                     servo.attach( servoPin ) ;

            servo.write( pos ) ;
        }
    }
}