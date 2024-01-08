#!/usr/bin/env python
import os


retCode = os.system("arduino-cli burn-bootloader -b arduino:avr:nano -p COM8 -P arduinoasisp")

print("BURNING BOOTLOADER SUCCES!!! ")
