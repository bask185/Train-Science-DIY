
#include <Arduino.h>

const int currentLimit       =  10 ; // CHECK ME< ADD CALCULATION
const int speedPin           =  A0 ;
const int currentSensePin    =  A1 ;
const int rs485dir           =   2 ; // Xnet control pin
const int in1                =  10 ; // h brug
const int in2                =  11 ; // h brug
const int Fmin               =  50 ;
const int Fmax               = 100 ;
const int accelerationFactor =  50 ; // smaller -> faster acceleration, bigger -> slower acceleration
const int maxSpeed           = 100 ; // may NOT be bigger than 100

// #define HAS_XPRESSNET
