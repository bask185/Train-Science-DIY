#include <Arduino.h>

const int GPIO[] =
{
     4,  3,  5,  6, // GPIO01-GPIO04
     7, 10, 11, 12, // GPIO05-GPIO08
     1,  0, A5, A4, // GPIO09-GPIO12
    A3, A2, A1, A0, // GPIO13-GPIO16
} ;

const int nGpio         = 16 ;
const int configPin     =  8 ;
const int blueLedPin    =  9 ;
const int yellowLedPin  = 13 ;

const int currentSensePin = A6 ;
const int coilVoltagePin  = A7 ;

