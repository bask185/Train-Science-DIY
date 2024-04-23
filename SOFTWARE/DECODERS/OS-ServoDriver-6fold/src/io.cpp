#include <Arduino.h>
#include "io.h"
extern void initIO(void) {
	pinMode(servoPin1, OUTPUT);
	pinMode(servoPin2, OUTPUT);
	pinMode(servoPin3, OUTPUT);
	pinMode(servoPin4, OUTPUT);
	pinMode(servoPin5, OUTPUT);
	pinMode(servoPin6, OUTPUT);

	pinMode(relayPin1, OUTPUT);
	pinMode(relayPin2, OUTPUT);
	pinMode(relayPin3, OUTPUT);
	pinMode(relayPin4, OUTPUT);
	pinMode(relayPin5, OUTPUT);
	pinMode(relayPin6, OUTPUT);

	pinMode(switchPin0, INPUT_PULLUP);
	pinMode(switchPin1, INPUT_PULLUP);
	pinMode(switchPin2, INPUT_PULLUP);
	pinMode(switchPin3, INPUT_PULLUP);
    
	pinMode(ledPin, OUTPUT);
}