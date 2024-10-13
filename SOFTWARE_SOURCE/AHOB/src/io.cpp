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