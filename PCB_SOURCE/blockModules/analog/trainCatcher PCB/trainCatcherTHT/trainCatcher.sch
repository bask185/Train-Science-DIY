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
L Device:R R2
U 1 1 5FF8AAA3
P 5550 4800
F 0 "R2" V 5750 4800 50  0000 C CNN
F 1 "R" V 5666 4800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5480 4800 50  0001 C CNN
F 3 "~" H 5550 4800 50  0001 C CNN
	1    5550 4800
	0    1    -1   0   
$EndComp
$Comp
L Device:R R3
U 1 1 5FF8B74D
P 5550 4900
F 0 "R3" V 5650 4900 50  0000 C CNN
F 1 "R" V 5750 4900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5480 4900 50  0001 C CNN
F 3 "~" H 5550 4900 50  0001 C CNN
	1    5550 4900
	0    -1   1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 5FFBE662
P 7000 4450
F 0 "R6" H 7070 4496 50  0000 L CNN
F 1 "100k" H 7070 4405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6930 4450 50  0001 C CNN
F 3 "~" H 7000 4450 50  0001 C CNN
	1    7000 4450
	1    0    0    1   
$EndComp
Wire Wire Line
	7100 4600 7000 4600
Wire Wire Line
	7100 5100 7000 5100
$Comp
L Device:R R5
U 1 1 5FFBF2B7
P 7000 5250
F 0 "R5" H 7070 5296 50  0000 L CNN
F 1 "100k" H 7070 5205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6930 5250 50  0001 C CNN
F 3 "~" H 7000 5250 50  0001 C CNN
	1    7000 5250
	1    0    0    1   
$EndComp
Connection ~ 7000 5100
Wire Wire Line
	2250 5050 2250 4900
Wire Wire Line
	6750 5650 7000 5650
$Comp
L Device:D_Bridge_+-AA D1
U 1 1 5FF89C63
P 1600 3150
F 0 "D1" H 1850 3350 50  0000 L CNN
F 1 "D" H 1800 3400 50  0000 L CNN
F 2 "Diode_THT:Diode_Bridge_DIP-4_W7.62mm_P5.08mm" H 1600 3150 50  0001 C CNN
F 3 "~" H 1600 3150 50  0001 C CNN
	1    1600 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 6250 2250 6400
$Comp
L Regulator_Linear:MC78L05_TO92 U1
U 1 1 6008C147
P 3050 3150
F 0 "U1" H 3050 3392 50  0000 C CNN
F 1 "MC78L05_TO92" H 3050 3301 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 3050 3375 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM78L05A.pdf" H 3050 3100 50  0001 C CNN
	1    3050 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 600BB0F2
P 6000 1800
F 0 "#PWR0101" H 6000 1550 50  0001 C CNN
F 1 "GND" H 6005 1627 50  0000 C CNN
F 2 "" H 6000 1800 50  0001 C CNN
F 3 "" H 6000 1800 50  0001 C CNN
	1    6000 1800
	-1   0    0    -1  
$EndComp
$Comp
L Device:R_POT RV1
U 1 1 5FFA737A
P 3900 1750
F 0 "RV1" V 3693 1750 50  0000 C CNN
F 1 "R_POT" V 3784 1750 50  0000 C CNN
F 2 "Potentiometer_THT:Potentiometer_Runtron_RM-065_Vertical" H 3900 1750 50  0001 C CNN
F 3 "~" H 3900 1750 50  0001 C CNN
	1    3900 1750
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 5FFA8689
P 3900 1450
F 0 "#PWR0102" H 3900 1300 50  0001 C CNN
F 1 "+5V" H 3915 1623 50  0000 C CNN
F 2 "" H 3900 1450 50  0001 C CNN
F 3 "" H 3900 1450 50  0001 C CNN
	1    3900 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5FFA905D
P 3900 2000
F 0 "#PWR0103" H 3900 1750 50  0001 C CNN
F 1 "GND" H 3905 1827 50  0000 C CNN
F 2 "" H 3900 2000 50  0001 C CNN
F 3 "" H 3900 2000 50  0001 C CNN
	1    3900 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 1500 3900 1600
Wire Wire Line
	3900 1900 3900 2000
$Comp
L power:+5V #PWR0104
U 1 1 5FFA9D61
P 2250 4900
F 0 "#PWR0104" H 2250 4750 50  0001 C CNN
F 1 "+5V" H 2265 5073 50  0000 C CNN
F 2 "" H 2250 4900 50  0001 C CNN
F 3 "" H 2250 4900 50  0001 C CNN
	1    2250 4900
	1    0    0    -1  
$EndComp
Connection ~ 7000 5650
Wire Wire Line
	7000 5650 7400 5650
Wire Wire Line
	7000 5650 7000 5400
$Comp
L Device:D_Schottky D4
U 1 1 5FFE6E0C
P 7900 5100
F 0 "D4" V 7946 5020 50  0000 R CNN
F 1 "D_Schottky" V 7855 5020 50  0000 R CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 7900 5100 50  0001 C CNN
F 3 "~" H 7900 5100 50  0001 C CNN
	1    7900 5100
	0    -1   1    0   
$EndComp
$Comp
L Device:D_Schottky D5
U 1 1 5FFE8ADF
P 7900 4550
F 0 "D5" V 7854 4630 50  0000 L CNN
F 1 "D_Schottky" V 7945 4630 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 7900 4550 50  0001 C CNN
F 3 "~" H 7900 4550 50  0001 C CNN
	1    7900 4550
	0    1    -1   0   
$EndComp
$Comp
L power:+5V #PWR0105
U 1 1 600EF3DB
P 6000 1700
F 0 "#PWR0105" H 6000 1550 50  0001 C CNN
F 1 "+5V" H 6050 1900 50  0000 C CNN
F 2 "" H 6000 1700 50  0001 C CNN
F 3 "" H 6000 1700 50  0001 C CNN
	1    6000 1700
	-1   0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATtiny:ATtiny13-20PU U2
U 1 1 6015F403
P 2250 5650
F 0 "U2" H 1721 5696 50  0000 R CNN
F 1 "ATtiny13-20PU" H 1721 5605 50  0000 R CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 2250 5650 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc2535.pdf" H 2250 5650 50  0001 C CNN
	1    2250 5650
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:IRF540N Q1
U 1 1 601642F8
P 7300 5100
F 0 "Q1" H 7504 5054 50  0000 L CNN
F 1 "IRF540N" H 7504 5145 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 7550 5025 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf540n.pdf" H 7300 5100 50  0001 L CNN
	1    7300 5100
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:IRF540N Q2
U 1 1 6016927C
P 7300 4600
F 0 "Q2" H 7504 4646 50  0000 L CNN
F 1 "IRF540N" H 7504 4555 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 7550 4525 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf540n.pdf" H 7300 4600 50  0001 L CNN
	1    7300 4600
	1    0    0    1   
$EndComp
Text Label 1600 3550 0    50   ~ 0
rightTrack
Text Label 3300 5550 0    50   ~ 0
break
Wire Wire Line
	7400 5650 7400 5800
Wire Wire Line
	7000 4300 7000 4000
Connection ~ 7000 4600
Text Label 4950 4850 0    50   ~ 0
PWM
Text Label 3300 5450 0    50   ~ 0
PWM
Wire Wire Line
	2850 5450 3300 5450
Text Label 1600 2750 0    50   ~ 0
leftTrack
Wire Wire Line
	1600 2850 1600 2750
Wire Wire Line
	1600 3550 1600 3450
$Comp
L power:GND #PWR0109
U 1 1 61EC07CB
P 3500 3700
F 0 "#PWR0109" H 3500 3450 50  0001 C CNN
F 1 "GND" H 3505 3527 50  0000 C CNN
F 2 "" H 3500 3700 50  0001 C CNN
F 3 "" H 3500 3700 50  0001 C CNN
	1    3500 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 61EC0B77
P 3050 3700
F 0 "#PWR0110" H 3050 3450 50  0001 C CNN
F 1 "GND" H 3055 3527 50  0000 C CNN
F 2 "" H 3050 3700 50  0001 C CNN
F 3 "" H 3050 3700 50  0001 C CNN
	1    3050 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 61EC0D40
P 2600 3700
F 0 "#PWR0111" H 2600 3450 50  0001 C CNN
F 1 "GND" H 2605 3527 50  0000 C CNN
F 2 "" H 2600 3700 50  0001 C CNN
F 3 "" H 2600 3700 50  0001 C CNN
	1    2600 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 61EC0F4B
P 1300 3700
F 0 "#PWR0112" H 1300 3450 50  0001 C CNN
F 1 "GND" H 1305 3527 50  0000 C CNN
F 2 "" H 1300 3700 50  0001 C CNN
F 3 "" H 1300 3700 50  0001 C CNN
	1    1300 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 3150 1300 3700
Wire Wire Line
	2600 3450 2600 3700
Wire Wire Line
	3050 3700 3050 3450
Wire Wire Line
	3500 3450 3500 3700
$Comp
L power:GND #PWR0113
U 1 1 61EDAAAA
P 2250 6400
F 0 "#PWR0113" H 2250 6150 50  0001 C CNN
F 1 "GND" H 2255 6227 50  0000 C CNN
F 2 "" H 2250 6400 50  0001 C CNN
F 3 "" H 2250 6400 50  0001 C CNN
	1    2250 6400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0114
U 1 1 61EE5E15
P 3950 3150
F 0 "#PWR0114" H 3950 3000 50  0001 C CNN
F 1 "+5V" H 3965 3323 50  0000 C CNN
F 2 "" H 3950 3150 50  0001 C CNN
F 3 "" H 3950 3150 50  0001 C CNN
	1    3950 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 61EFDE80
P 3700 1500
F 0 "C4" V 3952 1500 50  0000 C CNN
F 1 "C" V 3861 1500 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 3738 1350 50  0001 C CNN
F 3 "~" H 3700 1500 50  0001 C CNN
	1    3700 1500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3850 1500 3900 1500
Wire Wire Line
	3900 1500 3900 1450
Connection ~ 3900 1500
Wire Wire Line
	3750 1750 3550 1750
Wire Wire Line
	3550 1750 3550 1500
Wire Wire Line
	3550 1750 3200 1750
Connection ~ 3550 1750
Text Label 3200 1750 2    50   ~ 0
breakSpeed
Text Label 3300 5750 0    50   ~ 0
breakSpeed
Text Label 5750 2000 0    50   ~ 0
rec
Text Label 5750 1900 0    50   ~ 0
hold
Text Label 3300 5350 0    50   ~ 0
rec
Text Label 3300 5650 0    50   ~ 0
hold
Wire Wire Line
	2850 5350 3300 5350
Wire Wire Line
	2850 5650 3300 5650
Wire Wire Line
	2850 5550 3300 5550
Wire Wire Line
	2850 5750 3300 5750
Wire Wire Line
	7400 5650 7900 5650
Connection ~ 7400 5650
Wire Wire Line
	7900 5650 7900 5250
Wire Wire Line
	7400 5650 7400 5300
Wire Wire Line
	6700 4000 6850 4000
$Comp
L Device:CP C1
U 1 1 61DD87B6
P 2600 3300
F 0 "C1" H 2718 3346 50  0000 L CNN
F 1 "CP" H 2718 3255 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D4.0mm_P2.00mm" H 2638 3150 50  0001 C CNN
F 3 "~" H 2600 3300 50  0001 C CNN
	1    2600 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3150 2750 3150
$Comp
L Connector:Conn_01x04_Male J4
U 1 1 61E41EC1
P 8750 1000
F 0 "J4" V 8812 1144 50  0000 L CNN
F 1 "thru" V 8903 1144 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Horizontal" H 8750 1000 50  0001 C CNN
F 3 "~" H 8750 1000 50  0001 C CNN
	1    8750 1000
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x04_Female J5
U 1 1 61E43D24
P 8750 1850
F 0 "J5" V 8596 1998 50  0000 L CNN
F 1 "thru" V 8687 1998 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Horizontal" H 8750 1850 50  0001 C CNN
F 3 "~" H 8750 1850 50  0001 C CNN
	1    8750 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	8750 1650 8750 1300
Wire Wire Line
	8850 1650 8850 1550
Wire Wire Line
	8650 1650 8650 1550
Wire Wire Line
	8550 1650 8550 1300
Text Label 8450 1550 2    50   ~ 0
leftTrack
Text Label 8450 1300 2    50   ~ 0
rightTrack
Wire Wire Line
	8450 1300 8550 1300
Connection ~ 8550 1300
Wire Wire Line
	8550 1300 8550 1200
Wire Wire Line
	8450 1550 8650 1550
Connection ~ 8650 1550
Wire Wire Line
	8650 1550 8650 1200
$Comp
L power:GND #PWR0117
U 1 1 61E5C318
P 9200 1550
F 0 "#PWR0117" H 9200 1300 50  0001 C CNN
F 1 "GND" H 9205 1377 50  0000 C CNN
F 2 "" H 9200 1550 50  0001 C CNN
F 3 "" H 9200 1550 50  0001 C CNN
	1    9200 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 1550 9200 1550
Connection ~ 8850 1550
Wire Wire Line
	8850 1550 8850 1200
Text Label 9150 1300 0    50   ~ 0
rec
Wire Wire Line
	9150 1300 8750 1300
Connection ~ 8750 1300
Wire Wire Line
	8750 1300 8750 1200
Wire Wire Line
	2850 5850 3300 5850
Text Label 3300 5850 0    50   ~ 0
stop
Wire Wire Line
	3350 3150 3500 3150
$Comp
L Device:CP C2
U 1 1 61E501BE
P 3500 3300
F 0 "C2" H 3618 3346 50  0000 L CNN
F 1 "CP" H 3618 3255 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D4.0mm_P2.00mm" H 3538 3150 50  0001 C CNN
F 3 "~" H 3500 3300 50  0001 C CNN
	1    3500 3300
	1    0    0    -1  
$EndComp
Connection ~ 3500 3150
$Comp
L Device:C C6
U 1 1 61E5069E
P 3850 3300
F 0 "C6" V 4102 3300 50  0000 C CNN
F 1 "C" V 4011 3300 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 3888 3150 50  0001 C CNN
F 3 "~" H 3850 3300 50  0001 C CNN
	1    3850 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	3500 3150 3850 3150
Wire Wire Line
	3950 3150 3850 3150
Connection ~ 3850 3150
$Comp
L power:GND #PWR0118
U 1 1 61E6245B
P 3850 3700
F 0 "#PWR0118" H 3850 3450 50  0001 C CNN
F 1 "GND" H 3855 3527 50  0000 C CNN
F 2 "" H 3850 3700 50  0001 C CNN
F 3 "" H 3850 3700 50  0001 C CNN
	1    3850 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 3450 3850 3700
$Comp
L Device:C C5
U 1 1 61E781EA
P 2300 3300
F 0 "C5" V 2552 3300 50  0000 C CNN
F 1 "C" V 2461 3300 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 2338 3150 50  0001 C CNN
F 3 "~" H 2300 3300 50  0001 C CNN
	1    2300 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	1900 3150 2300 3150
Connection ~ 2600 3150
Connection ~ 2300 3150
Wire Wire Line
	2300 3150 2600 3150
$Comp
L power:GND #PWR0119
U 1 1 61E9043D
P 2300 3700
F 0 "#PWR0119" H 2300 3450 50  0001 C CNN
F 1 "GND" H 2305 3527 50  0000 C CNN
F 2 "" H 2300 3700 50  0001 C CNN
F 3 "" H 2300 3700 50  0001 C CNN
	1    2300 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 3450 2300 3700
Wire Wire Line
	6750 5650 6750 4800
Wire Wire Line
	5900 4600 5800 4600
Wire Wire Line
	5800 5100 5900 5100
Wire Wire Line
	4950 4850 5400 4850
Wire Wire Line
	5900 4800 5700 4800
Wire Wire Line
	5700 4900 5900 4900
$Comp
L power:GND #PWR0115
U 1 1 61F44452
P 5800 5200
F 0 "#PWR0115" H 5800 4950 50  0001 C CNN
F 1 "GND" H 5800 5050 50  0000 C CNN
F 2 "" H 5800 5200 50  0001 C CNN
F 3 "" H 5800 5200 50  0001 C CNN
	1    5800 5200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7400 4900 7400 4850
Wire Wire Line
	7900 4950 7900 4850
Wire Wire Line
	7400 4850 7900 4850
Connection ~ 7400 4850
Wire Wire Line
	7400 4850 7400 4800
Connection ~ 7900 4850
Wire Wire Line
	7900 4850 7900 4700
Wire Wire Line
	6500 4600 7000 4600
Wire Wire Line
	6500 5100 7000 5100
Wire Wire Line
	6500 4900 6700 4900
Wire Wire Line
	6500 4800 6750 4800
Wire Wire Line
	6700 4900 6700 4000
Wire Wire Line
	7400 4400 7400 4250
Wire Wire Line
	7900 4400 7900 4250
Wire Wire Line
	7400 4250 7900 4250
Wire Wire Line
	5400 4900 5400 4850
Connection ~ 5400 4850
Wire Wire Line
	5400 4850 5400 4800
$Comp
L power:GND #PWR0106
U 1 1 61DDC8CB
P 5300 3200
F 0 "#PWR0106" H 5300 2950 50  0001 C CNN
F 1 "GND" H 5300 3050 50  0000 C CNN
F 2 "" H 5300 3200 50  0001 C CNN
F 3 "" H 5300 3200 50  0001 C CNN
	1    5300 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 3400 5150 3400
Wire Wire Line
	6100 2950 6100 3100
Wire Wire Line
	6750 2950 6100 2950
Wire Wire Line
	6750 3700 6100 3700
Text Label 7450 2550 0    50   ~ 0
stopTrack
Text Label 7350 2550 2    50   ~ 0
breakSection
Connection ~ 6750 2950
Wire Wire Line
	7300 2950 6750 2950
Wire Wire Line
	7300 3150 7300 2950
Wire Wire Line
	6750 3700 6750 3650
Connection ~ 6750 3700
Wire Wire Line
	7300 3700 7300 3450
Wire Wire Line
	6750 3700 7300 3700
$Comp
L Device:R R1
U 1 1 61E00711
P 7300 3300
F 0 "R1" H 7150 3450 50  0000 L CNN
F 1 "4k7" H 7150 3100 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7230 3300 50  0001 C CNN
F 3 "~" H 7300 3300 50  0001 C CNN
	1    7300 3300
	1    0    0    1   
$EndComp
Wire Wire Line
	7100 3350 7050 3350
Wire Wire Line
	7100 3400 7100 3350
Wire Wire Line
	6400 3400 7100 3400
Wire Wire Line
	6400 3350 6400 3400
Wire Wire Line
	6450 3350 6400 3350
Wire Wire Line
	6750 3050 6750 2950
Text Label 5150 3400 2    50   ~ 0
break
Wire Wire Line
	5300 3200 5400 3200
$Comp
L Device:R R4
U 1 1 5FF9ED57
P 6100 3550
F 0 "R4" V 6307 3550 50  0000 C CNN
F 1 "33R" V 6216 3550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6030 3550 50  0001 C CNN
F 3 "~" H 6100 3550 50  0001 C CNN
	1    6100 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:D D3
U 1 1 5FF93030
P 6100 3250
F 0 "D3" V 6050 3350 50  0000 L CNN
F 1 "D" V 6150 3350 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 6100 3250 50  0001 C CNN
F 3 "~" H 6100 3250 50  0001 C CNN
	1    6100 3250
	0    1    -1   0   
$EndComp
$Comp
L Device:D_Bridge_+-AA D2
U 1 1 5FF95481
P 6750 3350
F 0 "D2" V 6800 3300 50  0000 L CNN
F 1 "D" H 6950 3050 50  0000 L CNN
F 2 "Diode_THT:Diode_Bridge_DIP-4_W7.62mm_P5.08mm" H 6750 3350 50  0001 C CNN
F 3 "~" H 6750 3350 50  0001 C CNN
	1    6750 3350
	1    0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J2
U 1 1 600242D8
P 7350 2300
F 0 "J2" H 7500 2300 50  0000 L CNN
F 1 "break" H 7450 2200 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal" H 7350 2300 50  0001 C CNN
F 3 "~" H 7350 2300 50  0001 C CNN
	1    7350 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6100 3400 6000 3400
Wire Wire Line
	8050 3700 8700 3700
Connection ~ 8050 2950
Wire Wire Line
	7500 2950 8050 2950
Wire Wire Line
	7500 3150 7500 2950
Wire Wire Line
	8050 3700 8050 3650
Connection ~ 8050 3700
Wire Wire Line
	7500 3700 7500 3450
Wire Wire Line
	8050 3700 7500 3700
$Comp
L Device:R R8
U 1 1 61DFE5D9
P 7500 3300
F 0 "R8" H 7400 3450 50  0000 L CNN
F 1 "4k7" H 7350 3150 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7430 3300 50  0001 C CNN
F 3 "~" H 7500 3300 50  0001 C CNN
	1    7500 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	7700 3350 7750 3350
Wire Wire Line
	7700 3400 7700 3350
Wire Wire Line
	8400 3400 7700 3400
Wire Wire Line
	8400 3350 8400 3400
Wire Wire Line
	8350 3350 8400 3350
Wire Wire Line
	8050 3050 8050 2950
$Comp
L Device:R R7
U 1 1 61DFE5BA
P 8700 3550
F 0 "R7" V 8907 3550 50  0000 C CNN
F 1 "33R" V 8816 3550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8630 3550 50  0001 C CNN
F 3 "~" H 8700 3550 50  0001 C CNN
	1    8700 3550
	-1   0    0    -1  
$EndComp
$Comp
L Device:D_Bridge_+-AA D7
U 1 1 61DFE5AA
P 8050 3350
F 0 "D7" V 8100 3300 50  0000 L CNN
F 1 "D" H 8250 3050 50  0000 L CNN
F 2 "Diode_THT:Diode_Bridge_DIP-4_W7.62mm_P5.08mm" H 8050 3350 50  0001 C CNN
F 3 "~" H 8050 3350 50  0001 C CNN
	1    8050 3350
	-1   0    0    1   
$EndComp
Wire Wire Line
	8700 3400 8800 3400
Text Label 9650 3400 0    50   ~ 0
stop
$Comp
L power:GND #PWR0107
U 1 1 61DFE5C9
P 9500 3200
F 0 "#PWR0107" H 9500 2950 50  0001 C CNN
F 1 "GND" H 9500 3050 50  0000 C CNN
F 2 "" H 9500 3200 50  0001 C CNN
F 3 "" H 9500 3200 50  0001 C CNN
	1    9500 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 3200 9400 3200
$Comp
L Device:D D6
U 1 1 61DFE5B0
P 8700 3250
F 0 "D6" V 8654 3330 50  0000 L CNN
F 1 "D" V 8745 3330 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P7.62mm_Horizontal" H 8700 3250 50  0001 C CNN
F 3 "~" H 8700 3250 50  0001 C CNN
	1    8700 3250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8050 2950 8050 2650
Wire Wire Line
	6750 2950 6750 2650
Wire Wire Line
	8800 2950 8700 2950
Wire Wire Line
	8800 3200 8800 2950
Wire Wire Line
	8700 3100 8700 2950
Connection ~ 8700 2950
Wire Wire Line
	8700 2950 8050 2950
Wire Wire Line
	6000 3200 6000 2950
Wire Wire Line
	6000 2950 6100 2950
Connection ~ 6100 2950
Connection ~ 7300 3700
Wire Wire Line
	7400 3700 7300 3700
Wire Wire Line
	7400 3700 7500 3700
Connection ~ 7400 3700
Connection ~ 7500 3700
Wire Wire Line
	4750 4000 4750 3950
Wire Wire Line
	4750 4200 4750 4100
Text Label 4750 4200 0    50   ~ 0
rightTrack
Text Label 4750 3950 0    50   ~ 0
leftTrack
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 6003BF6D
P 4550 4000
F 0 "J1" H 4468 3675 50  0000 C CNN
F 1 "Power" H 4600 3750 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal" H 4550 4000 50  0001 C CNN
F 3 "~" H 4550 4000 50  0001 C CNN
	1    4550 4000
	-1   0    0    -1  
$EndComp
Text Label 7400 5800 0    50   ~ 0
rightTrack
Wire Wire Line
	7350 2650 6750 2650
Wire Wire Line
	7450 2650 8050 2650
Connection ~ 8700 3400
Connection ~ 6100 3400
Wire Wire Line
	9400 3400 9650 3400
Wire Wire Line
	7350 2500 7350 2650
Wire Wire Line
	7450 2500 7450 2650
$Comp
L Device:C C3
U 1 1 621340D9
P 4100 5650
F 0 "C3" V 4352 5650 50  0000 C CNN
F 1 "C" V 4261 5650 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 4138 5500 50  0001 C CNN
F 3 "~" H 4100 5650 50  0001 C CNN
	1    4100 5650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0116
U 1 1 621381CC
P 4100 5400
F 0 "#PWR0116" H 4100 5250 50  0001 C CNN
F 1 "+5V" H 4115 5573 50  0000 C CNN
F 2 "" H 4100 5400 50  0001 C CNN
F 3 "" H 4100 5400 50  0001 C CNN
	1    4100 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0120
U 1 1 6213C058
P 4100 5900
F 0 "#PWR0120" H 4100 5650 50  0001 C CNN
F 1 "GND" H 4105 5727 50  0000 C CNN
F 2 "" H 4100 5900 50  0001 C CNN
F 3 "" H 4100 5900 50  0001 C CNN
	1    4100 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 5900 4100 5800
Wire Wire Line
	4100 5500 4100 5400
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 61DCB207
P 5450 1800
F 0 "J3" H 5422 1682 50  0000 R CNN
F 1 "Conn_01x04_Male" H 5422 1773 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5450 1800 50  0001 C CNN
F 3 "~" H 5450 1800 50  0001 C CNN
	1    5450 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 1900 5650 1900
Wire Wire Line
	5750 2000 5650 2000
Wire Wire Line
	5650 1800 6000 1800
Wire Wire Line
	5650 1700 6000 1700
Wire Wire Line
	5800 5100 5800 4600
Wire Wire Line
	5800 5200 5800 5100
Connection ~ 5800 5100
Text Label 6850 3900 2    50   ~ 0
leftTrack
Wire Wire Line
	6850 3900 6850 4000
Connection ~ 6850 4000
Wire Wire Line
	6850 4000 7000 4000
Connection ~ 7400 4250
Wire Wire Line
	7400 3700 7400 4250
$Comp
L Isolator:LTV-847 U7
U 1 1 62045CE4
P 5700 3300
F 0 "U7" H 5700 2983 50  0000 C CNN
F 1 "LTV-847" H 5700 3074 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 5500 3100 50  0001 L CIN
F 3 "http://optoelectronics.liteon.com/upload/download/DS-70-96-0016/LTV-8X7%20series.PDF" H 5700 3300 50  0001 L CNN
	1    5700 3300
	-1   0    0    1   
$EndComp
$Comp
L Isolator:LTV-847 U7
U 2 1 62047560
P 9100 3300
F 0 "U7" H 9100 3625 50  0000 C CNN
F 1 "LTV-847" H 9100 3534 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 8900 3100 50  0001 L CIN
F 3 "http://optoelectronics.liteon.com/upload/download/DS-70-96-0016/LTV-8X7%20series.PDF" H 9100 3300 50  0001 L CNN
	2    9100 3300
	1    0    0    1   
$EndComp
$Comp
L Isolator:LTV-847 U7
U 3 1 62048522
P 6200 4700
F 0 "U7" H 6200 4400 50  0000 C CNN
F 1 "LTV-847" H 6200 4500 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 6000 4500 50  0001 L CIN
F 3 "http://optoelectronics.liteon.com/upload/download/DS-70-96-0016/LTV-8X7%20series.PDF" H 6200 4700 50  0001 L CNN
	3    6200 4700
	1    0    0    1   
$EndComp
$Comp
L Isolator:LTV-847 U7
U 4 1 6204A478
P 6200 5000
F 0 "U7" H 6200 4700 50  0000 C CNN
F 1 "LTV-847" H 6200 4800 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 6000 4800 50  0001 L CIN
F 3 "http://optoelectronics.liteon.com/upload/download/DS-70-96-0016/LTV-8X7%20series.PDF" H 6200 5000 50  0001 L CNN
	4    6200 5000
	1    0    0    -1  
$EndComp
$EndSCHEMATC