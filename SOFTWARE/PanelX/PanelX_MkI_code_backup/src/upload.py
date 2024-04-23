#!/usr/bin/env python
import os
os.system("python src/build.py")
print("UPLOADING")
# os.system("arduino-cli upload -v -b arduino:avr:nano:cpu=atmega328old -p COM3 -i ./build/arduino.avr.nano/PanelX_MkI_code.ino.with_bootloader.hex")
 
os.system("arduino-cli upload -b arduino:avr:nano -p COM10 -i  ./build/arduino.avr.nano/PanelX_MkI_code_backup.ino.with_bootloader.hex")
# os.system("arduino-cli upload -b arduino:avr:uno -p COM8 -P arduinoasisp -i ./build/arduino.avr.uno/PanelX_MkI_code.ino.with_bootloader.hex")
exit