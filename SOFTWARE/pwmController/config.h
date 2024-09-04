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

// maximum current
const int currentLimit       =  25 ; // roughly 2.5A

// used IO PINs
const int speedPin           =  A0 ;
const int currentSensePin    =  A5 ;
const int minSpeedPin        =  A1 ;
const int maxSpeedPin        =  A2 ;
const int accelFactorPin     =  A3 ;
const int waitTimePin        =  A4 ;
const int in1                =   6 ; // h brug
const int in2                =   7 ; // h brug
const int S1                 =   2 ;
const int S2                 =   3 ;
const int ledRed             =   8 ;
const int ledGreen           =   9 ;

// PWM parameters
const int Fmin               =  50 ;
const int Fmax               = 100 ;
      int accelerationFactor =  15 ;
      int minSpeed           =  10 ; // minimum speed. Slow trains start driving at higher throttle levers
      int maxSpeed           = 100 ; // may NOT be bigger than 100

// shuttle service station wait time
uint32_t    waitTime = 30000 ; // 30 seconds
