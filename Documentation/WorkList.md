Bugs
=====================
 * Can't add modules, errors on address already used, only on the raspberry pi 
 * Move setup to init function and add input checking

Main Controller Software
========================
## Worklist 
 * Add programmable LPF constant for AI
 * Implement pH/ORP driver
 * Enable/disable channels for AI
 * Refactor ATO to separate module
 * Add ATO scene
 * RO/DI module
 * Saltwater mixing module
 * Wave module
 * Add/Move widgets to home screen at runtime
 * Remove cards, power strips at runtime
 * Notifications
 * Tank maintenance scheduler
 * Runtime modules
 * Data logging on server
 * Backup/Store settings on database
 * Low priority - Lighting return to auto (ramp/hold to auto setpoint) and shutdown
 * _Maybe_ - Continue moving from list to dictionary in driver

Peripherals
===========
## Analog Input 
#### Firmware
 * Test on a board
 * Enable/disable channels
 * Add EEPROM emulation in flash to persist each channel's low pass filter factor value and enable state
#### Hardware

## Analog Output
#### Firmware
 * Refactor processor to PIC32MM0064GPM028
#### Hardware
 * Refactor processor to PIC32MM0064GPM028
 * Figure out how to do different output types, ie 0-10 or PWM

## Digital Input
#### Firmware
 * Refactor processor to PIC32MM0064GPM028
#### Hardware
 * Refactor processor to PIC32MM0064GPM028

## pH/ORP 
#### Firmware
 * Test on a board
 * Add EEPROM emulation in flash to persist each channel's low pass filter factor value and enable state
 * _Maybe_ - Add hardware power enable to each channel
#### Hardware
 * _Maybe_ - Add hardware power enable to each channel

## Power
#### Firmware
 * Refactor processor to PIC32MM0064GPM***
#### Hardware
 * Refactor processor to PIC32MM0064GPM***

Long Range Goals
================
 * Look at outlet current of heaters to determine what the setpoint of the heater is currently set at and then also check for stuck heater. 
 * Home server for data logging, and website. Home server will also issues commands to controller.
 * Water change module
 * Dosing Pumps
 * Feeding
 * Salinity
 * Function block programming
 * Move modules and GUI to ‘plugins’ that are added at runtime. This will allow the controller to be used for any sort of home automation such as home brewing, gardening, aquaponics. 
 * Over the “air” updates and boot loader for all slave modules 
