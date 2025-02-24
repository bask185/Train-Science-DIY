#include "Timers.h"

#include <Arduino.h>

BaseTimer::BaseTimer()
{
    startTrigger = 1 ;
    endTrigger   = 1 ;
}

BaseTimer::BaseTimer( uint32_t time )
{
    startTrigger = 1 ;
    endTrigger   = 1 ;
    presetTime = time ;
}

void BaseTimer::setTime( uint32_t time )
{
    presetTime = time ;
}

uint8_t T_ON::update(uint8_t IN) 
{
    Q = 0 ;
    if( !IN )
    {
        startTrigger = 1 ;
        endTrigger   = 1 ;
    }
    else if( startTrigger )
    {
        startTrigger = 0 ;
        startTime = millis() ;
    }

    if( IN 
    &&  millis() - startTime >= presetTime 
    &&  endTrigger == 1  )
    {
        endTrigger = 0 ;
        Q = 1 ;
    }
    return Q ;
}

uint8_t T_OFF::update(uint8_t IN) 
{
    Q = 0 ;
    if( IN )
    {
        startTrigger = 1 ;
        endTrigger   = 1 ;
    }
    else if( startTrigger )
    {
        startTrigger = 0 ;
        startTime = millis() ;
    }

    if( !IN
    &&  millis() - startTime >= presetTime 
    &&  endTrigger == 1 )
    {
        endTrigger = 0 ;
        Q = 1 ;
    }
    return Q ;
}

BlinkTimer::BlinkTimer(uint32_t time) : BaseTimer()
{
    presetTime = time;
}

uint8_t BlinkTimer::update(uint8_t IN) 
{
    if( !IN )
    {
        startTime = millis() ;
    }

    if( IN && (millis() - startTime) >= presetTime )
    {
        startTime = millis() ;
        Q = !Q ;
    }
    else
    {
        Q = 0 ;
    }
    return Q ;
}
