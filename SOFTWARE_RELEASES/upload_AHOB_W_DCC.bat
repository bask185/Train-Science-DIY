echo off
setlocal EnableDelayedExpansion
(set \n=^
%=Do not remove this line=%
)

set /p comNumber= "select comport number!\n!!\n!"

echo UPLOADING ArduinoISP.hex on COM%comNumber%

tools\avrdude -C tools\avrdude.conf -p atmega328p -c arduino -P COM%comNumber% -b 115200 -D -U flash:w:bin\AHOB_W_DCC.hex:i
pause