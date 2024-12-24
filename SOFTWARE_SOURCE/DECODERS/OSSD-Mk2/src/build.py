#!/usr/bin/env python
import os
import sys
import shutil
# print('ASSEMBLING IO FILES')
# os.system("updateIO.py")
# print('ADDING TIME STAMP')
# os.system("addDate.py")
print('BUILDING PROJECT...\r\n')
x = os.system('arduino-cli compile -b arduino:avr:uno -e')
if x == 1 :
    print('BUILD FAILED!!!')
    sys.exit(1)
else :
    print('BUILD SUCCES!!!\r\n')
    #shutil.copy("./build/arduino.avr.uno/OSSD-Mk2.ino.hex", "../../../SOFTWARE_RELEASES/bin/OSSD-Mk2.hex")
    #print('NEW BINARY DEPLOYED!')
    sys.exit(0)
