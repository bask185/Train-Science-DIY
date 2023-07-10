#!/usr/bin/env python
import os
import sys

print('BUILDING PROJECT')
x = os.system('arduino-cli compile -b arduino:avr:nano -e')
if x == 1 :
    print('BUILD FAILED!!!')
    sys.exit(1)
else :
    print('BUILD SUCCES!!!')
    sys.exit(0)
