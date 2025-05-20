#include <Arduino.h>

const int GPIO[] =
{
     9,  3,  5,  6, // GPIO01-GPIO04
     7, 10, 11, 12, // GPIO05-GPIO08
     1,  0, A5, A4, // 1,0, A5, A4 GPIO09-GPIO12
    A3, A2, A1, A0, // GPIO13-GPIO16
} ;

const int DEF_SINGLE_PULSE_TIME     = 5000 ;
const int DEF_DOUBLE_PULSE_TIME     =   50 ;
const int DEF_DUTYCYCLE             =   10 ;
const int LOCKOUT_TIME              =  500 ;
const int nDccAddresses             = 2048 ;


// const int GPIO[] = // BAD ARRAY
// {
//     9,  2,  5,  6, // GPIO01-GPIO04
//      7, 10, 11, 12, // GPIO05-GPIO08
//     13, 4, A5, A4, // 1,0, A5, A4 GPIO09-GPIO12
//     A3, A2, A1, A0, // GPIO13-GPIO16
// } ;

const int nGpio         = 16 ;
const int configPin     =  8 ;
const int rightLedPin   =  4 ; 
const int leftLedPin    = 13 ;

const int currentSensePin = A6 ;
const int coilVoltagePin  = A7 ;

