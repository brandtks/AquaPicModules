README
======
Board design and firmware for all modules used with the AquaPic Aquarium Controller. Each board should have corresponding firmware that works with that board version. All modules communicate with and are controlled by the [master controller/head unit](https://github.com/AquaPic/AquaPicController).

 * All PCB designs and layouts including gerber files are located under the _Boards_ directory. All PCB design is done using [KiCAD](http://kicad-pcb.org/). 
 * Firmware is located in the _Firmware_ directory. All the card's firmware should build without the need of the [MPLAB X IDE](http://www.microchip.com/mplab/mplab-x-ide), just the [XC compilers](http://www.microchip.com/mplab/compilers).
 * Bill of materials are located in _BillOfMaterials/SharedBOM_. The BOMs are also shared in [Dropbox](https://www.dropbox.com/sh/w6prsuc161ip8sk/AAB5hL-DV105aX62imDUn7SDa?dl=0) so they can be viewed online. 
 * The _Documentation_ directory contains some useful information such as AquaPic Bus packets and other random notes.

### Status 
**IMPORTANT**: All modules are a work in progress. Most, if not all, are untested and very alpha quality.

### Website 
[HackADay Build Log](https://hackaday.io/project/1436-aquapic-aquarium-controller)
