EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:freetronics_schematic
LIBS:RF_OEM_Parts
LIBS:whale_circuit_1-cache
EELAYER 25 0
EELAYER END
$Descr A2 23386 16535
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
L Battery LOGIC
U 1 1 5A0339E0
P 3150 15350
F 0 "LOGIC" H 3250 15450 50  0000 L CNN
F 1 "Battery" H 3250 15350 50  0000 L CNN
F 2 "" V 3150 15410 50  0001 C CNN
F 3 "" V 3150 15410 50  0001 C CNN
	1    3150 15350
	1    0    0    -1  
$EndComp
$Comp
L Battery POWER
U 1 1 5A033BBB
P 6350 15350
F 0 "POWER" H 6450 15450 50  0000 L CNN
F 1 "Battery" H 6450 15350 50  0000 L CNN
F 2 "" V 6350 15410 50  0001 C CNN
F 3 "" V 6350 15410 50  0001 C CNN
	1    6350 15350
	1    0    0    -1  
$EndComp
$Comp
L Fuse F?
U 1 1 5A033C26
P 3150 15000
F 0 "F?" V 3230 15000 50  0001 C CNN
F 1 "6A" V 3075 15000 50  0000 C CNN
F 2 "" V 3080 15000 50  0001 C CNN
F 3 "" H 3150 15000 50  0001 C CNN
	1    3150 15000
	1    0    0    -1  
$EndComp
$Comp
L Fuse F?
U 1 1 5A033CC3
P 6350 15000
F 0 "F?" V 6430 15000 50  0001 C CNN
F 1 "6A" V 6275 15000 50  0000 C CNN
F 2 "" V 6280 15000 50  0001 C CNN
F 3 "" H 6350 15000 50  0001 C CNN
	1    6350 15000
	1    0    0    -1  
$EndComp
$Comp
L L7805 U?
U 1 1 5A033D77
P 3150 14550
F 0 "U?" H 3000 14675 50  0001 C CNN
F 1 "5V Converter" H 3150 14675 50  0000 C CNN
F 2 "" H 3175 14400 50  0001 L CIN
F 3 "" H 3150 14500 50  0001 C CNN
	1    3150 14550
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5A033F1C
P 3450 14550
F 0 "#PWR?" H 3450 14300 50  0001 C CNN
F 1 "GND" H 3450 14400 50  0000 C CNN
F 2 "" H 3450 14550 50  0001 C CNN
F 3 "" H 3450 14550 50  0001 C CNN
	1    3450 14550
	1    0    0    -1  
$EndComp
$Comp
L FINDER-32.21-x300 E-STOP
U 1 1 5A033F6C
P 6150 14550
F 0 "E-STOP" H 6600 14700 50  0001 L CNN
F 1 "FINDER-32.21-x300" H 6600 14600 50  0001 L CNN
F 2 "Relays_THT:Relay_SPST_Finder_32.21-x300" H 7420 14520 50  0001 C CNN
F 3 "" H 6150 14550 50  0001 C CNN
	1    6150 14550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5A034021
P 5950 14850
F 0 "#PWR?" H 5950 14600 50  0001 C CNN
F 1 "GND" H 5950 14700 50  0000 C CNN
F 2 "" H 5950 14850 50  0001 C CNN
F 3 "" H 5950 14850 50  0001 C CNN
	1    5950 14850
	1    0    0    -1  
$EndComp
$Comp
L ARDUINO_FOOTPRINT Sense/Think
U 1 1 5A0341EF
P 2600 12150
F 0 "Sense/Think" H 2150 13300 60  0000 C CNN
F 1 "ARDUINO_FOOTPRINT" H 2650 11200 60  0001 C CNN
F 2 "FT:ARDUINO_SHIELD" H 2600 11950 50  0001 C CNN
F 3 "" H 2600 12150 60  0000 C CNN
	1    2600 12150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5A034297
P 3150 15550
F 0 "#PWR?" H 3150 15300 50  0001 C CNN
F 1 "GND" H 3150 15400 50  0000 C CNN
F 2 "" H 3150 15550 50  0001 C CNN
F 3 "" H 3150 15550 50  0001 C CNN
	1    3150 15550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5A0342B2
P 6350 15550
F 0 "#PWR?" H 6350 15300 50  0001 C CNN
F 1 "GND" H 6350 15400 50  0000 C CNN
F 2 "" H 6350 15550 50  0001 C CNN
F 3 "" H 6350 15550 50  0001 C CNN
	1    6350 15550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5A0342C6
P 1450 12050
F 0 "#PWR?" H 1450 11800 50  0001 C CNN
F 1 "GND" H 1450 11900 50  0000 C CNN
F 2 "" H 1450 12050 50  0001 C CNN
F 3 "" H 1450 12050 50  0001 C CNN
	1    1450 12050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 12050 1450 12050
$Comp
L Thermistor_PTC TH?
U 1 1 5A034358
P 1050 12450
F 0 "TH?" V 890 12450 50  0001 C CNN
F 1 "Temp_Sensor" V 1170 12450 50  0000 C CNN
F 2 "" H 1100 12250 50  0001 L CNN
F 3 "" H 1050 12450 50  0001 C CNN
	1    1050 12450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1200 12450 1650 12450
$Comp
L GND #PWR?
U 1 1 5A03474F
P 800 12450
F 0 "#PWR?" H 800 12200 50  0001 C CNN
F 1 "GND" H 800 12300 50  0000 C CNN
F 2 "" H 800 12450 50  0001 C CNN
F 3 "" H 800 12450 50  0001 C CNN
	1    800  12450
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  12450 800  12450
$Comp
L SW_SPDT Water_Sensor
U 1 1 5A034832
P 3900 12550
F 0 "Water_Sensor" H 3900 12720 50  0000 C CNN
F 1 "SW_SPDT" H 3900 12350 50  0001 C CNN
F 2 "" H 3900 12550 50  0001 C CNN
F 3 "" H 3900 12550 50  0001 C CNN
	1    3900 12550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 12550 3550 12550
$Comp
L GND #PWR?
U 1 1 5A034A35
P 4200 12650
F 0 "#PWR?" H 4200 12400 50  0001 C CNN
F 1 "GND" H 4200 12500 50  0000 C CNN
F 2 "" H 4200 12650 50  0001 C CNN
F 3 "" H 4200 12650 50  0001 C CNN
	1    4200 12650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 12650 4200 12650
Wire Wire Line
	4300 12450 4100 12450
$Comp
L VCC #PWR?
U 1 1 5A034D4E
P 4300 12450
F 0 "#PWR?" H 4300 12300 50  0001 C CNN
F 1 "VCC" H 4300 12600 50  0000 C CNN
F 2 "" H 4300 12450 50  0001 C CNN
F 3 "" H 4300 12450 50  0001 C CNN
	1    4300 12450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 12250 1250 12250
Wire Wire Line
	1250 12250 1250 12200
$Comp
L VCC #PWR?
U 1 1 5A034DFE
P 1250 12200
F 0 "#PWR?" H 1250 12050 50  0001 C CNN
F 1 "VCC" H 1250 12350 50  0000 C CNN
F 2 "" H 1250 12200 50  0001 C CNN
F 3 "" H 1250 12200 50  0001 C CNN
	1    1250 12200
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 5A034E45
P 3150 14250
F 0 "#PWR?" H 3150 14100 50  0001 C CNN
F 1 "VCC" H 3150 14400 50  0000 C CNN
F 2 "" H 3150 14250 50  0001 C CNN
F 3 "" H 3150 14250 50  0001 C CNN
	1    3150 14250
	1    0    0    -1  
$EndComp
$Comp
L ARDUINO_FOOTPRINT Act
U 1 1 5A0351A6
P 6500 12150
F 0 "Act" H 6050 13300 60  0000 C CNN
F 1 "ARDUINO_FOOTPRINT" H 6550 11200 60  0001 C CNN
F 2 "FT:ARDUINO_SHIELD" H 6500 11950 50  0001 C CNN
F 3 "" H 6500 12150 60  0000 C CNN
	1    6500 12150
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x03_Top_Bottom J?
U 1 1 5A0358D9
P 3000 9900
F 0 "J?" H 3050 10100 50  0001 C CNN
F 1 "PIXY CAM" H 3050 9700 50  0000 C CNN
F 2 "" H 3000 9900 50  0001 C CNN
F 3 "" H 3000 9900 50  0001 C CNN
	1    3000 9900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 9800 2300 9800
Wire Wire Line
	2300 9800 2300 10850
Wire Wire Line
	2300 10850 2500 10850
Wire Wire Line
	2500 10750 2400 10750
Wire Wire Line
	2400 10750 2400 9900
Wire Wire Line
	2400 9900 2800 9900
Wire Wire Line
	2800 10000 2500 10000
Wire Wire Line
	2500 10000 2500 10650
Wire Wire Line
	3550 10650 3550 10000
Wire Wire Line
	3550 10000 3300 10000
Wire Wire Line
	3550 10750 3650 10750
Wire Wire Line
	3650 10750 3650 9900
Wire Wire Line
	3650 9900 3300 9900
Wire Wire Line
	3300 9800 3750 9800
Wire Wire Line
	3750 9800 3750 10850
Wire Wire Line
	3750 10850 3550 10850
$Comp
L XBEE XB?
U 1 1 5A035AF5
P 2750 8250
F 0 "XB?" H 2750 9050 60  0000 C CNN
F 1 "XBEE" H 2750 9150 60  0000 C CNN
F 2 "" H 2750 8250 60  0000 C CNN
F 3 "" H 2750 8250 60  0000 C CNN
	1    2750 8250
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR?
U 1 1 5A035CB4
P 3950 8850
F 0 "#PWR?" H 3950 8700 50  0001 C CNN
F 1 "VCC" H 3950 9000 50  0000 C CNN
F 2 "" H 3950 8850 50  0001 C CNN
F 3 "" H 3950 8850 50  0001 C CNN
	1    3950 8850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5A0360DA
P 4000 7800
F 0 "#PWR?" H 4000 7550 50  0001 C CNN
F 1 "GND" H 4000 7650 50  0000 C CNN
F 2 "" H 4000 7800 50  0001 C CNN
F 3 "" H 4000 7800 50  0001 C CNN
	1    4000 7800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 8850 3800 8700
Wire Wire Line
	3800 7800 4000 7800
Wire Wire Line
	3800 8600 4400 8600
Wire Wire Line
	4400 8600 4400 12950
Wire Wire Line
	4600 8500 4600 12850
Wire Wire Line
	4600 8500 3800 8500
Wire Wire Line
	4400 12950 3550 12950
Wire Wire Line
	4600 12850 3550 12850
Wire Wire Line
	3550 11150 5700 11150
Wire Wire Line
	5700 11150 5700 11000
Wire Wire Line
	5700 11000 7450 11000
Wire Wire Line
	7450 11000 7450 11150
Wire Wire Line
	7450 11250 7550 11250
Wire Wire Line
	7550 11250 7550 10950
Wire Wire Line
	7550 10950 5600 10950
Wire Wire Line
	5600 10950 5600 11250
Wire Wire Line
	5600 11250 3550 11250
Wire Wire Line
	5950 14250 7650 14250
Wire Wire Line
	7650 14250 7650 12050
Wire Wire Line
	7650 12050 7450 12050
$Comp
L SW_Reed SW?
U 1 1 5A0E6483
P 5250 11900
F 0 "SW?" H 5250 12000 50  0001 C CNN
F 1 "E-Stop" H 5250 11800 50  0000 C CNN
F 2 "" H 5250 11900 50  0001 C CNN
F 3 "" H 5250 11900 50  0001 C CNN
	1    5250 11900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 11950 5450 11900
$Comp
L VCC #PWR?
U 1 1 5A0E656E
P 4900 11850
F 0 "#PWR?" H 4900 11700 50  0001 C CNN
F 1 "VCC" H 4900 12000 50  0000 C CNN
F 2 "" H 4900 11850 50  0001 C CNN
F 3 "" H 4900 11850 50  0001 C CNN
	1    4900 11850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 11850 4900 11900
Wire Wire Line
	4900 11900 5050 11900
Wire Wire Line
	5400 12100 5550 12050
$Comp
L GND #PWR?
U 1 1 5A0E68D1
P 5400 12100
F 0 "#PWR?" H 5400 11850 50  0001 C CNN
F 1 "GND" H 5400 11950 50  0000 C CNN
F 2 "" H 5400 12100 50  0001 C CNN
F 3 "" H 5400 12100 50  0001 C CNN
	1    5400 12100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 10650 7750 11550
Wire Wire Line
	7750 11550 7450 11550
$Comp
L LED D?
U 1 1 5A0E6C13
P 7900 10650
F 0 "D?" H 7900 10750 50  0001 C CNN
F 1 "STATUS LED" H 7900 10500 50  0000 C CNN
F 2 "" H 7900 10650 50  0001 C CNN
F 3 "" H 7900 10650 50  0001 C CNN
	1    7900 10650
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 5A0E6C40
P 8050 10650
F 0 "#PWR?" H 8050 10400 50  0001 C CNN
F 1 "GND" H 8050 10500 50  0000 C CNN
F 2 "" H 8050 10650 50  0001 C CNN
F 3 "" H 8050 10650 50  0001 C CNN
	1    8050 10650
	1    0    0    -1  
$EndComp
$EndSCHEMATC
