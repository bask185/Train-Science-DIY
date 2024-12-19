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

const int GPIO[] =
{
     4,  3,  5,  6, // GPIO01-GPIO04
     7, 10, 11, 12, // GPIO05-GPIO08
     1,  0, A5, A4, // GPIO09-GPIO12
    A3, A2, A1, A0, // GPIO13-GPIO16
} ;


const int nGpio = 16 ;
const int configPin = 8 ;
const int ledPin = 13 ;

