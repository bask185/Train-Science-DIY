#!/usr/bin/env python
import os
import sys
retCode = os.system("python src/build.py")
if retCode == 0 :
    print("UPLOADING")
    retCode = os.system("arduino-cli upload -b arduino:avr:uno -p COM6 -P arduinoasisp -v a -i ./build/arduino.avr.uno/OSSD.ino.hex") #
    if retCode == 1 :
        print("UPLOADING FAILED!!! ")
    else :
        print("UPLOADING SUCCES!!! ")
