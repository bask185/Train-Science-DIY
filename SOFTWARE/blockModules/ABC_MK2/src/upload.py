#!/usr/bin/env python
import os
import sys
retCode = os.system("python src/build.py")
if retCode == 0 :
    print("UPLOADING")
    retCode = os.system("arduino-cli upload -b arduino:avr:uno -p COM7 -v -P arduinoasisp -i ./build/arduino.avr.uno/ABC_MK2.ino.hex")
    if retCode == 1 :
        print("UPLOADING FAILED!!! ")
    else :
        print("UPLOADING SUCCES!!! ")
