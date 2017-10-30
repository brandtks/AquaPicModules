EESchema Schematic File Version 2
LIBS:AquaPic
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
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
LIBS:Level-cache
EELAYER 25 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "Level Sensor"
Date "20 oct 2013"
Rev "0"
Comp "AquaPic"
Comment1 "Skyler Brandt"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MPVZ5004G U1
U 1 1 52645F40
P 4500 4100
F 0 "U1" H 4300 4350 60  0000 C CNN
F 1 "MPVZ5004G" H 4850 3750 60  0000 C CNN
F 2 "~" H 4500 4150 60  0000 C CNN
F 3 "~" H 4500 4150 60  0000 C CNN
	1    4500 4100
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 K1
U 1 1 52645F8E
P 4500 3200
F 0 "K1" V 4450 3200 50  0000 C CNN
F 1 "CONN_3" V 4550 3200 40  0000 C CNN
F 2 "~" H 4500 3200 60  0000 C CNN
F 3 "~" H 4500 3200 60  0000 C CNN
	1    4500 3200
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR01
U 1 1 52645FAC
P 4500 3650
F 0 "#PWR01" H 4500 3650 30  0001 C CNN
F 1 "GND" H 4500 3580 30  0001 C CNN
F 2 "~" H 4500 3650 60  0000 C CNN
F 3 "~" H 4500 3650 60  0000 C CNN
	1    4500 3650
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR02
U 1 1 52645FBB
P 4250 3550
F 0 "#PWR02" H 4250 3640 20  0001 C CNN
F 1 "+5V" H 4250 3640 30  0000 C CNN
F 2 "~" H 4250 3550 60  0000 C CNN
F 3 "~" H 4250 3550 60  0000 C CNN
	1    4250 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 3550 4250 3650
Wire Wire Line
	4250 3650 4400 3650
Wire Wire Line
	4400 3650 4400 3550
Wire Wire Line
	4500 3550 4500 3650
Wire Wire Line
	4600 3550 4600 3650
Wire Wire Line
	4600 3650 5150 3650
Text Label 5000 3650 2    60   ~ 0
Level
$Comp
L C C2
U 1 1 52645FD5
P 3850 4250
F 0 "C2" H 3850 4350 40  0000 L CNN
F 1 "u01" H 3856 4165 40  0000 L CNN
F 2 "~" H 3888 4100 30  0000 C CNN
F 3 "~" H 3850 4250 60  0000 C CNN
	1    3850 4250
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 52645FE4
P 3550 4250
F 0 "C1" H 3550 4350 40  0000 L CNN
F 1 "1uF" H 3556 4165 40  0000 L CNN
F 2 "~" H 3588 4100 30  0000 C CNN
F 3 "~" H 3550 4250 60  0000 C CNN
	1    3550 4250
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR03
U 1 1 52645FF3
P 3550 3850
F 0 "#PWR03" H 3550 3940 20  0001 C CNN
F 1 "+5V" H 3550 3940 30  0000 C CNN
F 2 "~" H 3550 3850 60  0000 C CNN
F 3 "~" H 3550 3850 60  0000 C CNN
	1    3550 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 3850 3550 4050
Wire Wire Line
	3550 3950 3950 3950
Connection ~ 3550 3950
Wire Wire Line
	3850 3950 3850 4050
Connection ~ 3850 3950
Wire Wire Line
	3550 4450 3550 4700
Wire Wire Line
	3550 4700 4500 4700
Wire Wire Line
	4500 4600 4500 4800
Wire Wire Line
	3850 4450 3850 4700
Connection ~ 3850 4700
$Comp
L GND #PWR04
U 1 1 52646048
P 4500 4800
F 0 "#PWR04" H 4500 4800 30  0001 C CNN
F 1 "GND" H 4500 4730 30  0001 C CNN
F 2 "~" H 4500 4800 60  0000 C CNN
F 3 "~" H 4500 4800 60  0000 C CNN
	1    4500 4800
	1    0    0    -1  
$EndComp
Connection ~ 4500 4700
Wire Wire Line
	5150 3950 5050 3950
Wire Wire Line
	5150 3650 5150 3950
Wire Notes Line
	3100 2900 5950 2900
Wire Notes Line
	5950 2900 5950 5300
Wire Notes Line
	5950 5300 3100 5300
Wire Notes Line
	3100 5300 3100 2900
Text Notes 5950 5300 2    60   ~ 0
Level Sensor\n
$EndSCHEMATC
