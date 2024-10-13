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
L Jumper:SolderJumper_2_Open JP1
U 1 1 61DD709A
P 5450 2300
F 0 "JP1" V 5404 2368 50  0000 L CNN
F 1 "ju" V 5495 2368 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 5450 2300 50  0001 C CNN
F 3 "~" H 5450 2300 50  0001 C CNN
	1    5450 2300
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP2
U 1 1 61DD71B7
P 5650 2300
F 0 "JP2" V 5604 2368 50  0000 L CNN
F 1 "ju" V 5695 2368 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 5650 2300 50  0001 C CNN
F 3 "~" H 5650 2300 50  0001 C CNN
	1    5650 2300
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP3
U 1 1 61DD8A58
P 5850 2300
F 0 "JP3" V 5804 2368 50  0000 L CNN
F 1 "ju" V 5895 2368 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 5850 2300 50  0001 C CNN
F 3 "~" H 5850 2300 50  0001 C CNN
	1    5850 2300
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP4
U 1 1 61DDAEF6
P 6050 2300
F 0 "JP4" V 6004 2368 50  0000 L CNN
F 1 "ju" V 6095 2368 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 6050 2300 50  0001 C CNN
F 3 "~" H 6050 2300 50  0001 C CNN
	1    6050 2300
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP5
U 1 1 61DDAEFC
P 6250 2300
F 0 "JP5" V 6204 2368 50  0000 L CNN
F 1 "ju" V 6295 2368 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 6250 2300 50  0001 C CNN
F 3 "~" H 6250 2300 50  0001 C CNN
	1    6250 2300
	0    1    1    0   
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP6
U 1 1 61DDAF02
P 6450 2300
F 0 "JP6" V 6404 2368 50  0000 L CNN
F 1 "ju" V 6495 2368 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 6450 2300 50  0001 C CNN
F 3 "~" H 6450 2300 50  0001 C CNN
	1    6450 2300
	0    1    1    0   
$EndComp
Text Label 5150 2800 2    50   ~ 0
out1
$Comp
L Device:R R1
U 1 1 61DE4302
P 3700 3500
F 0 "R1" H 3500 3550 50  0000 L CNN
F 1 "100k" H 3450 3450 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3630 3500 50  0001 C CNN
F 3 "~" H 3700 3500 50  0001 C CNN
	1    3700 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3350 3700 3100
Wire Wire Line
	3700 3650 3700 4300
Wire Wire Line
	4450 4300 4450 4200
$Comp
L power:GND #PWR02
U 1 1 61DE54F7
P 4450 4400
F 0 "#PWR02" H 4450 4150 50  0001 C CNN
F 1 "GND" H 4455 4227 50  0000 C CNN
F 2 "" H 4450 4400 50  0001 C CNN
F 3 "" H 4450 4400 50  0001 C CNN
	1    4450 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 4400 4450 4300
Connection ~ 4450 4300
Wire Wire Line
	6450 2150 6250 2150
Connection ~ 5450 2150
Connection ~ 5650 2150
Wire Wire Line
	5650 2150 5450 2150
Connection ~ 5850 2150
Wire Wire Line
	5850 2150 5650 2150
Connection ~ 6050 2150
Wire Wire Line
	6050 2150 5850 2150
Connection ~ 6250 2150
Wire Wire Line
	6250 2150 6050 2150
Connection ~ 3700 3100
$Comp
L power:+5V #PWR01
U 1 1 61DE72C7
P 4450 2400
F 0 "#PWR01" H 4450 2250 50  0001 C CNN
F 1 "+5V" H 4465 2573 50  0000 C CNN
F 2 "" H 4450 2400 50  0001 C CNN
F 3 "" H 4450 2400 50  0001 C CNN
	1    4450 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2500 4450 2400
Wire Wire Line
	3950 2900 3900 2900
Wire Wire Line
	3950 2800 3350 2800
Text Label 3350 2800 0    50   ~ 0
rec
$Comp
L 4xxx:4017 U1
U 1 1 61DD4530
P 4450 3300
F 0 "U1" H 4650 4150 50  0000 C CNN
F 1 "4017" H 4650 4050 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 4450 3300 50  0001 C CNN
F 3 "http://www.intersil.com/content/dam/Intersil/documents/cd40/cd4017bms-22bms.pdf" H 4450 3300 50  0001 C CNN
	1    4450 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 4300 3900 4300
Wire Wire Line
	3700 3100 3950 3100
Wire Wire Line
	3900 2900 3900 4300
Connection ~ 3900 4300
Wire Wire Line
	3900 4300 4450 4300
NoConn ~ 4950 3900
$Comp
L Jumper:SolderJumper_2_Open JP7
U 1 1 61DF6105
P 6650 2300
F 0 "JP7" V 6604 2368 50  0000 L CNN
F 1 "ju" V 6695 2368 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 6650 2300 50  0001 C CNN
F 3 "~" H 6650 2300 50  0001 C CNN
	1    6650 2300
	0    1    1    0   
$EndComp
Wire Wire Line
	6650 2150 6450 2150
$Comp
L Jumper:SolderJumper_2_Open JP8
U 1 1 61E0342F
P 6850 2300
F 0 "JP8" V 6804 2368 50  0000 L CNN
F 1 "ju" V 6895 2368 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 6850 2300 50  0001 C CNN
F 3 "~" H 6850 2300 50  0001 C CNN
	1    6850 2300
	0    1    1    0   
$EndComp
Wire Wire Line
	6850 2150 6650 2150
Connection ~ 6650 2150
Wire Wire Line
	3700 2150 3700 3100
Wire Wire Line
	3700 2150 5450 2150
Wire Wire Line
	6650 3600 7150 3600
Wire Wire Line
	6450 3500 7150 3500
Wire Wire Line
	6250 3400 7150 3400
Wire Wire Line
	6050 3300 7150 3300
Wire Wire Line
	5850 3200 7150 3200
Wire Wire Line
	5650 3100 7150 3100
Wire Wire Line
	5450 3000 7150 3000
Wire Wire Line
	4950 2900 7150 2900
Text Label 5150 2900 2    50   ~ 0
out2
Text Label 5150 3000 2    50   ~ 0
out3
Text Label 5150 3100 2    50   ~ 0
out4
Text Label 5150 3200 2    50   ~ 0
out5
Text Label 5150 3300 2    50   ~ 0
out6
Text Label 5150 3400 2    50   ~ 0
out7
Text Label 5150 3500 2    50   ~ 0
out8
Text Label 5150 3600 2    50   ~ 0
out9
Wire Wire Line
	4950 3600 6650 3600
Connection ~ 6650 3600
Wire Wire Line
	4950 3500 6450 3500
Wire Wire Line
	4950 3400 6250 3400
Wire Wire Line
	4950 3300 6050 3300
Wire Wire Line
	4950 3200 5850 3200
Wire Wire Line
	4950 3100 5650 3100
Wire Wire Line
	4950 3000 5450 3000
Connection ~ 6450 3500
Connection ~ 6250 3400
Connection ~ 6050 3300
Connection ~ 5850 3200
Connection ~ 5650 3100
Connection ~ 5450 3000
Wire Wire Line
	5450 2450 5450 3000
Wire Wire Line
	5650 2450 5650 3100
Wire Wire Line
	5850 2450 5850 3200
Wire Wire Line
	6050 2450 6050 3300
Wire Wire Line
	6250 2450 6250 3400
Wire Wire Line
	6450 2450 6450 3500
Wire Wire Line
	6650 2450 6650 3600
Wire Wire Line
	6850 2450 6850 3700
Wire Wire Line
	4950 2800 7150 2800
Text Label 5200 3700 2    50   ~ 0
out10
Wire Wire Line
	4950 3700 6850 3700
Connection ~ 6850 3700
Wire Wire Line
	6850 3700 7150 3700
$Comp
L Connector:Screw_Terminal_01x05 J2
U 1 1 61DEE04E
P 7350 3000
F 0 "J2" H 7430 3042 50  0000 L CNN
F 1 "Screw_Terminal_01x10" H 7430 2951 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-5-5.08_1x05_P5.08mm_Horizontal" H 7350 3000 50  0001 C CNN
F 3 "~" H 7350 3000 50  0001 C CNN
	1    7350 3000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x03 J1
U 1 1 61DF3257
P 2450 2850
F 0 "J1" H 2368 2525 50  0000 C CNN
F 1 "screw" H 2368 2616 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-3-5.08_1x03_P5.08mm_Horizontal" H 2450 2850 50  0001 C CNN
F 3 "~" H 2450 2850 50  0001 C CNN
	1    2450 2850
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 61DF5E9A
P 2800 3000
F 0 "#PWR0101" H 2800 2750 50  0001 C CNN
F 1 "GND" H 2805 2827 50  0000 C CNN
F 2 "" H 2800 3000 50  0001 C CNN
F 3 "" H 2800 3000 50  0001 C CNN
	1    2800 3000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 61DF62C0
P 2900 2800
F 0 "#PWR0102" H 2900 2650 50  0001 C CNN
F 1 "+5V" H 2915 2973 50  0000 C CNN
F 2 "" H 2900 2800 50  0001 C CNN
F 3 "" H 2900 2800 50  0001 C CNN
	1    2900 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 3000 2800 2950
Wire Wire Line
	2800 2950 2650 2950
Wire Wire Line
	2900 2850 2900 2800
Wire Wire Line
	2650 2850 2900 2850
Text Label 2800 2750 2    50   ~ 0
rec
Wire Wire Line
	2800 2750 2650 2750
$Comp
L Connector:Screw_Terminal_01x05 J3
U 1 1 61E2131A
P 7350 3500
F 0 "J3" H 7430 3542 50  0000 L CNN
F 1 "Screw_Terminal_01x10" H 7430 3451 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-5-5.08_1x05_P5.08mm_Horizontal" H 7350 3500 50  0001 C CNN
F 3 "~" H 7350 3500 50  0001 C CNN
	1    7350 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 61E25771
P 3000 3750
F 0 "C1" H 3115 3796 50  0000 L CNN
F 1 "C" H 3115 3705 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 3038 3600 50  0001 C CNN
F 3 "~" H 3000 3750 50  0001 C CNN
	1    3000 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 61E25A95
P 3000 3950
F 0 "#PWR0103" H 3000 3700 50  0001 C CNN
F 1 "GND" H 3005 3777 50  0000 C CNN
F 2 "" H 3000 3950 50  0001 C CNN
F 3 "" H 3000 3950 50  0001 C CNN
	1    3000 3950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0104
U 1 1 61E25F88
P 3000 3550
F 0 "#PWR0104" H 3000 3400 50  0001 C CNN
F 1 "+5V" H 3015 3723 50  0000 C CNN
F 2 "" H 3000 3550 50  0001 C CNN
F 3 "" H 3000 3550 50  0001 C CNN
	1    3000 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 3600 3000 3550
Wire Wire Line
	3000 3900 3000 3950
$EndSCHEMATC
