#ifndef PWM_CONTROLLER_H
#define PWM_CONTROLLER_H

#include <Arduino.h>
#include "src/macros.h"

#define MAX_PWM_PINS 16  // Maximum number of software PWM pins

const int INACTIVE = 255 ;
struct PwmPin
{
    volatile uint8_t    *portReg ;          // Pointer to port register
    uint8_t             bitmask ;           // Bitmask for pin
    uint8_t             dutyCycle ;         // Duty cycle (0-10)
    uint8_t             pin = INACTIVE ;    // Pin number (255 = inactive)
} ;


void updatePwm();  
void subscribePwm(   uint8_t digitalPin, uint8_t dutyCycle ) ;
void unsubscribePwm( uint8_t digitalPin ) ;

#endif // PWM_CONTROLLER_H
