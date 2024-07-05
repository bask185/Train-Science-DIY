#include <Arduino.h>
#include "io.h"
extern void initIO(void) {
	pinMode(redPin, OUTPUT);
	pinMode(yellowPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(mainServoPin, OUTPUT);
	pinMode(distantServoPin, OUTPUT);
	pinMode(bufferSensorPin, INPUT_PULLUP);
	pinMode(stopSensorPin, INPUT_PULLUP);
	pinMode(uplinePin, INPUT_PULLUP);
	pinMode(downlinePin, INPUT_PULLUP);
	pinMode(ABC, OUTPUT);
	pinMode(toOccupancy, OUTPUT);
	pinMode(ledPin, OUTPUT);
}