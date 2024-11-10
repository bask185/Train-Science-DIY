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

echo UPLOADING PanelX MK1 Xnet on %comPort%

tools\avrdude -C tools\avrdude.conf -v -p atmega328p -c arduino -P \\.\%comPort% -b 115200 -D -U flash:w:bin\PanelX_MkI_Xnet.hex:i
pause
