EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x03_Male J5
U 1 1 5F3327C3
P 3850 1000
F 0 "J5" V 3900 700 50  0000 L CNN
F 1 "detector" V 3800 500 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 3850 1000 50  0001 C CNN
F 3 "~" H 3850 1000 50  0001 C CNN
	1    3850 1000
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x04_Male J6
U 1 1 5F333CA5
P 4950 1000
F 0 "J6" V 4950 1250 50  0000 L CNN
F 1 "relay" V 5050 1200 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Horizontal" H 4950 1000 50  0001 C CNN
F 3 "~" H 4950 1000 50  0001 C CNN
	1    4950 1000
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x03_Male J4
U 1 1 5F334CD1
P 2850 1000
F 0 "J4" V 2912 1144 50  0000 L CNN
F 1 "config" V 3003 1144 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 2850 1000 50  0001 C CNN
F 3 "~" H 2850 1000 50  0001 C CNN
	1    2850 1000
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x03_Male J7
U 1 1 5F337AE0
P 5700 1000
F 0 "J7" V 5762 1144 50  0000 L CNN
F 1 "servo" V 5853 1144 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 5700 1000 50  0001 C CNN
F 3 "~" H 5700 1000 50  0001 C CNN
	1    5700 1000
	0    1    1    0   
$EndComp
Wire Wire Line
	5700 1300 5700 1200
Wire Wire Line
	2850 1300 2850 1200
Wire Wire Line
	2950 1200 2950 1350
Wire Wire Line
	5800 1200 5800 1350
Connection ~ 2950 1350
$Comp
L Switch:SW_DIP_x04 SW1
U 1 1 5F3C681E
P 9400 1650
F 0 "SW1" V 9354 1980 50  0000 L CNN
F 1 "SW_DIP_x04" V 9445 1980 50  0000 L CNN
F 2 "Button_Switch_THT:SW_DIP_SPSTx04_Piano_10.8x11.72mm_W7.62mm_P2.54mm" H 9400 1650 50  0001 C CNN
F 3 "~" H 9400 1650 50  0001 C CNN
	1    9400 1650
	0    -1   1    0   
$EndComp
Wire Wire Line
	9400 1300 9400 1350
Wire Wire Line
	9400 1300 9300 1300
Wire Wire Line
	9300 1300 9300 1350
Wire Wire Line
	9300 1300 9200 1300
Wire Wire Line
	9200 1300 9200 1350
Connection ~ 9300 1300
$Comp
L Connector:Screw_Terminal_01x03 J2
U 1 1 5F3DC841
P 1450 1000
F 0 "J2" V 1500 1200 50  0000 L CNN
F 1 "controlSignals" V 1400 1200 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-3_P5.08mm" H 1450 1000 50  0001 C CNN
F 3 "~" H 1450 1000 50  0001 C CNN
	1    1450 1000
	0    -1   -1   0   
$EndComp
Text Label 4750 2650 1    50   ~ 0
relayPin
Text Label 3750 2200 3    50   ~ 0
detectorPin
Text Label 2750 2250 3    50   ~ 0
configPin
$Comp
L Connector:Screw_Terminal_01x05 J1
U 1 1 5F32BAB2
P 7600 1050
F 0 "J1" H 7600 1250 50  0000 C CNN
F 1 "signalLights" H 7500 750 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-5_P5.08mm" H 7600 1050 50  0001 C CNN
F 3 "~" H 7600 1050 50  0001 C CNN
	1    7600 1050
	0    1    -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5F324D3E
P 8450 2050
F 0 "R2" V 8350 2050 50  0000 C CNN
F 1 "1k" V 8550 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8380 2050 50  0001 C CNN
F 3 "~" H 8450 2050 50  0001 C CNN
	1    8450 2050
	-1   0    0    1   
$EndComp
$Comp
L Device:R R3
U 1 1 5F324579
P 7950 2050
F 0 "R3" V 7850 2050 50  0000 C CNN
F 1 "1k" V 8050 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7880 2050 50  0001 C CNN
F 3 "~" H 7950 2050 50  0001 C CNN
	1    7950 2050
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 5F323A40
P 7450 2050
F 0 "R1" V 7350 2050 50  0000 C CNN
F 1 "1k" V 7550 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7380 2050 50  0001 C CNN
F 3 "~" H 7450 2050 50  0001 C CNN
	1    7450 2050
	-1   0    0    1   
$EndComp
Text Label 3850 4000 0    50   ~ 0
servoPin
Text Label 4350 4500 2    50   ~ 0
detectorPin
Text Label 3950 5700 0    50   ~ 0
Rx
Text Label 3950 5900 0    50   ~ 0
Tx
Text Label 3950 5500 0    50   ~ 0
relayPin
Text Label 3850 3700 0    50   ~ 0
yellowLed
Text Label 3950 5600 0    50   ~ 0
redLed
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 5F35E8C4
P 1950 1000
F 0 "J3" V 2012 1144 50  0000 L CNN
F 1 "buttons" V 2103 1144 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Horizontal" H 1950 1000 50  0001 C CNN
F 3 "~" H 1950 1000 50  0001 C CNN
	1    1950 1000
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5F485885
P 9750 1300
F 0 "#PWR06" H 9750 1050 50  0001 C CNN
F 1 "GND" H 9755 1127 50  0000 C CNN
F 2 "" H 9750 1300 50  0001 C CNN
F 3 "" H 9750 1300 50  0001 C CNN
	1    9750 1300
	-1   0    0    -1  
$EndComp
Text Label 3850 3600 0    50   ~ 0
greenLed
$Comp
L Connector:Screw_Terminal_01x02 J9
U 1 1 5F406EA2
P 6600 3750
F 0 "J9" V 6650 3900 50  0000 L CNN
F 1 "power" V 6550 3850 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 6600 3750 50  0001 C CNN
F 3 "~" H 6600 3750 50  0001 C CNN
	1    6600 3750
	-1   0    0    1   
$EndComp
Text Label 5600 2650 1    50   ~ 0
servoPin
Text Label 4100 5000 2    50   ~ 0
dip0
Text Label 4100 4900 2    50   ~ 0
dip1
Text Label 4100 4800 2    50   ~ 0
dip2
Text Label 4100 4700 2    50   ~ 0
dip3
$Comp
L power:GND #PWR03
U 1 1 5F48AF60
P 4450 1350
F 0 "#PWR03" H 4450 1100 50  0001 C CNN
F 1 "GND" H 4455 1177 50  0000 C CNN
F 2 "" H 4450 1350 50  0001 C CNN
F 3 "" H 4450 1350 50  0001 C CNN
	1    4450 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1350 5050 1350
Connection ~ 4450 1350
Text Label 1350 2650 1    50   ~ 0
Tx
Text Label 1750 2650 1    50   ~ 0
buttonPin
Entry Wire Line
	3450 3900 3550 4000
Entry Wire Line
	3450 5600 3550 5700
Entry Wire Line
	3450 5800 3550 5900
Entry Wire Line
	3450 5700 3550 5800
Entry Wire Line
	6200 4200 6300 4300
Entry Wire Line
	1750 2700 1850 2800
Entry Wire Line
	1550 2700 1650 2800
Entry Wire Line
	1350 2700 1450 2800
Entry Wire Line
	2750 2700 2850 2800
Wire Wire Line
	2750 1200 2750 2400
Entry Wire Line
	4650 2800 4750 2700
Entry Wire Line
	5500 2800 5600 2700
Wire Wire Line
	5600 1200 5600 2700
$Comp
L power:+12V #PWR01
U 1 1 5F7193C7
P 8000 1250
F 0 "#PWR01" H 8000 1100 50  0001 C CNN
F 1 "+12V" H 8015 1423 50  0000 C CNN
F 2 "" H 8000 1250 50  0001 C CNN
F 3 "" H 8000 1250 50  0001 C CNN
	1    8000 1250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR02
U 1 1 5F74EC20
P 4400 1300
F 0 "#PWR02" H 4400 1150 50  0001 C CNN
F 1 "+5V" H 4415 1473 50  0000 C CNN
F 2 "" H 4400 1300 50  0001 C CNN
F 3 "" H 4400 1300 50  0001 C CNN
	1    4400 1300
	1    0    0    -1  
$EndComp
Connection ~ 4400 1300
Entry Wire Line
	4750 2800 4850 2700
Text Label 4850 2650 1    50   ~ 0
slowSpeed
Text Label 3950 5400 0    50   ~ 0
slowSpeed
Text Label 1550 2650 1    50   ~ 0
Rx
Text Notes 4700 900  0    50   ~ 0
DC brake\nABC type1\nABC type2\nshut off\n(analog brake?)
Wire Wire Line
	2850 1300 3100 1300
Entry Wire Line
	3650 2800 3750 2700
Wire Wire Line
	2050 1350 2950 1350
Wire Wire Line
	2050 1350 2050 1200
Connection ~ 5050 1350
Wire Wire Line
	5050 1350 5800 1350
Connection ~ 4950 1300
Wire Wire Line
	4950 1300 5700 1300
Wire Wire Line
	4400 1300 4950 1300
Wire Wire Line
	5050 1200 5050 1350
Wire Wire Line
	4950 1200 4950 1300
Wire Wire Line
	4850 1200 4850 2700
Wire Wire Line
	4750 1200 4750 2700
Entry Bus Bus
	3450 2900 3550 2800
Entry Bus Bus
	3350 2800 3450 2900
Entry Wire Line
	1450 2700 1550 2800
Text Label 1450 2650 1    50   ~ 0
direction
Entry Wire Line
	3450 4400 3550 4500
Text Label 3950 5800 0    50   ~ 0
direction
Entry Wire Line
	3450 4300 3550 4400
Text Label 6100 4200 2    50   ~ 0
pot
Entry Wire Line
	4000 2800 4100 2700
$Comp
L Device:R_POT RV1
U 1 1 5F5AB590
P 4100 1950
F 0 "RV1" V 3893 1950 50  0000 C CNN
F 1 "10K" V 3984 1950 50  0000 C CNN
F 2 "Potentiometer_THT:Potentiometer_Runtron_RM-065_Vertical" H 4100 1950 50  0001 C CNN
F 3 "~" H 4100 1950 50  0001 C CNN
	1    4100 1950
	0    1    1    0   
$EndComp
Wire Wire Line
	4100 2700 4100 2450
Wire Wire Line
	3950 1950 3950 1350
Connection ~ 3950 1350
Wire Wire Line
	3950 1350 4450 1350
Wire Wire Line
	4250 1950 4250 1300
Connection ~ 4250 1300
Wire Wire Line
	4250 1300 4400 1300
Text Label 4100 2600 1    50   ~ 0
pot
Wire Wire Line
	3750 1200 3750 1700
Wire Wire Line
	1550 1200 1550 2700
Wire Wire Line
	1350 1200 1350 2700
Wire Wire Line
	3850 1300 3850 1200
Connection ~ 3850 1300
Wire Wire Line
	3850 1300 4250 1300
Wire Wire Line
	3950 1350 3950 1200
Text Notes 4150 2150 0    50   ~ 0
fall-off time\n
$Comp
L Device:R R4
U 1 1 5F6055E7
P 3350 1700
F 0 "R4" V 3250 1700 50  0000 C CNN
F 1 "100k" V 3450 1700 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3280 1700 50  0001 C CNN
F 3 "~" H 3350 1700 50  0001 C CNN
	1    3350 1700
	0    -1   -1   0   
$EndComp
Text Notes 2900 2000 0    50   ~ 0
optional pull down \nfor entry/exit signals
Wire Wire Line
	3750 1700 3500 1700
Connection ~ 3750 1700
Wire Wire Line
	3750 1700 3750 2700
Wire Wire Line
	1450 1200 1450 2700
Wire Wire Line
	2950 1350 3150 1350
$Comp
L Connector:Conn_01x03_Male J10
U 1 1 5F624CCC
P 2900 1700
F 0 "J10" H 2800 1650 50  0000 L CNN
F 1 "jumper" H 2800 1900 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 2900 1700 50  0001 C CNN
F 3 "~" H 2900 1700 50  0001 C CNN
	1    2900 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 1700 3100 1700
Wire Wire Line
	3100 1600 3100 1300
Connection ~ 3100 1300
Wire Wire Line
	3100 1300 3850 1300
Wire Wire Line
	3100 1800 3150 1800
Wire Wire Line
	3150 1800 3150 1350
Connection ~ 3150 1350
Wire Wire Line
	3150 1350 3950 1350
Wire Wire Line
	9400 1300 9500 1300
Wire Wire Line
	9500 1300 9500 1350
Connection ~ 9400 1300
$Comp
L Connector:Conn_01x03_Male J8
U 1 1 601E617F
P 6250 1000
F 0 "J8" V 6312 1144 50  0000 L CNN
F 1 "servo" V 6403 1144 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6250 1000 50  0001 C CNN
F 3 "~" H 6250 1000 50  0001 C CNN
	1    6250 1000
	0    1    1    0   
$EndComp
Wire Wire Line
	5700 1300 6250 1300
Wire Wire Line
	6250 1300 6250 1200
Connection ~ 5700 1300
Wire Wire Line
	6350 1200 6350 1350
Wire Wire Line
	6350 1350 5800 1350
Connection ~ 5800 1350
Wire Wire Line
	6150 1200 6150 2700
Entry Wire Line
	6050 2800 6150 2700
Text Label 6150 2650 1    50   ~ 0
preSignal
Text Label 3850 3800 0    50   ~ 0
preSignal
Entry Wire Line
	3450 5500 3550 5600
Entry Wire Line
	3450 3700 3550 3800
$Comp
L Device:R R5
U 1 1 60279B88
P 1750 2000
F 0 "R5" V 1650 1900 50  0000 C CNN
F 1 "20k" V 1650 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1680 2000 50  0001 C CNN
F 3 "~" H 1750 2000 50  0001 C CNN
	1    1750 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 6028464F
P 2150 2000
F 0 "R8" V 2050 1900 50  0000 C CNN
F 1 "1k" V 2050 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2080 2000 50  0001 C CNN
F 3 "~" H 2150 2000 50  0001 C CNN
	1    2150 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 60280C39
P 1950 2000
F 0 "R6" V 1850 1900 50  0000 C CNN
F 1 "10k" V 1850 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1880 2000 50  0001 C CNN
F 3 "~" H 1950 2000 50  0001 C CNN
	1    1950 2000
	1    0    0    -1  
$EndComp
Connection ~ 1750 2200
Wire Wire Line
	1750 2200 1750 2150
Wire Wire Line
	1950 2150 1950 2200
Wire Wire Line
	1950 2200 1750 2200
$Comp
L Device:R R7
U 1 1 602B4087
P 2050 2500
F 0 "R7" V 1950 2400 50  0000 C CNN
F 1 "10k" V 1950 2550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1980 2500 50  0001 C CNN
F 3 "~" H 2050 2500 50  0001 C CNN
	1    2050 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	1750 2500 1750 2200
Wire Wire Line
	1950 1850 1950 1800
Wire Wire Line
	1950 1800 1850 1800
Wire Wire Line
	2150 1850 2150 1750
Wire Wire Line
	2150 1750 1950 1750
$Comp
L power:+5V #PWR0101
U 1 1 602CDB24
P 2300 2500
F 0 "#PWR0101" H 2300 2350 50  0001 C CNN
F 1 "+5V" H 2315 2673 50  0000 C CNN
F 2 "" H 2300 2500 50  0001 C CNN
F 3 "" H 2300 2500 50  0001 C CNN
	1    2300 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 2500 2200 2500
Wire Wire Line
	1950 2200 2150 2200
Wire Wire Line
	2150 2200 2150 2150
Connection ~ 1950 2200
Wire Wire Line
	1750 2700 1750 2500
Connection ~ 1750 2500
Wire Wire Line
	1750 1200 1750 1850
Wire Wire Line
	1850 1200 1850 1800
Wire Wire Line
	1950 1200 1950 1750
Text Label 1750 1650 1    50   ~ 0
greenButton
Text Label 1850 1650 1    50   ~ 0
yellowButton
Text Label 1950 1650 1    50   ~ 0
redButton
Wire Wire Line
	1900 2500 1750 2500
$Comp
L Device:C C3
U 1 1 602FDFE2
P 4300 2450
F 0 "C3" V 4048 2450 50  0000 C CNN
F 1 "C" V 4139 2450 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4338 2300 50  0001 C CNN
F 3 "~" H 4300 2450 50  0001 C CNN
	1    4300 2450
	0    1    1    0   
$EndComp
Wire Wire Line
	4150 2450 4100 2450
Connection ~ 4100 2450
Wire Wire Line
	4100 2450 4100 2100
$Comp
L power:GND #PWR0102
U 1 1 60303DDB
P 4500 2450
F 0 "#PWR0102" H 4500 2200 50  0001 C CNN
F 1 "GND" H 4505 2277 50  0000 C CNN
F 2 "" H 4500 2450 50  0001 C CNN
F 3 "" H 4500 2450 50  0001 C CNN
	1    4500 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 2450 4450 2450
$Comp
L Device:C C1
U 1 1 603095E1
P 3000 2400
F 0 "C1" V 2748 2400 50  0000 C CNN
F 1 "C" V 2839 2400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3038 2250 50  0001 C CNN
F 3 "~" H 3000 2400 50  0001 C CNN
	1    3000 2400
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 60309923
P 3250 2400
F 0 "#PWR0103" H 3250 2150 50  0001 C CNN
F 1 "GND" H 3255 2227 50  0000 C CNN
F 2 "" H 3250 2400 50  0001 C CNN
F 3 "" H 3250 2400 50  0001 C CNN
	1    3250 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 2400 2750 2400
Connection ~ 2750 2400
Wire Wire Line
	2750 2400 2750 2700
Wire Wire Line
	3150 2400 3250 2400
Entry Wire Line
	3450 4500 3550 4600
Text Label 4300 4600 2    50   ~ 0
buttonPin
$Comp
L Device:R R9
U 1 1 60326844
P 6950 2050
F 0 "R9" V 6850 2050 50  0000 C CNN
F 1 "1k" V 7050 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6880 2050 50  0001 C CNN
F 3 "~" H 6950 2050 50  0001 C CNN
	1    6950 2050
	-1   0    0    1   
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 60375D0E
P 4450 6700
F 0 "Y1" H 4450 6432 50  0000 C CNN
F 1 "Crystal" H 4450 6523 50  0000 C CNN
F 2 "Crystal:Crystal_SMD_2012-2Pin_2.0x1.2mm" H 4450 6700 50  0001 C CNN
F 3 "~" H 4450 6700 50  0001 C CNN
	1    4450 6700
	-1   0    0    1   
$EndComp
Entry Wire Line
	3450 4600 3550 4700
Entry Wire Line
	3450 4800 3550 4900
Entry Wire Line
	3450 4700 3550 4800
Entry Wire Line
	3450 4900 3550 5000
Text Label 6100 4100 2    50   ~ 0
configPin
Wire Wire Line
	3550 4400 4450 4400
Wire Wire Line
	4450 4500 3550 4500
Wire Wire Line
	3550 4600 4450 4600
Wire Wire Line
	4450 4700 3550 4700
Wire Wire Line
	3550 4800 4450 4800
Wire Wire Line
	4450 4900 3550 4900
Wire Wire Line
	3550 5000 4450 5000
Entry Wire Line
	3450 3400 3550 3500
Entry Wire Line
	3450 3600 3550 3700
Entry Wire Line
	3450 3500 3550 3600
Entry Wire Line
	3450 3800 3550 3900
Wire Wire Line
	3550 3600 4450 3600
Wire Wire Line
	3550 3700 4450 3700
Wire Wire Line
	3550 3900 4450 3900
Text Label 3850 3900 0    50   ~ 0
yellowLed2
Wire Wire Line
	3550 5600 4450 5600
Wire Wire Line
	4450 3500 3550 3500
Entry Wire Line
	6200 4100 6300 4200
Wire Wire Line
	5650 4100 6200 4100
Wire Wire Line
	5650 4200 6200 4200
Wire Wire Line
	3550 5900 4450 5900
Wire Wire Line
	3550 5800 4450 5800
Wire Wire Line
	3550 5700 4450 5700
Entry Wire Line
	3450 5300 3550 5400
Entry Wire Line
	3450 5400 3550 5500
Wire Wire Line
	3550 5500 4450 5500
Wire Wire Line
	3550 5400 4450 5400
Entry Wire Line
	3450 5200 3550 5300
Entry Wire Line
	3450 5100 3550 5200
Wire Wire Line
	3550 5300 4450 5300
Wire Wire Line
	3550 5200 4450 5200
Text Label 3950 5300 0    50   ~ 0
X1
Text Label 3950 5200 0    50   ~ 0
X2
Wire Wire Line
	3550 3800 4450 3800
Wire Wire Line
	3550 4000 4450 4000
Wire Bus Line
	6300 6400 3450 6400
$Comp
L Device:C C2
U 1 1 604FD11B
P 4100 6850
F 0 "C2" H 4215 6896 50  0000 L CNN
F 1 "22nF" H 4215 6805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4138 6700 50  0001 C CNN
F 3 "~" H 4100 6850 50  0001 C CNN
	1    4100 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 604FD63B
P 4800 6850
F 0 "C4" H 4915 6896 50  0000 L CNN
F 1 "22nF" H 4915 6805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4838 6700 50  0001 C CNN
F 3 "~" H 4800 6850 50  0001 C CNN
	1    4800 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 6700 4600 6700
Wire Wire Line
	4300 6700 4100 6700
Wire Wire Line
	4800 7000 4450 7000
Text Label 4200 6700 0    50   ~ 0
X1
Text Label 4650 6700 0    50   ~ 0
X2
$Comp
L power:GND #PWR0104
U 1 1 6050EAE2
P 4450 7000
F 0 "#PWR0104" H 4450 6750 50  0001 C CNN
F 1 "GND" H 4455 6827 50  0000 C CNN
F 2 "" H 4450 7000 50  0001 C CNN
F 3 "" H 4450 7000 50  0001 C CNN
	1    4450 7000
	1    0    0    -1  
$EndComp
Connection ~ 4450 7000
Wire Wire Line
	4450 7000 4100 7000
$Comp
L power:GND #PWR0105
U 1 1 60568EA1
P 6650 2000
F 0 "#PWR0105" H 6650 1750 50  0001 C CNN
F 1 "GND" H 6655 1827 50  0000 C CNN
F 2 "" H 6650 2000 50  0001 C CNN
F 3 "" H 6650 2000 50  0001 C CNN
	1    6650 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 605696A8
P 7150 2000
F 0 "#PWR0106" H 7150 1750 50  0001 C CNN
F 1 "GND" H 7155 1827 50  0000 C CNN
F 2 "" H 7150 2000 50  0001 C CNN
F 3 "" H 7150 2000 50  0001 C CNN
	1    7150 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 605698AC
P 7650 2000
F 0 "#PWR0107" H 7650 1750 50  0001 C CNN
F 1 "GND" H 7655 1827 50  0000 C CNN
F 2 "" H 7650 2000 50  0001 C CNN
F 3 "" H 7650 2000 50  0001 C CNN
	1    7650 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 60569A7B
P 8150 2000
F 0 "#PWR0108" H 8150 1750 50  0001 C CNN
F 1 "GND" H 8155 1827 50  0000 C CNN
F 2 "" H 8150 2000 50  0001 C CNN
F 3 "" H 8150 2000 50  0001 C CNN
	1    8150 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 1900 8450 1750
Wire Wire Line
	8150 2000 8150 1950
Wire Wire Line
	7950 1900 7950 1750
Wire Wire Line
	7650 2000 7650 1950
Wire Wire Line
	7450 1900 7450 1750
Wire Wire Line
	7150 2000 7150 1950
Wire Wire Line
	6950 1900 6950 1750
Wire Wire Line
	7600 1450 7650 1450
Wire Wire Line
	7650 1450 7650 1550
Wire Wire Line
	7500 1450 7150 1450
Wire Wire Line
	7150 1450 7150 1550
Wire Wire Line
	6650 1550 6650 1400
Wire Wire Line
	6650 1400 7400 1400
Wire Wire Line
	7400 1400 7400 1250
Wire Wire Line
	7700 1400 8150 1400
Wire Wire Line
	8150 1400 8150 1550
Wire Wire Line
	8000 1250 7800 1250
Entry Wire Line
	6850 2800 6950 2700
Entry Wire Line
	7350 2800 7450 2700
Entry Wire Line
	7850 2800 7950 2700
Wire Wire Line
	7500 1250 7500 1450
Wire Wire Line
	7600 1250 7600 1450
Wire Wire Line
	7700 1250 7700 1400
Wire Wire Line
	6950 2200 6950 2700
Wire Wire Line
	7450 2200 7450 2700
Wire Wire Line
	7950 2200 7950 2700
Wire Wire Line
	8450 2200 8450 2700
Text Label 8450 2650 1    50   ~ 0
greenLed
Text Label 7950 2650 1    50   ~ 0
yellowLed
Text Label 7450 2650 1    50   ~ 0
yellowLed2
Text Label 6950 2600 1    50   ~ 0
redLed
Entry Wire Line
	8350 2800 8450 2700
Entry Wire Line
	9100 2800 9200 2700
Entry Wire Line
	9200 2800 9300 2700
Entry Wire Line
	9300 2800 9400 2700
Entry Wire Line
	9400 2800 9500 2700
Wire Wire Line
	9200 2700 9200 1950
Wire Wire Line
	9300 1950 9300 2700
Wire Wire Line
	9400 2700 9400 1950
Wire Wire Line
	9500 1950 9500 2700
Text Label 9500 2550 1    50   ~ 0
dip0
Text Label 9400 2550 1    50   ~ 0
dip1
Text Label 9300 2550 1    50   ~ 0
dip2
Text Label 9200 2550 1    50   ~ 0
dip3
Wire Wire Line
	6650 2000 6650 1950
$Comp
L power:GND #PWR0109
U 1 1 60689DBE
P 5850 3250
F 0 "#PWR0109" H 5850 3000 50  0001 C CNN
F 1 "GND" H 5855 3077 50  0000 C CNN
F 2 "" H 5850 3250 50  0001 C CNN
F 3 "" H 5850 3250 50  0001 C CNN
	1    5850 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 3250 5850 3200
Wire Wire Line
	5050 3200 5850 3200
$Comp
L Device:C C5
U 1 1 606991F1
P 5600 6850
F 0 "C5" V 5348 6850 50  0000 C CNN
F 1 "100nF" V 5439 6850 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5638 6700 50  0001 C CNN
F 3 "~" H 5600 6850 50  0001 C CNN
	1    5600 6850
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0110
U 1 1 6069A3C0
P 5350 6850
F 0 "#PWR0110" H 5350 6700 50  0001 C CNN
F 1 "+5V" H 5365 7023 50  0000 C CNN
F 2 "" H 5350 6850 50  0001 C CNN
F 3 "" H 5350 6850 50  0001 C CNN
	1    5350 6850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 6069AD99
P 5900 6850
F 0 "#PWR0111" H 5900 6600 50  0001 C CNN
F 1 "GND" H 5905 6677 50  0000 C CNN
F 2 "" H 5900 6850 50  0001 C CNN
F 3 "" H 5900 6850 50  0001 C CNN
	1    5900 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 6850 5450 6850
Wire Wire Line
	5900 6850 5750 6850
$Comp
L power:+5V #PWR0112
U 1 1 606A93B8
P 5800 6200
F 0 "#PWR0112" H 5800 6050 50  0001 C CNN
F 1 "+5V" H 5815 6373 50  0000 C CNN
F 2 "" H 5800 6200 50  0001 C CNN
F 3 "" H 5800 6200 50  0001 C CNN
	1    5800 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 6200 5650 6200
Wire Wire Line
	5650 5900 5650 6200
Connection ~ 5650 6200
$Comp
L Transistor_BJT:BC847 Q4
U 1 1 606BE763
P 8250 1750
F 0 "Q4" H 8450 1950 50  0000 L CNN
F 1 "BC847" H 8450 1850 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8450 1675 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 8250 1750 50  0001 L CNN
	1    8250 1750
	-1   0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC847 Q3
U 1 1 606C6D78
P 7750 1750
F 0 "Q3" H 7950 1950 50  0000 L CNN
F 1 "BC847" H 7950 1850 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7950 1675 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 7750 1750 50  0001 L CNN
	1    7750 1750
	-1   0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC847 Q2
U 1 1 606C772B
P 7250 1750
F 0 "Q2" H 7450 1950 50  0000 L CNN
F 1 "BC847" H 7400 1850 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7450 1675 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 7250 1750 50  0001 L CNN
	1    7250 1750
	-1   0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC847 Q1
U 1 1 606C7E84
P 6750 1750
F 0 "Q1" H 6950 1950 50  0000 L CNN
F 1 "BC847" H 6950 1850 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 6950 1675 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 6750 1750 50  0001 L CNN
	1    6750 1750
	-1   0    0    -1  
$EndComp
$Comp
L Device:L L1
U 1 1 606F2496
P 9300 3850
F 0 "L1" V 9490 3850 50  0000 C CNN
F 1 "L" V 9399 3850 50  0000 C CNN
F 2 "Inductor_SMD:L_6.3x6.3_H3" H 9300 3850 50  0001 C CNN
F 3 "~" H 9300 3850 50  0001 C CNN
	1    9300 3850
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C7
U 1 1 606F913E
P 9600 4000
F 0 "C7" H 9715 4046 50  0000 L CNN
F 1 "C" H 9715 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 9638 3850 50  0001 C CNN
F 3 "~" H 9600 4000 50  0001 C CNN
	1    9600 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 6070C6E4
P 8150 4200
F 0 "#PWR0113" H 8150 3950 50  0001 C CNN
F 1 "GND" H 8155 4027 50  0000 C CNN
F 2 "" H 8150 4200 50  0001 C CNN
F 3 "" H 8150 4200 50  0001 C CNN
	1    8150 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 60750046
P 7300 3800
F 0 "C6" H 7415 3846 50  0000 L CNN
F 1 "C" H 7415 3755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7338 3650 50  0001 C CNN
F 3 "~" H 7300 3800 50  0001 C CNN
	1    7300 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:D D1
U 1 1 60768AC2
P 7000 3650
F 0 "D1" H 7000 3433 50  0000 C CNN
F 1 "D" H 7000 3524 50  0000 C CNN
F 2 "Diode_SMD:D_2010_5025Metric" H 7000 3650 50  0001 C CNN
F 3 "~" H 7000 3650 50  0001 C CNN
	1    7000 3650
	-1   0    0    1   
$EndComp
$Comp
L Regulator_Switching:LM2596S-5 U2
U 1 1 607C6211
P 8150 3750
F 0 "U2" H 8150 4117 50  0000 C CNN
F 1 "LM2596S-5" H 8150 4026 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-263-5_TabPin3" H 8200 3500 50  0001 L CIN
F 3 "http://www.ti.com/lit/ds/symlink/lm2596.pdf" H 8150 3750 50  0001 C CNN
	1    8150 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D2
U 1 1 607E4F04
P 8900 4000
F 0 "D2" V 8854 4080 50  0000 L CNN
F 1 "D_Schottky" V 8945 4080 50  0000 L CNN
F 2 "Diode_SMD:D_2010_5025Metric" H 8900 4000 50  0001 C CNN
F 3 "~" H 8900 4000 50  0001 C CNN
	1    8900 4000
	0    1    1    0   
$EndComp
Wire Wire Line
	8650 3850 8900 3850
Wire Wire Line
	8900 3850 9150 3850
Connection ~ 8900 3850
Wire Wire Line
	9450 3850 9600 3850
Wire Wire Line
	8650 3650 9600 3650
Wire Wire Line
	9600 3650 9600 3850
Connection ~ 9600 3850
Wire Wire Line
	9600 4150 9600 4200
Wire Wire Line
	9600 4200 8900 4200
Wire Wire Line
	8900 4200 8900 4150
Wire Wire Line
	8900 4200 8150 4200
Wire Wire Line
	8150 4200 8150 4050
Connection ~ 8900 4200
Wire Wire Line
	8150 4200 7650 4200
Wire Wire Line
	7300 4200 7300 3950
Connection ~ 8150 4200
Wire Wire Line
	7650 3850 7650 4200
Connection ~ 7650 4200
Wire Wire Line
	7650 4200 7300 4200
Wire Wire Line
	7150 3650 7300 3650
Wire Wire Line
	7300 3650 7400 3650
Connection ~ 7300 3650
Wire Wire Line
	6800 3650 6850 3650
Wire Wire Line
	6800 3750 7100 3750
Wire Wire Line
	7100 3750 7100 4200
Wire Wire Line
	7100 4200 7300 4200
Connection ~ 7300 4200
$Comp
L power:+12V #PWR0114
U 1 1 6084A342
P 7400 3650
F 0 "#PWR0114" H 7400 3500 50  0001 C CNN
F 1 "+12V" H 7415 3823 50  0000 C CNN
F 2 "" H 7400 3650 50  0001 C CNN
F 3 "" H 7400 3650 50  0001 C CNN
	1    7400 3650
	1    0    0    -1  
$EndComp
Connection ~ 7400 3650
Wire Wire Line
	7400 3650 7650 3650
Wire Wire Line
	9600 3850 9900 3850
$Comp
L power:+5V #PWR0115
U 1 1 60852643
P 9900 3850
F 0 "#PWR0115" H 9900 3700 50  0001 C CNN
F 1 "+5V" H 9915 4023 50  0000 C CNN
F 2 "" H 9900 3850 50  0001 C CNN
F 3 "" H 9900 3850 50  0001 C CNN
	1    9900 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 1300 9500 1300
Connection ~ 9500 1300
$Comp
L w_connectors:HEADER_3x2 J11
U 1 1 6086D0D9
P 10550 1650
F 0 "J11" H 10550 1987 60  0000 C CNN
F 1 "HEADER_3x2" H 10550 1881 60  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 10550 1650 60  0001 C CNN
F 3 "" H 10550 1650 60  0000 C CNN
	1    10550 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 1650 10300 2700
Wire Wire Line
	10700 1650 10850 1650
Wire Wire Line
	10850 1650 10850 2700
Entry Wire Line
	10200 2800 10300 2700
Entry Wire Line
	10300 2800 10400 2700
Entry Wire Line
	10750 2800 10850 2700
Wire Wire Line
	10400 1650 10300 1650
$Comp
L power:GND #PWR0116
U 1 1 608A6C24
P 10950 1800
F 0 "#PWR0116" H 10950 1550 50  0001 C CNN
F 1 "GND" H 10955 1627 50  0000 C CNN
F 2 "" H 10950 1800 50  0001 C CNN
F 3 "" H 10950 1800 50  0001 C CNN
	1    10950 1800
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0117
U 1 1 608A78E3
P 10950 1500
F 0 "#PWR0117" H 10950 1350 50  0001 C CNN
F 1 "+5V" H 10965 1673 50  0000 C CNN
F 2 "" H 10950 1500 50  0001 C CNN
F 3 "" H 10950 1500 50  0001 C CNN
	1    10950 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	10950 1550 10700 1550
Wire Wire Line
	10950 1800 10950 1750
Wire Wire Line
	10950 1750 10700 1750
Wire Wire Line
	10950 1550 10950 1500
Text Notes 10850 1700 0    50   ~ 0
11\n
Text Notes 10200 1850 0    50   ~ 0
13
Wire Wire Line
	10400 1550 10200 1550
Wire Wire Line
	10200 1550 10200 2700
Entry Wire Line
	10100 2800 10200 2700
Text Notes 10150 1550 0    50   ~ 0
12
Wire Wire Line
	10400 1750 10400 2450
Text Label 10300 2600 1    50   ~ 0
slowSpeed
Text Label 10200 2600 1    50   ~ 0
relayPin
Text Label 10850 2650 1    50   ~ 0
redLed
Text Notes 10450 1900 0    50   ~ 0
ICSP
Text Label 10400 2600 1    50   ~ 0
reset
Text Label 3950 4400 0    50   ~ 0
reset
$Comp
L Device:R R10
U 1 1 608F2586
P 10600 2300
F 0 "R10" H 10670 2346 50  0000 L CNN
F 1 "10k" H 10670 2255 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10530 2300 50  0001 C CNN
F 3 "~" H 10600 2300 50  0001 C CNN
	1    10600 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10600 2450 10400 2450
Connection ~ 10400 2450
Wire Wire Line
	10400 2450 10400 2700
Wire Wire Line
	10600 2150 11100 2150
Wire Wire Line
	11100 2150 11100 1550
Wire Wire Line
	11100 1550 10950 1550
Connection ~ 10950 1550
NoConn ~ 4450 4100
NoConn ~ 4450 4200
$Comp
L MCU_Microchip_ATmega:ATmega328PB-MU U1
U 1 1 60F05002
P 5050 4700
F 0 "U1" H 5050 3019 50  0000 C CNN
F 1 "ATmega328PB-MU" H 5050 3110 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-32-1EP_5x5mm_P0.5mm_EP3.1x3.1mm" H 5050 4700 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/40001906C.pdf" H 5050 4700 50  0001 C CNN
	1    5050 4700
	-1   0    0    1   
$EndComp
Wire Wire Line
	5050 6200 5650 6200
Wire Bus Line
	6300 4100 6300 6400
Wire Bus Line
	1200 2800 3350 2800
Wire Bus Line
	3550 2800 11100 2800
Wire Bus Line
	3450 2900 3450 6400
$EndSCHEMATC