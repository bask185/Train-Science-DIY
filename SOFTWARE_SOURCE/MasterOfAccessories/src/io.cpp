#include <Arduino.h>
#include "io.h"
extern void initIO(void) {
	pinMode(latch_pin, OUTPUT);
	pinMode(clock_pin, OUTPUT);
	pinMode(data_in_pin, OUTPUT);
	pinMode(data_out_pin, OUTPUT);
	pinMode(modePin1, INPUT_PULLUP);
	pinMode(modePin2, INPUT_PULLUP);
}