#include "pwm.h"

// PWM pin array (fixed size, dynamic management)
PwmPin pwmPins[MAX_PWM_PINS];

uint8_t activePins = 0;  // Tracks active pins

// Function to update software PWM every 1ms
void updatePwm()
{
    static uint8_t counter = 0 ;

    REPEAT_MS( 1 )
    {
        if( ++counter == 10 ) counter = 0 ;

        for( uint8_t i = 0; i < activePins; i++ )
        {
            if( pwmPins[i].pin == INACTIVE ) continue;  // Skip inactive slots

            if( counter == 0 )
            {
                // Turn all active pins ON
                *pwmPins[i].portReg |= pwmPins[i].bitmask;
            }
            else if( counter == pwmPins[i].dutyCycle )
            {
                // Turn off pins when counter matches duty cycle
                *pwmPins[i].portReg &= ~pwmPins[i].bitmask ;
            }
        }
    }
    END_REPEAT
}

// Function to subscribe a pin to software PWM
void subscribePwm(uint8_t digitalPin, uint8_t dutyCycle)
{
    if (dutyCycle > 10) return; // Prevent invalid duty cycles

    uint8_t             port        = digitalPinToPort( digitalPin );
    uint8_t             bitmask     = digitalPinToBitMask( digitalPin );
    volatile uint8_t    *portReg    = portOutputRegister( port );

    // Check if the pin is already in the array
    for( uint8_t i = 0; i < MAX_PWM_PINS; i++ )
    {
        if( pwmPins[i].pin == digitalPin )  
        {
            pwmPins[i].dutyCycle = dutyCycle;
            return;  // Pin found, update values and return
        }
    }

    // If the pin is not found, add a new entry
    for (uint8_t i = 0; i < MAX_PWM_PINS; i++)
    {
        if (pwmPins[i].pin != INACTIVE) continue; // Skip used slots

        pwmPins[i].portReg      = portReg;
        pwmPins[i].bitmask      = bitmask;
        pwmPins[i].dutyCycle    = dutyCycle;
        pwmPins[i].pin          = digitalPin;
        return;
    }
}



// Function to unsubscribe a pin
void unsubscribePwm(uint8_t digitalPin)
{
    for( uint8_t i = 0; i < MAX_PWM_PINS; i++ )
    {
        if( pwmPins[i].pin == digitalPin )
        {
            pwmPins[i].pin = INACTIVE; 
            return;
        }
    }
}