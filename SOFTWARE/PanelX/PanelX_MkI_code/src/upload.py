#!/usr/bin/env python
import os
os.system("python src/build.py")
print("UPLOADING")
os.system("arduino-cli upload -b arduino:avr:nano -p COM6 -i ./build/arduino.avr.nano/PanelX_MkI_code.ino.with_bootloader.hex")
exit