#include <Arduino.h>
#include "io.h"
extern void initIO(void) {
	pinMode(servoPin1, OUTPUT);
	pinMode(servoPin2, OUTPUT);
	pinMode(servoPin3, OUTPUT);
	pinMode(servoPin4, OUTPUT);
	pinMode(servoPin5, OUTPUT);
	pinMode(servoPin6, OUTPUT);
	pinMode(servoPin7, OUTPUT);
	pinMode(servoPin8, OUTPUT);
	pinMode(switchPin0, INPUT_PULLUP);
	pinMode(switchPin1, INPUT_PULLUP);
	pinMode(switchPin2, INPUT_PULLUP);
	pinMode(switchPin3, INPUT_PULLUP);
	pinMode(switchPin4, INPUT_PULLUP);
	pinMode(ledPin, OUTPUT);
}