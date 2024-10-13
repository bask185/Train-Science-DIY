#!/usr/bin/env python
import os
import sys
# print('ASSEMBLING IO FILES')
# os.system("updateIO.py")
# print('ADDING TIME STAMP')
# os.system("addDate.py")
print('BUILDING PROJECT')
x = os.system('arduino-cli compile -b megaTinyCore:megaavr:atxy6:chip=1616,clock=20internal,bodvoltage=1v8,bodmode=disabled,eesave=enable,millis=enabled,resetpin=UPDI,startuptime=8,wiremode=mors,printf=default,PWMmux=A_default,attach=allenabled,WDTtimeout=disabled,WDTwindow=disabled -e')
if x == 1 :
    print('BUILD FAILED!!!')
    sys.exit(1)
else :
    print('BUILD SUCCES!!!')
    sys.exit(0)


#-fqbn=megaTinyCore:megaavr:atxy6:chip=1616,clock=20internal,bodvoltage=1v8,bodmode=disabled,eesave=enable,millis=enabled,resetpin=UPDI,startuptime=8,wiremode=mors,printf=default,PWMmux=A_default,attach=allenabled,WDTtimeout=disabled,WDTwindow=disabled -ide-version=10819 -build-path C:\Users\GEBRUI~1\AppData\Local\Temp\arduino_build_29712 -warnings=more -build-cache C:\Users\GEBRUI~1\AppData\Local\Temp\arduino_cache_646761 -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.avrdude.path=C:\Users\Gebruiker\AppData\Local\Arduino15\packages\DxCore\tools\avrdude\6.3.0-arduino17or18 -prefs=runtime.tools.avrdude-6.3.0-arduino17or18.path=C:\Users\Gebruiker\AppData\Local\Arduino15\packages\DxCore\tools\avrdude\6.3.0-arduino17or18 -prefs=runtime.tools.arduinoOTA.path=C:\Users\Gebruiker\AppData\Local\Arduino15\packages\arduino\tools\arduinoOTA\1.3.0 -prefs=runtime.tools.arduinoOTA-1.3.0.path=C:\Users\Gebruiker\AppData\Local\Arduino15\packages\arduino\tools\arduinoOTA\1.3.0 -prefs=runtime.tools.python3.path=C:\Users\Gebruiker\AppData\Local\Arduino15\packages\megaTinyCore\tools\python3\3.7.2-post1 -prefs=runtime.tools.python3-3.7.2-post1.path=C:\Users\Gebruiker\AppData\Local\Arduino15\packages\megaTinyCore\tools\python3\3.7.2-post1 -prefs=runtime.tools.avr-gcc.path=C:\Users\Gebruiker\AppData\Local\Arduino15\packages\DxCore\tools\avr-gcc\7.3.0-atmel3.6.1-azduino6 -prefs=runtime.tools.avr-gcc-7.3.0-atmel3.6.1-azduino6.path=C:\Users\Gebruiker\AppData\Local\Arduino15\packages\DxCore\tools\avr-gcc\7.3.0-atmel3.6.1-azduino6 -verbose C:\Users\Gebruiker\Documents\PROJECTS\Train-Science-DIY\SOFTWARE\LoPiTS-XS\LoPiTS-XS.ino
