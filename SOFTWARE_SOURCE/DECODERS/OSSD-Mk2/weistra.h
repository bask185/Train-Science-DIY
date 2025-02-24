/*
 * Copyright (C) 2024 Sebastiaan Knippels, Train-Science
 *
 * To the extent possible under law, the person who associated CC0 with this work
 * has waived all copyright and related or neighboring rights to this work.
 *
 * This work is published from: The Netherlands
 *
 * You can copy, modify, distribute and perform the work, even for commercial purposes,
 * all without asking permission. See the full license for details at:
 * https://creativecommons.org/publicdomain/zero/1.0/
 * 
 * modified for OSSD MK2 code
 */

#include <Arduino.h>

class Weistra {
public:
    Weistra();
    void begin(uint8_t, uint8_t, uint8_t);
    void update();
    void setSpeed(int8_t);
    uint8_t getSpeed() ;
    void setState(uint8_t ) ;

private:
    uint32_t intervalTime ;
    uint16_t newIntervalTime ;
    uint32_t prevTime ;
    uint8_t trackPin1 ;
    uint8_t dutyCycle ;
    uint8_t newDutyCycle ;
    uint8_t Fmin ;
    uint8_t Fmax ;
    uint8_t state ;
    uint8_t counter = 0;

    volatile uint8_t *portx_p1 ;
};

