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

#ifndef PWM_H
#define	PWM_H

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
#define CCPTMRS0_C1TSEL_TMR2    0b00000000  /* C1TSEL: CCP1 Timer Selection, Timer2 in PWM mode */
#define CCPTMRS0_C1TSEL_TMR4    0b00000001  /* C1TSEL: CCP1 Timer Selection, Timer4 in PWM mode */ 
#define CCPTMRS0_C1TSEL_TMR6    0b00000010  /* C1TSEL: CCP1 Timer Selection, Timer6 in PWM mode */
#define CCPTMRS0_C2TSEL_TMR2    0b00000000  /* C2TSEL: CCP2 Timer Selection, Timer2 in PWM mode */
#define CCPTMRS0_C2TSEL_TMR4    0b00000100  /* C2TSEL: CCP2 Timer Selection, Timer4 in PWM mode */ 
#define CCPTMRS0_C2TSEL_TMR6    0b00001000  /* C2TSEL: CCP2 Timer Selection, Timer6 in PWM mode */
#define CCPTMRS0_C3TSEL_TMR2    0b00000000  /* C3TSEL: CCP3 Timer Selection, Timer2 in PWM mode */
#define CCPTMRS0_C3TSEL_TMR4    0b00010000  /* C3TSEL: CCP3 Timer Selection, Timer4 in PWM mode */ 
#define CCPTMRS0_C3TSEL_TMR6    0b00100000  /* C3TSEL: CCP3 Timer Selection, Timer6 in PWM mode */ 
#define CCPTMRS0_C4TSEL_TMR2    0b00000000  /* C4TSEL: CCP4 Timer Selection, Timer2 in PWM mode */
#define CCPTMRS0_C4TSEL_TMR4    0b01000000  /* C4TSEL: CCP4 Timer Selection, Timer4 in PWM mode */ 
#define CCPTMRS0_C4TSEL_TMR6    0b10000000  /* C4TSEL: CCP4 Timer Selection, Timer6 in PWM mode */ 

#define CCPTMRS1_C5TSEL_TMR2    0b00000000  /* C5TSEL: CCP5 Timer Selection, Timer2 in PWM mode */
#define CCPTMRS1_C5TSEL_TMR4    0b00000001  /* C5TSEL: CCP5 Timer Selection, Timer4 in PWM mode */ 
#define CCPTMRS1_C5TSEL_TMR6    0b00000010  /* C5TSEL: CCP5 Timer Selection, Timer6 in PWM mode */

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
struct pwmObjStruct {
    volatile uint8_t* ccpcon;
    volatile uint8_t* ccpr;
};

typedef struct pwmObjStruct* pwmObj;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/*****Initialize***************************************************************/
void inline initPwm(pwmObj inst, 
        volatile uint8_t* ccpcon, 
        volatile uint8_t* ccpr);

/*****Public*******************************************************************/
void setPwmValue(pwmObj inst, uint16_t value);
uint16_t getPwmValue(pwmObj inst);

#endif	/* PWM_H */
