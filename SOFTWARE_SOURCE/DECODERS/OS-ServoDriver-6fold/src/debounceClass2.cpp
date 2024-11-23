#include "debounceClass2.h"

Debouncer::Debouncer() 
{
    pin             = 255 ;
    interval        = 0 ;
    flank           = 0 ;
    hasRisenFlag    = 1 ;
    hasFallenFlag   = 1 ;
    oldSample       = 1 ;
    statePrev       = 1 ;
    hasLongPressed  = true ;
    hasShortPressed = true ;
}

Debouncer& Debouncer::setFlank( uint8 _flank ) 
{
    flank = _flank ;
    return *this ;
}

Debouncer& Debouncer::setPin( uint8 _pin ) 
{
    pin = _pin ;
    return *this ;
}

Debouncer& Debouncer::setDebounceTime( uint32 _interval ) 
{
    interval = _interval ;
    return *this ;
}

uint8 Debouncer::debounce()
{
    return debounce( digitalRead( pin ) ) ;
}

uint8 Debouncer::debounce( uint8 newSample )
{
    if( interval == 0 || millis() - prevTime >= interval ) 
    {    
        if( interval ) prevTime = millis() ;

        if( newSample == oldSample )   
        {
            if( newSample != statePrev ) 
            {
                statePrev = newSample ;

                if( newSample ) { newState =  RISING ;  hasRisenFlag = 1 ; } 
                else            { newState = FALLING ; hasFallenFlag = 1 ; }
            }
            else            
            {
                if( newSample ) newState = HIGH ; 
                else            newState =  LOW ;
            }
        }

        oldSample = newSample ;
    }

    if( state != newState )
    {
        if( onStateChange ) onStateChange( this, newState ) ;
    }

    state = newState ;

    Q = ( flank ==  RISING && state ==  RISING )
     || ( flank == FALLING && state == FALLING )
     || ( flank ==    BOTH && (state == FALLING || state == RISING) ) ;

    if( newState ==  RISING ) newState = HIGH ;
    if( newState == FALLING ) newState =  LOW ;

    return state ;
}

uint8 Debouncer::getState() { return state ; }

uint8 Debouncer::pressTime( uint32 limit, uint8 useInternal )
{
    if( useInternal )
    {
        debounce(); // Roep debounce aan als gebruik van interne input
    }
    
    if( state == FALLING ) {
        pressStartTime  = millis() ;
        hasLongPressed  = false ;
        hasShortPressed = false ;
    }

    if( state == RISING && !hasShortPressed ) {
        hasShortPressed = true ;
        
        uint32 pressDuration = millis() - pressStartTime ; 

        if( pressDuration < limit )
        {
            if( onShortPress ) onShortPress( this );
            return SHORT ; 
        }
    }

    if( state == LOW && !hasLongPressed && (millis() - pressStartTime >= limit) )
    {
        if( onLongPress ) onLongPress( this );
        hasLongPressed = true ;
        return LONG ;
    }

    return 0;
}

uint8 Debouncer::hasRisen()
{
    if( hasRisenFlag == 0 ) return 0 ;

    hasRisenFlag = 0 ;
    return 1 ;
}

uint8 Debouncer::hasFallen()
{
    if( hasFallenFlag == 0 ) return 0 ;

    hasFallenFlag = 0 ;
    return 1 ;
}
