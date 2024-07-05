echo off
setlocal EnableDelayedExpansion
(set \n=^
%=Do not remove this line=%
)

set /p comNumber= "select comport number!\n!!\n!"

:retry
echo UPLOADING OS-ServoDriver.hex on COM%comNumber%

tools\avrdude -C tools\avrdude.conf -v -V -p atmega328p -c stk500v1 -P COM%comNumber% -b 19200 -U flash:w:bin\OS-ServoDriver_DCCnext.hex:i -U lock:w:0x3F:m -U efuse:w:0xFD:m -U hfuse:w:0xDA:m -U lfuse:w:0xFF:m

set /p delBuild=Restart Uploading [y/n]?:

if %delBuild% == Y goto retry

pause