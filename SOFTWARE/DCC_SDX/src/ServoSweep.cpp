#include "ServoSweep.h"

ServoSweep::ServoSweep( )
{
}
void ServoSweep::begin( uint8_t _servoPin, uint8_t _min, uint8_t _max, uint8_t _speed, uint8_t _relayPin )
{
    servoPin = _servoPin ;
    servoSpeed = _speed ;
    servoMin = _min ;
    servoMax = _max ;

    middlePosition = ( (long)servoMax - (long)servoMin ) / (long)2 + (long)servoMin ;

    pos = middlePosition ;


    if( servoPin != 255)
    {
        servo.write( pos ) ;
        servo.attach( servoPin ) ;
    }

    relayPin = _relayPin ;
    if( relayPin != 255 )
    {
        pinMode( relayPin, OUTPUT ) ;
    }
}

void ServoSweep::setMin( uint8_t newPos )
{
    servoMin = newPos ;
}
void ServoSweep::setMax( uint8_t newPos )
{
    servoMax = newPos ;
}

void ServoSweep::setState( uint8_t _state )
{
    state = _state ;
}


uint8_t ServoSweep::update ( ) {
    if( millis() > timeToRun ) {
        timeToRun = millis() + servoSpeed ;

        if( state ) {
            if( pos < servoMax ) pos ++ ;
        }
        else {
            if( pos > servoMin ) pos -- ;
        }

        if( prevPos != pos ) {
            prevPos  = pos ;

            if( pos == servoMax || pos == servoMin ) servo.detach( ) ;          // position reached, kill servo
            else if( !servo.attached() )             servo.attach( servoPin ) ; // position changed and servo is off -> turn on servo

            if( relayPresent ) {
                if( pos < middlePosition ) digitalWrite( relayPin,  LOW ) ;
                else                       digitalWrite( relayPin, HIGH ) ;
            }
            if( servoPin != 255 ) servo.write( pos ) ;
            return pos ;
        }
        else
        {
            return 0 ;
        }
    }
}