#!/usr/bin/env python
import os
import sys
retCode = os.system("python src/build.py")
if retCode == 0 :
    print("UPLOADING")
    #retCode = os.system("arduino-cli upload -b arduino:avr:nano -p COM8 -i ./build/arduino.avr.nano/OS-ServoDriver.ino.hex")
    retCode = os.system("arduino-cli upload -b arduino:avr:nano -p COM7  -i ./build/arduino.avr.nano/OS-ServoDriver_DCCnext.ino.hex")
###-P arduinoasisp###
    if retCode == 1 :
        print("UPLOADING FAILED!!! ")
    else :
        print("UPLOADING SUCCES!!! ")
