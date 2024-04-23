#!/usr/bin/env python
import os


retCode = os.system("arduino-cli burn-bootloader -b arduino:avr:nano -p COM10 -P arduinoasisp")

if( retCode == 0 ):
    print("BURNING BOOTLOADER SUCCES!!! ")
else:
     print("BURNING BOOTLOADER FAILED!!! ")
