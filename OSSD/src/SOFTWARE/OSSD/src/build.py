#!/usr/bin/env python
import os
import sys
import shutil
# print('ASSEMBLING IO FILES')
# os.system("updateIO.py")
# print('ADDING TIME STAMP')
# os.system("addDate.py")
print('BUILDING PROJECT')
x = os.system('arduino-cli compile -b arduino:avr:uno -e')
if x == 1 :
    print('BUILD FAILED!!!')
    sys.exit(1)
else :
    print('BUILD SUCCES!!!')
    shutil.copy("./build/arduino.avr.uno/OSSD.ino.hex", "../../../binary/bin/OSSD.hex")
    sys.exit(0)
