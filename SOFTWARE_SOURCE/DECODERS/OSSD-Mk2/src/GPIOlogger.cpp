#include "GPIOlogger.h"

GPIOlogger::GPIOlogger() {}

GPIOlogger& GPIOlogger::addGPIO( uint8_t newPin )
{
    C_trigger* newTriggers = new C_trigger[nGpio + 1];
    uint8_t* newPins = new uint8_t[nGpio + 1];

    for ( uint8_t i = 0 ; i < nGpio ; i++ )
    {
        newTriggers[i] = triggers[i];
        newPins[i] = pins[i];
    }

    newTriggers[nGpio] = C_trigger(); 
    newPins[nGpio] = newPin;

    delete[] triggers ;
    delete[] pins ;

    triggers = newTriggers ;
    pins = newPins ;

    nGpio ++ ;

    return *this ;
}

void GPIOlogger::log()
{
    for( int i = 0 ; i < nGpio;  i ++ )
    {
        uint8_t state = digitalRead(pins[i]) ;

        if(  triggers[i].trigger( state ) )
        {
            if( notifyLog )
                notifyLog( pins[i], state ) ;
        }
    }
}