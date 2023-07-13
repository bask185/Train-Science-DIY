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
        set = false ;

        if( state ) digitalWrite( pinA, HIGH ) ;
        else        digitalWrite( pinB, HIGH ) ;
    }

    if( set == false && (millis() - prevTime) >= 100 ) // if time has expired, kill coils and clear set flag
    {   set  = true ;
        
        digitalWrite( pinA, LOW ) ;
        digitalWrite( pinB, LOW ) ;
    }

    return set ;
}