#!/usr/bin/env python
import os
os.system("python src/build.py")
print("UPLOADING")
os.system("arduino-cli upload -b arduino:avr:nano -p COM4 -i ./build/arduino.avr.nano/inputModule.ino.hex")
exit