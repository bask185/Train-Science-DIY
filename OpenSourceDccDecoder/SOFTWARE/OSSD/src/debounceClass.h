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