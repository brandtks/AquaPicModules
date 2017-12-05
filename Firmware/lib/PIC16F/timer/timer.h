/*
 * Timer
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

#ifndef TIMER_H
#define	TIMER_H

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
#define T2              2

#ifdef _16F18855
#define CLKCON_CS_FOSC4 0b00000001  /* CS: Timer2/4/6 Clock Select bits,  FOSC/4 */

#define CON_ON_ON       0b10000000  /* ON: Timerx On bit, Timerx is on */
#define CON_ON_OFF      0b00000000  /* ON: Timerx On bit, Timerx is off: all counters and state machines are reset */
#define CON_CKPS_64     0b01100000  /* CKPS: Timer2-type Clock Prescale Select bits, 1:64 Prescaler */
#define CON_OUTPS_1     0b00000000  /* OUTPS: Timerx Output Postscaler Select bits, 1:1 Postscaler */

#define HLT_MODE_PPSG   0b00000000  /* MODE: Timerx Control Mode Selection bits, Period pulse, Software gate */
#endif

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
#ifdef _16F18855
void inline timer_init8bit(uint8_t timerName, 
        uint8_t clkcon, 
        uint8_t con, 
        uint8_t hlt, 
        uint8_t rst, 
        uint8_t pr);
#endif

#endif /* TIMER_H */