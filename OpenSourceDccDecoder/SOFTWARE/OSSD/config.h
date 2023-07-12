#include <Arduino.h>

const int configButton = 2 ;
const int ledPin = 13 ;
const int nGPIO = 16 ;

const int GPIO[] =
{
     7,  6,  5, 10,
    11, 12,  4,  3,
     1,  0, A5, A4,
    A3, A3, A2, A0,
} ;

const int   MODE_ADDRESS = 0x0000 ;