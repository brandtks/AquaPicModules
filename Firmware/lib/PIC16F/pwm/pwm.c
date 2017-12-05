/*
 * Analog To Digital Converter
 *
 */

/*******************************************************************************
 * Copyright (c) 2017 Goodtime Development
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 ******************************************************************************/

#include <xc.h>
#include <stdint.h>     /* uint8_t, int8_t */
#include "pwm.h" 

void inline initPwm(pwmObj inst, 
        volatile uint8_t* ccpcon, 
        volatile uint8_t* ccpr) 
{
    inst->ccpcon = ccpcon;
    inst->ccpr = ccpr;
    *(inst->ccpcon) = 0b00001100;
                      /* 00****** = PxM: Enhanced PWM Output Configuration, Single output; PxA modulated; PxB, PxC, PxD assigned as port pins */
                      /* **00**** = DCxB: PWM Duty Cycle Least Significant bits */
                      /* ****1100 = CCPxM: CCPx Mode Select, PWM mode */
    *(inst->ccpr) = 0x00;
}

void setPwmValue(pwmObj inst, uint16_t value) {
    *(inst->ccpr) = (uint8_t)((value >> 2) & 0xFF);

    if (value & 0x01) //if LSB set bit 4 in CCPxCON register
        *(inst->ccpcon) |= 0x10;
    else
        *(inst->ccpcon) &= ~0x10;

    if (value & 0x02) //if 2nd LSD set bit 5 in CCPxCON register
        *(inst->ccpcon) |= 0x20;
    else
        *(inst->ccpcon) &= ~0x20;
}

uint16_t getPwmValue(pwmObj inst) {
    uint16_t value = *(inst->ccpr) << 2;
    value |= (*(inst->ccpcon) & 0x30) >> 4; //bits 5:4 in CCPxCON are the two LSBs
    return value;
}