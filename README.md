README
======
Firmware for the input/output cards for the AquaPic aquarium controller. Includes Analog Input and Output, Digital Input, and Power Strip stored in the projects directory. Libraries are included in the lib directory. This inludes the AquaPicBus slave driver, some common macros, and PIC16F specific drivers for ADC, UART, and PWM. All modules communicate with and are controlled by the [master controller/head unit](https://github.com/AquaPic/AquaPicController).

### Status
**IMPORTANT**: All modules are a work in progress. Most, if not all, are probably unstable and currently untested.  

### Toolchain
Microchip MPLAB X v3.51. May work with early versions.

Microchip XC8 v1.40. May work with early versions.

### Building
 * Edit Makefile-define.mk to match the executing environment and desired output paths
 * ```make```

### License
The AquaPic firmware code is released under the terms of the GNU General Public License (GPL), version 3 or later. See COPYING for details.

### Website
 * [HackADay Build Log](https://hackaday.io/project/1436-aquapic-aquarium-controller)
 * [Less Updated Build Log](https://sites.google.com/site/aquapicbuildlog/)

### Additional Notes
**AquaPicBus Slave Test Code**

_/lib/aquaPicBus/test_ contains a NetBeans project for manually testing that the slave driver works as expected.

**Building**

```cd lib/aquaPicBus/test```

```make```
