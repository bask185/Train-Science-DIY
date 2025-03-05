#include "pwm.h"
#include "src/macros.h"
#include "config.h"

PwmPin pwmPins[16];


void addPin( uint8_t idx )
{
    uint8_t pinNumber = GPIO[idx];

    pwmPins[idx].pinNumber = pinNumber;
    
    if( pinNumber < 8 )  // Pinnen 0-7 horen bij PORTD
    {
        pwmPins[idx].portx_p = &PORTD;
        pwmPins[idx].pin = (1 << pinNumber);
    }
    else if( pinNumber < 14 )  // Pinnen 8-13 horen bij PORTB
    {
        pwmPins[idx].portx_p = &PORTB;
        pwmPins[idx].pin = (1 << (pinNumber - 8));
    }
    else // Analoge pinnen horen bij PORTC (A0-A5)
    {
        pwmPins[idx].portx_p = &PORTC;
        pwmPins[idx].pin = (1 << (pinNumber - A0));
    }
}


void updatePwm()
{
    static uint8_t counter = 0 ;

    REPEAT_MS( 1 )
    {
        if( counter == 0 ) 
        {
            uint8_t portD_state = 0 ;
            uint8_t portB_state = 0 ;
            uint8_t portC_state = 0 ;

            if( pwmPins[ 0].state == 1 ) { portD_state |= pwmPins[ 0].pin ; }
            if( pwmPins[ 1].state == 1 ) { portD_state |= pwmPins[ 1].pin ; }
            if( pwmPins[ 2].state == 1 ) { portD_state |= pwmPins[ 2].pin ; }
            if( pwmPins[ 3].state == 1 ) { portD_state |= pwmPins[ 3].pin ; }
            if( pwmPins[ 4].state == 1 ) { portD_state |= pwmPins[ 4].pin ; }
            if( pwmPins[ 5].state == 1 ) { portB_state |= pwmPins[ 5].pin ; }
            if( pwmPins[ 6].state == 1 ) { portB_state |= pwmPins[ 6].pin ; }
            if( pwmPins[ 7].state == 1 ) { portB_state |= pwmPins[ 7].pin ; }
            if( pwmPins[ 8].state == 1 ) { portD_state |= pwmPins[ 8].pin ; }
            if( pwmPins[ 9].state == 1 ) { portD_state |= pwmPins[ 9].pin ; }
            if( pwmPins[10].state == 1 ) { portC_state |= pwmPins[10].pin ; }
            if( pwmPins[11].state == 1 ) { portC_state |= pwmPins[11].pin ; }
            if( pwmPins[12].state == 1 ) { portC_state |= pwmPins[12].pin ; }
            if( pwmPins[13].state == 1 ) { portC_state |= pwmPins[13].pin ; }
            if( pwmPins[14].state == 1 ) { portC_state |= pwmPins[14].pin ; }
            if( pwmPins[15].state == 1 ) { portC_state |= pwmPins[15].pin ; }

            PORTD = portD_state ;
            PORTB = portB_state ;
            PORTC = portC_state ;
        } 
        else 
        {
            if( counter == pwmPins[ 0].dutycycle ) { *pwmPins[ 0].portx_p &= ~pwmPins[ 0].pin ; }
            if( counter == pwmPins[ 1].dutycycle ) { *pwmPins[ 1].portx_p &= ~pwmPins[ 1].pin ; }
            if( counter == pwmPins[ 2].dutycycle ) { *pwmPins[ 2].portx_p &= ~pwmPins[ 2].pin ; }
            if( counter == pwmPins[ 3].dutycycle ) { *pwmPins[ 3].portx_p &= ~pwmPins[ 3].pin ; }
            if( counter == pwmPins[ 4].dutycycle ) { *pwmPins[ 4].portx_p &= ~pwmPins[ 4].pin ; }
            if( counter == pwmPins[ 5].dutycycle ) { *pwmPins[ 5].portx_p &= ~pwmPins[ 5].pin ; }
            if( counter == pwmPins[ 6].dutycycle ) { *pwmPins[ 6].portx_p &= ~pwmPins[ 6].pin ; }
            if( counter == pwmPins[ 7].dutycycle ) { *pwmPins[ 7].portx_p &= ~pwmPins[ 7].pin ; }
            if( counter == pwmPins[ 8].dutycycle ) { *pwmPins[ 8].portx_p &= ~pwmPins[ 8].pin ; }
            if( counter == pwmPins[ 9].dutycycle ) { *pwmPins[ 9].portx_p &= ~pwmPins[ 9].pin ; }
            if( counter == pwmPins[10].dutycycle ) { *pwmPins[10].portx_p &= ~pwmPins[10].pin ; }
            if( counter == pwmPins[11].dutycycle ) { *pwmPins[11].portx_p &= ~pwmPins[11].pin ; }
            if( counter == pwmPins[12].dutycycle ) { *pwmPins[12].portx_p &= ~pwmPins[12].pin ; }
            if( counter == pwmPins[13].dutycycle ) { *pwmPins[13].portx_p &= ~pwmPins[13].pin ; }
            if( counter == pwmPins[14].dutycycle ) { *pwmPins[14].portx_p &= ~pwmPins[14].pin ; }
            if( counter == pwmPins[15].dutycycle ) { *pwmPins[15].portx_p &= ~pwmPins[15].pin ; }
        }

        if( ++ counter == 10 ) { counter = 0 ; }
    }
    END_REPEAT
}

void begin()
{
    for( uint8_t i = 0 ; i < 16 ; i ++ ) 
    {
        addPin( i );
    }
}
