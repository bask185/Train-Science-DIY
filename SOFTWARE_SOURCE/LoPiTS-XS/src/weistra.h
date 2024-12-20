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
 */

#include <Arduino.h>

class Weistra {
public:
    Weistra(uint8_t, uint8_t, uint8_t);
    Weistra(uint8_t, uint8_t, uint8_t, uint8_t);
    void    begin();
    void    update();
    void    setSpeed(int8_t);
    void    setState(uint8_t ) ;
    void    setCurrentSense( uint8_t, uint8_t ) ;
    uint8_t getState( ) ;
    uint8_t kickStart( uint32_t ) ;
    void    setMin( uint8_t ) ;
    void    setMax( uint8_t ) ;

private:
    uint32_t intervalTime ;
    uint16_t newIntervalTime ;
    uint16_t interval ;
    uint32_t prevTime ;
    uint8_t trackPin1 ;
    uint8_t trackPin2 ;
    uint8_t dutyCycle ;
    uint8_t newDutyCycle ;
    uint8_t Fmin ;
    uint8_t Fmax ;
    uint8_t minSpeed =   0 ;
    uint8_t maxSpeed = 100 ;
    uint8_t state ;
    uint8_t dir ;
    uint8_t currentLimit ;
    uint8_t sensePin ;
    uint8_t counter = 0;
    uint8_t currentCounter = 0;
    uint16_t kickStartTime ;
    uint32_t prevTime2 = 0;

    const int sampleRate = 10 ;

    volatile uint8_t *portx_p1 ;
    volatile uint8_t *portx_p2 ;
};

