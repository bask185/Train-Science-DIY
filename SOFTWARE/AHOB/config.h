#include <Arduino.h>

const int armSpeed         = 5000 ;                    // speed of arms opening/closing bases on 90 degrees
const int closePos         =   90 ;                    // degrees of close positions of servos
const int openPos          =    0 ;                    // degrees of open positions of servos
// 45 blinks/minute -> 0.75Hz -> blink time = 1333ms   // SK: may need to be halved
// 90 blinks/minute -> 1.50Hz -? blink time =  667ms
const int blinkTime        = 1333 ;                    // interval of blinking leds
const int fadeTime         =  500 ;                    // Total fade time = `fadeTime` * 256uS
const int debounceInterval =  100 ;                    // interval of sampling sensors

#define IS_AHOB
// #define IS_AKI

const int dccPin  =  2 ;
// Available PWM pins
// 3, 5, 6, 11

const int blinkLed1 =  3 ;
const int blinkLed2 =  5 ;
const int blinkLed3 =  6 ;
const int servoPin1 =  7 ; 
const int servoPin2 =  8 ;
const int servoPin3 =  9 ;
const int servoPin4 = 10 ;

const int detector1 = A0 ;
const int detector2 = A1 ;
const int detector3 = A2 ;
const int detector4 = A3 ;
const int detector5 = A4 ;
const int detector6 = A5 ;
