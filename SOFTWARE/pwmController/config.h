
#include <Arduino.h>

// maximum current
const int currentLimit       =  10 ; // CHECK ME ADD CALCULATION

// used IO PINs
const int speedPin           =  A0 ;
const int maxSpeedPin        =  A1 ;
const int accelFactorPin     =  A2 ;
const int waitTimePin        =  A3 ;
const int currentSensePin    =  A4 ;
const int in1                =  10 ; // h brug
const int in2                =  11 ; // h brug
const int S1                 =   3 ;
const int S2                 =   4 ;

// PWM parameters
const int Fmin               =  50 ;
const int Fmax               = 100 ;
      int accelerationFactor =  50 ;
      int maxSpeed           = 100 ; // may NOT be bigger than 100

// shuttle service station wait time
uint32_t    waitTime = 15000 ; // 15 seconds
