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

#ifndef button_h
#define	button_h

// //#define 
// #define ON 9 // random numbers, RISING and FALLING are already defined in Arduino.h
// #define OFF 10 // REPLACED FOR HIGH AND LOW

class Debounce {
public:
	Debounce() ;
	unsigned char getState() ;
	void debounceInputs() ;
	void begin( uint8_t ) ;

private:
	unsigned char state ;
	unsigned char pin ; 
	bool oldSample = false ;
	bool statePrev = false ;
} ;
	
#endif