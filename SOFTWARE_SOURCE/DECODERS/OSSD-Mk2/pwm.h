#ifndef PWM_H
#define PWM_H

#include <Arduino.h>

// Structure to hold PWM pin information
struct PwmPin {
    uint8_t pinNumber;      // The physical pin number
    volatile uint8_t *portx_p; // Pointer to the corresponding PORT register
    uint8_t pin;           // Bitmask for the pin
    uint8_t state;         // Current state (0 = OFF, 1 = ON)
    uint8_t dutycycle;     // Duty cycle percentage (0-100)
};

extern PwmPin pwmPins[16]; // Array of 16 PWM pins

void addPin(uint8_t idx);
uint8_t getPwmIndex(uint8_t pinNumber);
void updatePwm();

#endif // PWM_H
