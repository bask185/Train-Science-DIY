#include "CoilDrive.h"

CoilDrive::CoilDrive( )
{
}

void CoilDrive::setState( uint8 _state )
{
    state = _state ;
}

uint8_t CoilDrive::update()
{
    if( state != prevState ) // state has changed, turn on a coil
    {  prevState = state ;

        prevTime = millis() ;
        set = true ;

        if( state ) digitalWrite( pinA, HIGH ) ;
        else        digitalWrite( pinB, HIGH ) ;
    }

    if( set == true && (millis() - prevTime) >= 100 )
    {   set  = false ;
        
        digitalWrite( pinA, LOW ) ;
        digitalWrite( pinB, LOW ) ;
    }

    return set ;
}