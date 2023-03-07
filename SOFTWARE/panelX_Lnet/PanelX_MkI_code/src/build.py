#!/usr/bin/env python
import os
#os.system("python src/updateIO.py")
#os.system("python src/addDate.py")
print('BUILDING')
os.system('arduino-cli compile -b arduino:avr:nano -e')
exit