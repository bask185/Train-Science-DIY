#!/usr/bin/env python
import os
import sys
import serial.tools.list_ports


def get_com_port():
    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        if 'Arduino' in port.description or 'CH340' in port.description:
            return port.device
    return None


com_port = get_com_port()
if com_port is None:
    print("No device on COM port found")
    sys.exit(1)

retCode = os.system("python src/build.py")
if retCode == 0 :
    print("UPLOADING")
    retCode = os.system(f"arduino-cli upload -b arduino:avr:uno -p {com_port} -i ./build/arduino.avr.uno/MasterOfAccessories.ino.hex")
    if retCode == 1 :
        print("UPLOADING FAILED!!! ")
    else :
        print("UPLOADING SUCCES!!! ")
