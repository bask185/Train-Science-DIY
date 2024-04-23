echo off
setlocal EnableDelayedExpansion
(set \n=^
%=Do not remove this line=%
)

set /p comNumber= "select comport number!\n!!\n!"

echo UPLOADING XnetBlockModule.hex on COM%comNumber%

tools\avrdude -C tools\avrdude.conf -p atmega328p -V -c stk500v1 -P COM%comNumber% -b 19200 -D -U flash:w:bin\XnetBlockModule.hex.hex:i -U efuse:w:0xFD:m -U hfuse:w:0xDE:m -U lfuse:w:0xFF:m
pause


