#include <Arduino.h>
#include "io.h"
extern void initIO(void) {
	pinMode(detector1, INPUT_PULLUP);
	pinMode(detector2, INPUT_PULLUP);
	pinMode(detector3, INPUT_PULLUP);
	pinMode(detector4, INPUT_PULLUP);
	pinMode(servoPin1, OUTPUT);
	pinMode(servoPin2, OUTPUT);
	pinMode(led1, OUTPUT);
	pinMode(blinkLed1, OUTPUT);
	pinMode(blinkLed2, OUTPUT);
}