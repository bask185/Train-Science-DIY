#!/usr/bin/env python
import os
import sys
retCode = os.system("python src/build.py")
if retCode == 0 :
    print("UPLOADING")
    #os.system("../../../binary/upload.bat")
    retCode =  os.system("arduino-cli upload -b arduino:avr:uno -p COM7 -P arduinoasisp -v a -i ./build/arduino.avr.uno/OSSD-Mk2.ino.hex") #
    if retCode == 1 :
        print("UPLOADING FAILED!!! ")
    else :
        print("UPLOADING SUCCES!!! ")
