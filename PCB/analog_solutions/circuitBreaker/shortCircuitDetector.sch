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
L Connector:Screw_Terminal_01x02 J3
U 1 1 6188583A
P 4200 3750
F 0 "J3" H 4118 3425 50  0000 C CNN
F 1 "AC IN" H 4100 3500 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal" H 4200 3750 50  0001 C CNN
F 3 "~" H 4200 3750 50  0001 C CNN
	1    4200 3750
	-1   0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J4
U 1 1 61885F72
P 6050 3750
F 0 "J4" H 5950 4050 50  0000 C CNN
F 1 "AC_OUT" H 6000 3900 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal" H 6050 3750 50  0001 C CNN
F 3 "~" H 6050 3750 50  0001 C CNN
	1    6050 3750
	1    0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 618A4192
P 5000 4850
F 0 "R1" H 4850 4850 50  0000 L CNN
F 1 "1R" H 4850 4750 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4930 4850 50  0001 C CNN
F 3 "~" H 5000 4850 50  0001 C CNN
	1    5000 4850
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:IRF540N Q3
U 1 1 618A974E
P 5100 4400
F 0 "Q3" H 5304 4446 50  0000 L CNN
F 1 "IRF540N" V 5350 4050 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 5350 4325 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf540n.pdf" H 5100 4400 50  0001 L CNN
	1    5100 4400
	-1   0    0    -1  
$EndComp
Text Notes 5850 3800 0    50   ~ 0
~
Text Notes 5600 3850 0    50   ~ 0
AC
Text Notes 4500 3850 0    50   ~ 0
AC
Wire Wire Line
	5000 5100 5000 5050
$Comp
L power:VCC #PWR05
U 1 1 618EA916
P 5550 2550
F 0 "#PWR05" H 5550 2400 50  0001 C CNN
F 1 "VCC" H 5565 2723 50  0000 C CNN
F 2 "" H 5550 2550 50  0001 C CNN
F 3 "" H 5550 2550 50  0001 C CNN
	1    5550 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 2550 6850 2550
Wire Wire Line
	6150 2550 6250 2550
Wire Wire Line
	6550 3000 6550 2850
$Comp
L power:GND #PWR07
U 1 1 618EA920
P 6550 3000
F 0 "#PWR07" H 6550 2750 50  0001 C CNN
F 1 "GND" H 6555 2827 50  0000 C CNN
F 2 "" H 6550 3000 50  0001 C CNN
F 3 "" H 6550 3000 50  0001 C CNN
	1    6550 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 2850 7000 2850
Connection ~ 6550 2850
Wire Wire Line
	6150 2850 6550 2850
$Comp
L Device:C C7
U 1 1 618EA929
P 7000 2700
F 0 "C7" H 7115 2746 50  0000 L CNN
F 1 "C" H 7115 2655 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 7038 2550 50  0001 C CNN
F 3 "~" H 7000 2700 50  0001 C CNN
	1    7000 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 618EA92F
P 6150 2700
F 0 "C6" H 6265 2746 50  0000 L CNN
F 1 "C" H 6265 2655 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 6188 2550 50  0001 C CNN
F 3 "~" H 6150 2700 50  0001 C CNN
	1    6150 2700
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:L78L05_TO92 U4
U 1 1 618EA935
P 6550 2550
F 0 "U4" H 6550 2792 50  0000 C CNN
F 1 "L78L05_TO92" H 6550 2701 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 6550 2775 50  0001 C CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/15/55/e5/aa/23/5b/43/fd/CD00000446.pdf/files/CD00000446.pdf/jcr:content/translations/en.CD00000446.pdf" H 6550 2500 50  0001 C CNN
	1    6550 2550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR010
U 1 1 618EA93B
P 7600 2550
F 0 "#PWR010" H 7600 2400 50  0001 C CNN
F 1 "+5V" H 7615 2723 50  0000 C CNN
F 2 "" H 7600 2550 50  0001 C CNN
F 3 "" H 7600 2550 50  0001 C CNN
	1    7600 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 618F89FF
P 5800 4400
F 0 "R5" V 5593 4400 50  0000 C CNN
F 1 "22R" V 5684 4400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5730 4400 50  0001 C CNN
F 3 "~" H 5800 4400 50  0001 C CNN
	1    5800 4400
	0    1    1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 618F8FEF
P 5550 4900
F 0 "R9" H 5480 4854 50  0000 R CNN
F 1 "10k" H 5480 4945 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5480 4900 50  0001 C CNN
F 3 "~" H 5550 4900 50  0001 C CNN
	1    5550 4900
	-1   0    0    1   
$EndComp
$Comp
L Device:R_POT RV1
U 1 1 61913F2D
P 9050 3900
F 0 "RV1" H 9250 3800 50  0000 R CNN
F 1 "R_POT" H 9300 3700 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Runtron_RM-065_Vertical" H 9050 3900 50  0001 C CNN
F 3 "~" H 9050 3900 50  0001 C CNN
	1    9050 3900
	1    0    0    1   
$EndComp
$Comp
L power:+5V #PWR013
U 1 1 6192638C
P 9050 3700
F 0 "#PWR013" H 9050 3550 50  0001 C CNN
F 1 "+5V" H 9065 3873 50  0000 C CNN
F 2 "" H 9050 3700 50  0001 C CNN
F 3 "" H 9050 3700 50  0001 C CNN
	1    9050 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 61926A54
P 9050 4250
F 0 "#PWR014" H 9050 4000 50  0001 C CNN
F 1 "GND" H 9055 4077 50  0000 C CNN
F 2 "" H 9050 4250 50  0001 C CNN
F 3 "" H 9050 4250 50  0001 C CNN
	1    9050 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 4200 9050 4250
Wire Wire Line
	9050 4200 9050 4050
Connection ~ 9050 4200
Wire Wire Line
	9050 3750 9050 3700
Wire Wire Line
	9200 3900 9450 3900
Text Label 9550 3900 0    50   ~ 0
adjust
$Comp
L power:GND #PWR04
U 1 1 6194E770
P 5550 5100
F 0 "#PWR04" H 5550 4850 50  0001 C CNN
F 1 "GND" H 5555 4927 50  0000 C CNN
F 2 "" H 5550 5100 50  0001 C CNN
F 3 "" H 5550 5100 50  0001 C CNN
	1    5550 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 6194EDEF
P 5000 5100
F 0 "#PWR02" H 5000 4850 50  0001 C CNN
F 1 "GND" H 5005 4927 50  0000 C CNN
F 2 "" H 5000 5100 50  0001 C CNN
F 3 "" H 5000 5100 50  0001 C CNN
	1    5000 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 5050 4650 5050
Connection ~ 5000 5050
Wire Wire Line
	5000 5050 5000 5000
Wire Wire Line
	4650 4150 4650 5050
Wire Wire Line
	4650 3750 4650 3850
Wire Wire Line
	4950 3750 5000 3750
$Comp
L Device:D D3
U 1 1 61892FC2
P 4650 4000
F 0 "D3" V 4604 4080 50  0000 L CNN
F 1 "D" V 4695 4080 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 4650 4000 50  0001 C CNN
F 3 "~" H 4650 4000 50  0001 C CNN
	1    4650 4000
	0    1    1    0   
$EndComp
$Comp
L Device:D D6
U 1 1 6189270E
P 4800 3750
F 0 "D6" H 4800 3533 50  0000 C CNN
F 1 "D" H 4800 3624 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 4800 3750 50  0001 C CNN
F 3 "~" H 4800 3750 50  0001 C CNN
	1    4800 3750
	-1   0    0    1   
$EndComp
$Comp
L MCU_Microchip_ATtiny:ATtiny45-20PU U6
U 1 1 61980D0F
P 7800 3950
F 0 "U6" H 8900 3450 50  0000 R CNN
F 1 "ATtiny45-20PU" H 9100 3350 50  0000 R CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 7800 3950 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf" H 7800 3950 50  0001 C CNN
	1    7800 3950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7800 3300 7800 3350
Wire Wire Line
	7800 4700 7800 4550
$Comp
L power:GND #PWR09
U 1 1 61980D62
P 7800 4700
F 0 "#PWR09" H 7800 4450 50  0001 C CNN
F 1 "GND" H 7805 4527 50  0000 C CNN
F 2 "" H 7800 4700 50  0001 C CNN
F 3 "" H 7800 4700 50  0001 C CNN
	1    7800 4700
	1    0    0    -1  
$EndComp
Text Label 6850 3850 0    50   ~ 0
currSens
Text Label 6850 3750 0    50   ~ 0
on_off
Wire Wire Line
	7200 3950 6850 3950
Text Label 6850 3950 0    50   ~ 0
adjust
$Comp
L Device:R R6
U 1 1 61980D75
P 6550 3800
F 0 "R6" V 6343 3800 50  0000 C CNN
F 1 "R" V 6434 3800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 6480 3800 50  0001 C CNN
F 3 "~" H 6550 3800 50  0001 C CNN
	1    6550 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 61980D7D
P 6550 3550
F 0 "#PWR06" H 6550 3400 50  0001 C CNN
F 1 "+5V" H 6565 3723 50  0000 C CNN
F 2 "" H 6550 3550 50  0001 C CNN
F 3 "" H 6550 3550 50  0001 C CNN
	1    6550 3550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR08
U 1 1 61980D83
P 7800 3300
F 0 "#PWR08" H 7800 3150 50  0001 C CNN
F 1 "+5V" H 7815 3473 50  0000 C CNN
F 2 "" H 7800 3300 50  0001 C CNN
F 3 "" H 7800 3300 50  0001 C CNN
	1    7800 3300
	1    0    0    -1  
$EndComp
NoConn ~ 7200 4050
Wire Wire Line
	6850 3750 7200 3750
Wire Wire Line
	7200 3850 6850 3850
Wire Wire Line
	6550 3550 6550 3650
Wire Wire Line
	6550 4150 6550 3950
Wire Wire Line
	6550 4150 7200 4150
Wire Wire Line
	5000 3750 5000 4200
$Comp
L Device:D D7
U 1 1 61C4A741
P 5250 3750
F 0 "D7" H 5250 3533 50  0000 C CNN
F 1 "D" H 5250 3624 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 5250 3750 50  0001 C CNN
F 3 "~" H 5250 3750 50  0001 C CNN
	1    5250 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3750 5000 3750
Connection ~ 5000 3750
$Comp
L Device:D D9
U 1 1 61C53120
P 5400 4000
F 0 "D9" V 5354 4080 50  0000 L CNN
F 1 "D" V 5445 4080 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 5400 4000 50  0001 C CNN
F 3 "~" H 5400 4000 50  0001 C CNN
	1    5400 4000
	0    1    1    0   
$EndComp
Wire Wire Line
	5000 4600 5000 4650
Wire Wire Line
	5400 3850 5400 3750
Connection ~ 5400 3750
Wire Wire Line
	5000 5050 5250 5050
Wire Wire Line
	5400 5050 5400 4150
Connection ~ 5000 4650
Wire Wire Line
	5000 4650 5000 4700
Text Label 6200 4650 2    50   ~ 0
currSens
Wire Wire Line
	5000 4650 5250 4650
Wire Wire Line
	5300 4400 5550 4400
Wire Wire Line
	5550 4750 5550 4400
Connection ~ 5550 4400
Wire Wire Line
	5550 4400 5650 4400
Wire Wire Line
	5550 5100 5550 5050
Text Label 6200 4400 2    50   ~ 0
on_off
Wire Wire Line
	6200 4400 5950 4400
Wire Wire Line
	5400 3500 5400 3650
$Comp
L Device:D D8
U 1 1 61CCB67A
P 5400 3350
F 0 "D8" V 5350 3150 50  0000 C CNN
F 1 "D" V 5450 3200 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 5400 3350 50  0001 C CNN
F 3 "~" H 5400 3350 50  0001 C CNN
	1    5400 3350
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR03
U 1 1 61CF8C61
P 5400 3100
F 0 "#PWR03" H 5400 2950 50  0001 C CNN
F 1 "VCC" H 5415 3273 50  0000 C CNN
F 2 "" H 5400 3100 50  0001 C CNN
F 3 "" H 5400 3100 50  0001 C CNN
	1    5400 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3750 4650 3750
Connection ~ 4650 3750
Wire Wire Line
	4400 3650 5400 3650
Wire Wire Line
	5400 3650 5850 3650
Connection ~ 5400 3650
$Comp
L Device:C C10
U 1 1 61D7038D
P 9450 4050
F 0 "C10" H 9565 4096 50  0000 L CNN
F 1 "C" H 9565 4005 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 9488 3900 50  0001 C CNN
F 3 "~" H 9450 4050 50  0001 C CNN
	1    9450 4050
	-1   0    0    1   
$EndComp
Connection ~ 9450 3900
Wire Wire Line
	9450 3900 9550 3900
Wire Wire Line
	9450 4200 9050 4200
$Comp
L Device:C C9
U 1 1 61D79AE0
P 8650 3950
F 0 "C9" H 8765 3996 50  0000 L CNN
F 1 "C" H 8765 3905 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 8688 3800 50  0001 C CNN
F 3 "~" H 8650 3950 50  0001 C CNN
	1    8650 3950
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR011
U 1 1 61D7A043
P 8650 3750
F 0 "#PWR011" H 8650 3600 50  0001 C CNN
F 1 "+5V" H 8665 3923 50  0000 C CNN
F 2 "" H 8650 3750 50  0001 C CNN
F 3 "" H 8650 3750 50  0001 C CNN
	1    8650 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 61D7A699
P 8650 4150
F 0 "#PWR012" H 8650 3900 50  0001 C CNN
F 1 "GND" H 8655 3977 50  0000 C CNN
F 2 "" H 8650 4150 50  0001 C CNN
F 3 "" H 8650 4150 50  0001 C CNN
	1    8650 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 4150 8650 4100
Wire Wire Line
	8650 3800 8650 3750
Wire Wire Line
	5400 3750 5850 3750
Wire Wire Line
	5400 3100 5400 3200
$Comp
L Device:CP C5
U 1 1 61DC81CC
P 5750 2700
F 0 "C5" H 5868 2746 50  0000 L CNN
F 1 "CP" H 5868 2655 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 5788 2550 50  0001 C CNN
F 3 "~" H 5750 2700 50  0001 C CNN
	1    5750 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C8
U 1 1 61DC8E54
P 7350 2700
F 0 "C8" H 7468 2746 50  0000 L CNN
F 1 "CP" H 7468 2655 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 7388 2550 50  0001 C CNN
F 3 "~" H 7350 2700 50  0001 C CNN
	1    7350 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2550 5750 2550
Connection ~ 6150 2550
Connection ~ 5750 2550
Wire Wire Line
	5750 2550 6150 2550
Wire Wire Line
	5750 2850 6150 2850
Connection ~ 6150 2850
Wire Wire Line
	7000 2850 7350 2850
Connection ~ 7000 2850
Wire Wire Line
	7600 2550 7350 2550
Connection ~ 7000 2550
Connection ~ 7350 2550
Wire Wire Line
	7350 2550 7000 2550
$Comp
L Device:R R2
U 1 1 618AD17B
P 5250 4850
F 0 "R2" H 5400 4850 50  0000 R CNN
F 1 "1R" H 5400 4950 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5180 4850 50  0001 C CNN
F 3 "~" H 5250 4850 50  0001 C CNN
	1    5250 4850
	-1   0    0    1   
$EndComp
Wire Wire Line
	5250 5000 5250 5050
Connection ~ 5250 5050
Wire Wire Line
	5250 5050 5400 5050
Wire Wire Line
	5250 4700 5250 4650
Connection ~ 5250 4650
Wire Wire Line
	5250 4650 6200 4650
$Comp
L Device:LED D1
U 1 1 61962C34
P 6600 5350
F 0 "D1" V 6639 5232 50  0000 R CNN
F 1 "LED" V 6548 5232 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 6600 5350 50  0001 C CNN
F 3 "~" H 6600 5350 50  0001 C CNN
	1    6600 5350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6600 5650 6600 5500
$Comp
L power:GND #PWR0101
U 1 1 61964D74
P 6600 5650
F 0 "#PWR0101" H 6600 5400 50  0001 C CNN
F 1 "GND" H 6605 5477 50  0000 C CNN
F 2 "" H 6600 5650 50  0001 C CNN
F 3 "" H 6600 5650 50  0001 C CNN
	1    6600 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 61965B89
P 6600 5000
F 0 "R3" V 6393 5000 50  0000 C CNN
F 1 "R" V 6484 5000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 6530 5000 50  0001 C CNN
F 3 "~" H 6600 5000 50  0001 C CNN
	1    6600 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 5200 6600 5150
Wire Wire Line
	6600 4850 6600 4800
Text Label 6600 4800 0    50   ~ 0
led
Text Label 6850 3650 0    50   ~ 0
led
Wire Wire Line
	6850 3650 7200 3650
NoConn ~ 8100 2400
$EndSCHEMATC