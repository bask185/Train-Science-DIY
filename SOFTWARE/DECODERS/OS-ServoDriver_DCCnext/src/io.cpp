#include <Arduino.h>
#include "io.h"
extern void initIO(void) {

	pinMode(switchPin0, INPUT_PULLUP);
	pinMode(switchPin1, INPUT_PULLUP);
	pinMode(switchPin2, INPUT_PULLUP);
	pinMode(switchPin3, INPUT_PULLUP);
	pinMode(ledPin, OUTPUT);
}