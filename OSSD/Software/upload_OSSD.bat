echo off
setlocal EnableDelayedExpansion
(set \n=^
%=Do not remove this line=%
)

set /p comNumber= "select comport number!\n!!\n!"

echo UPLOADING OSSD.hex on COM%comNumber%

tools\avrdude -C tools\avrdude.conf -p atmega328p -c arduino -P COM%comNumber% -b 115200 -D -U flash:w:bin\OSSD.hex:i
REM  -U efuse:w:0xFD:m -U hfuse:w:0xDE:m -U lfuse:w:0xFF:m for ISP programming
pause