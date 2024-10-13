#!/usr/bin/env python
import os
import sys
retCode = os.system("python src/build.py")
if retCode == 0 :
    print("UPLOADING")
    #retCode = os.system("arduino-cli upload -b megaTinyCore:megaavr:atxy6:chip=1616 -P jtag2updi -p COM8 -i ./build/arduino.avr.nano/LoPiTS-XS.ino.hex")

    retCode = os.system("arduino-cli upload -b megaTinyCore:megaavr:atxy6:chip=1667,clock=20internal,bodvoltage=1v8,bodmode=disabled,eesave=enable,millis=enabled,resetpin=UPDI,startuptime=0,wiremode=mors,attach=allenabled -p COM8 -P serialupdi921k -t -i ./build/arduino.avr.nano/LoPiTS-XS.ino.hex"")


    if retCode == 1 :
        print("UPLOADING FAILED!!! ")
    else :
        print("UPLOADING SUCCES!!! ")
