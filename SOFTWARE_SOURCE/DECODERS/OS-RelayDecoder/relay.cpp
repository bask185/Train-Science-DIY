#include "relay.h"

Relay::Relay() 
{
}

Relay::setPin( uint8 _pin ) 
{
    pin = _pin ;
}

Relay::setState( uint8 _state ) 
{
    state = _state ^ inverted ;
    digitalWrite( pin, state ) ;
}

Relay::invertPin( uint8 _inverted ) 
{
    inverted = _inverted ;
}
