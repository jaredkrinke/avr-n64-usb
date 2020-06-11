EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 7087 8661
encoding utf-8
Sheet 1 1
Title "avr-n64-usb"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATmega:ATmega32-16PU U?
U 1 1 5EE28C29
P 2500 3400
F 0 "U?" H 2500 1311 50  0001 C CNN
F 1 "ATmega32-16PU" H 2500 1311 50  0000 C CNN
F 2 "Package_DIP:DIP-40_W15.24mm" H 2500 3400 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc2503.pdf" H 2500 3400 50  0001 C CNN
	1    2500 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal 12MHz
U 1 1 5EE2C082
P 1500 2000
F 0 "12MHz" V 1454 2131 50  0000 L CNN
F 1 "Crystal" V 1545 2131 50  0000 L CNN
F 2 "" H 1500 2000 50  0001 C CNN
F 3 "~" H 1500 2000 50  0001 C CNN
	1    1500 2000
	0    1    1    0   
$EndComp
Wire Wire Line
	1500 1850 1900 1850
Wire Wire Line
	1900 1850 1900 1900
$Comp
L Device:C C?
U 1 1 5EE2E476
P 1100 1850
F 0 "C?" V 848 1850 50  0001 C CNN
F 1 "22pF" V 940 1850 50  0000 C CNN
F 2 "" H 1138 1700 50  0001 C CNN
F 3 "~" H 1100 1850 50  0001 C CNN
	1    1100 1850
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5EE2F5C1
P 1100 2200
F 0 "C?" V 848 2200 50  0001 C CNN
F 1 "22pF" V 940 2200 50  0000 C CNN
F 2 "" H 1138 2050 50  0001 C CNN
F 3 "~" H 1100 2200 50  0001 C CNN
	1    1100 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	1250 1850 1500 1850
Connection ~ 1500 1850
Wire Wire Line
	1900 2100 1900 2150
Wire Wire Line
	1900 2150 1500 2150
Wire Wire Line
	1500 2150 1500 2200
Wire Wire Line
	1500 2200 1250 2200
Connection ~ 1500 2150
Wire Wire Line
	2600 1400 2600 1250
Wire Wire Line
	2600 1250 2500 1250
Wire Wire Line
	2500 1400 2500 1250
Connection ~ 2500 1250
$Comp
L power:GND #PWR?
U 1 1 5EE34CB3
P 1950 6050
F 0 "#PWR?" H 1950 5800 50  0001 C CNN
F 1 "GND" H 1955 5877 50  0000 C CNN
F 2 "" H 1950 6050 50  0001 C CNN
F 3 "" H 1950 6050 50  0001 C CNN
	1    1950 6050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5EE35673
P 750 1400
F 0 "C?" H 865 1446 50  0001 L CNN
F 1 "100pF" H 865 1400 50  0000 L CNN
F 2 "" H 788 1250 50  0001 C CNN
F 3 "~" H 750 1400 50  0001 C CNN
	1    750  1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	750  1550 750  1850
Wire Wire Line
	950  1850 750  1850
Connection ~ 750  1850
Wire Wire Line
	750  1850 750  2200
Wire Wire Line
	950  2200 750  2200
Connection ~ 750  2200
Wire Wire Line
	750  2200 750  6000
$Comp
L Connector:USB_A J?
U 1 1 5EE372A5
P 3400 1050
F 0 "J?" H 3457 1517 50  0001 C CNN
F 1 "USB_A" H 3457 1425 50  0000 C CNN
F 2 "" H 3550 1000 50  0001 C CNN
F 3 " ~" H 3550 1000 50  0001 C CNN
	1    3400 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	750  1250 2500 1250
Connection ~ 2600 1250
Text Label 3750 800  0    50   ~ 0
5V
Wire Wire Line
	3700 600  2600 600 
Wire Wire Line
	2600 600  2600 1250
Wire Wire Line
	3700 600  3700 850 
$Comp
L power:GND #PWR?
U 1 1 5EE3B3C3
P 3400 1450
F 0 "#PWR?" H 3400 1200 50  0001 C CNN
F 1 "GND" H 3405 1277 50  0000 C CNN
F 2 "" H 3400 1450 50  0001 C CNN
F 3 "" H 3400 1450 50  0001 C CNN
	1    3400 1450
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LF33_TO220 U?
U 1 1 5EE3CA3B
P 4850 850
F 0 "U?" H 4850 1092 50  0001 C CNN
F 1 "3.3V Regulator" H 4850 1001 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4850 1075 50  0001 C CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/c4/0e/7e/2a/be/bc/4c/bd/CD00000546.pdf/files/CD00000546.pdf/jcr:content/translations/en.CD00000546.pdf" H 4850 800 50  0001 C CNN
	1    4850 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 850  4200 850 
Connection ~ 3700 850 
$Comp
L power:GND #PWR?
U 1 1 5EE401A0
P 4850 1150
F 0 "#PWR?" H 4850 900 50  0001 C CNN
F 1 "GND" H 4855 977 50  0000 C CNN
F 2 "" H 4850 1150 50  0001 C CNN
F 3 "" H 4850 1150 50  0001 C CNN
	1    4850 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 850  5500 850 
Text Label 5500 850  0    50   ~ 0
N64_VCC
Wire Wire Line
	4850 1150 5500 1150
Connection ~ 4850 1150
Text Notes 5500 1150 0    50   ~ 0
N64_GND
Text Notes 5500 1450 0    50   ~ 0
N64_DATA
$Comp
L Device:R R?
U 1 1 5EE43B00
P 3750 4600
F 0 "R?" V 3543 4600 50  0001 C CNN
F 1 "68" V 3635 4600 50  0000 C CNN
F 2 "" V 3680 4600 50  0001 C CNN
F 3 "~" H 3750 4600 50  0001 C CNN
	1    3750 4600
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5EE44426
P 3750 4900
F 0 "R?" V 3543 4900 50  0001 C CNN
F 1 "68" V 3635 4900 50  0000 C CNN
F 2 "" V 3680 4900 50  0001 C CNN
F 3 "~" H 3750 4900 50  0001 C CNN
	1    3750 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	3100 4600 3600 4600
Wire Wire Line
	3100 4800 3600 4800
Wire Wire Line
	3600 4800 3600 4900
$Comp
L Device:D_Zener D?
U 1 1 5EE4575E
P 4450 4600
F 0 "D?" H 4450 4817 50  0001 C CNN
F 1 "3.6V" H 4450 4725 50  0000 C CNN
F 2 "" H 4450 4600 50  0001 C CNN
F 3 "~" H 4450 4600 50  0001 C CNN
	1    4450 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Zener D?
U 1 1 5EE46D97
P 4900 4900
F 0 "D?" H 4900 5117 50  0001 C CNN
F 1 "3.6V" H 4900 5025 50  0000 C CNN
F 2 "" H 4900 4900 50  0001 C CNN
F 3 "~" H 4900 4900 50  0001 C CNN
	1    4900 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 4600 5050 4600
Wire Wire Line
	5050 4600 5050 4900
Connection ~ 5050 4900
Wire Wire Line
	3900 4900 4200 4900
Wire Wire Line
	3900 4600 4000 4600
Connection ~ 4000 4600
Wire Wire Line
	4000 4600 4300 4600
Connection ~ 4200 4900
Wire Wire Line
	4200 4900 4750 4900
Wire Wire Line
	4200 1150 3700 1150
$Comp
L Device:R R?
U 1 1 5EE4FBA2
P 4200 1000
F 0 "R?" H 4270 1046 50  0001 L CNN
F 1 "10k" H 4270 1000 50  0000 L CNN
F 2 "" V 4130 1000 50  0001 C CNN
F 3 "~" H 4200 1000 50  0001 C CNN
	1    4200 1000
	1    0    0    -1  
$EndComp
Connection ~ 4200 850 
Wire Wire Line
	4200 850  4550 850 
Connection ~ 4200 1150
Wire Wire Line
	4000 1050 3700 1050
Wire Wire Line
	4000 1050 4000 4600
Wire Wire Line
	4200 1150 4200 4900
Wire Wire Line
	3100 2600 5000 2600
Wire Wire Line
	5000 2600 5000 1450
Wire Wire Line
	5000 1450 5500 1450
Wire Wire Line
	2500 5400 1950 5400
Wire Wire Line
	1950 5400 1950 6000
Wire Wire Line
	750  6000 1950 6000
Connection ~ 1950 6000
Wire Wire Line
	1950 6000 1950 6050
Wire Wire Line
	5050 6000 1950 6000
Wire Wire Line
	5050 4900 5050 6000
$EndSCHEMATC
