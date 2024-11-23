@echo off
setlocal EnableDelayedExpansion

:: PowerShell script to find the COM port and output details
echo Detecting COM port for Arduino or CH340...

:: Get the list of serial ports and their descriptions
powershell -command "Get-WmiObject Win32_PnPEntity | Where-Object { $_.Name -match 'CH340' -or $_.Name -match 'Arduino' } | Format-Table Name, DeviceID"

for /f "tokens=2 delims=()" %%i in ('powershell -command "Get-WmiObject Win32_PnPEntity | Where-Object { $_.Name -match 'CH340' -or $_.Name -match 'Arduino' } | Select-Object -ExpandProperty Name"') do (
    set "comPort=%%i"
)

echo Detected COM port: %comPort%

if "%comPort%"=="" (
    echo Geen Arduino of CH340 USB TTL-converter gevonden.
    pause
    exit /b 1
)

:retry
echo UPLOADING OS-ServoDriver.hex on %comPort%

tools\avrdude -C tools\avrdude.conf -v -V -p atmega328p -c stk500v1 -P \\.\%comPort% -b 19200  -U flash:w:bin\OS-ServoDriver-6fold.hex:i -U lock:w:0x3F:m -U efuse:w:0xFD:m -U hfuse:w:0xDA:m -U lfuse:w:0xFF:m

set /p delBuild=Restart Uploading [y/n]?: 

if /I "%delBuild%"=="Y" goto retry

pause
endlocal
