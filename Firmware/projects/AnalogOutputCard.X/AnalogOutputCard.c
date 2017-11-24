/* Created by Skyler Brandt on August 2015
 *
 * Analog Output Card
 *
 *******************************************************************************
 * Copyright (C) 2015 Skyler Brandt
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

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <stdlib.h>     /* null */
#include <stdint.h>     /* uint8_t, int8_t */
#include <string.h>     /* memcpy */
#include <xc.h>         /* EEPROM */
#include <pic16f1936.h>
#include "../../lib/aquaPicBus/aquaPicBus.h"
#include "../../lib/common/common.h"
#include "../../lib/led/led.h"
#include "../../lib/pins/pins.h"
#include "../../lib/PIC16F/pwm/pwm.h"
#include "../../lib/PIC16F/uart/uart.h"
#include "bsp.h"

/******************************************************************************/
/* EEPROM                                                                     */
/******************************************************************************/
/*HEX FILE EEPROM INITIAL VALUES*/
__EEPROM_DATA (0,0,0,0,0,0,0,0);

/*Define eeprom read and write functions from xc.h*/
unsigned char eeprom_read(unsigned char address);
void eeprom_write(unsigned char address, unsigned char value);

/*User Defined*/
#define CHANNEL_1_ADDRESS 0x00
#define CHANNEL_2_ADDRESS 0x01
#define CHANNEL_3_ADDRESS 0x02
#define CHANNEL_4_ADDRESS 0x03

/******************************************************************************/
/* USER DEFINED                                                               */
/******************************************************************************/

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void initializeHardware(void);
void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength);
void writeUart(uint8_t* data, uint8_t length);

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
struct pwmObjStruct pwmInstStruct[4];
pwmObj pwmInst[4];
int8_t apbLastErrorState;

void main (void) {
    int i;
    for (i = 0; i < 4; ++i) {
        pwmInst[i] = &pwmInstStruct[i];
    }

    initializeHardware ();

#if SELECTABLE_OUTPUT
    int i;
    for (i = 0; i < 4; ++i) {
        uint8_t type = eeprom_read(i);
        WRITE_PIN(&PORTB, i + 1, type == 255);
    }
#endif

    /* AquaPic Bus initialization */
    if (!apb_init(&apbMessageHandler, 
            &writeUart,
            APB_ADDRESS,
            FRAMING_TIMER,
            ERROR_TIME)) {
        while (1);
    }
    apbLastErrorState = 0;

    /* Enable UART */
    TXSTAbits.TXEN = 1; /* Transmit Enable, Transmit enabled */
    RCSTAbits.CREN = 1; /* Continuous Receive Enable, Enables receiver */

    /*Global Interrupts*/
    PEIE = 1;   /* Enable peripheral interrupts */
    GIE = 1;    /* Enable Global interrupts */

    SET_LED(YELLOW_LED_PORT, YELLOW_LED_PIN, OFF);
    SET_LED(GREEN_LED_PORT, GREEN_LED_PIN, ON);

    while (1) {
        /* RCIF is set regardless of the global interrupts */
        /* apb_run might take a while so putting it in the main "loop" makes more sense */
        if (RCIF) {
            uint8_t data = RCREG;
            apb_run(data);
        }
    }
}

void interrupt ISR (void) {
    if (TMR6IF) {
        TMR6IF = 0; /* Clear flag */
        apb_framing();

        int8_t apbError = apb_isErrored();
        if (apbError && !apbLastErrorState) {
            SET_LED(GREEN_LED_PORT, GREEN_LED_PIN, OFF);
            SET_LED(RED_LED_PORT, RED_LED_PIN, ON);
        } else if (apbError && !apbLastErrorState) {
            SET_LED(RED_LED_PORT, RED_LED_PIN, OFF);
            SET_LED(GREEN_LED_PORT, GREEN_LED_PIN, ON);
        }
        apbLastErrorState = apbError;
    }
}

void initializeHardware (void) {
    /* Oscillator */
    OSCCONbits.SCS = 0b00;  //System Clock Select: Clock determined by FOSC<2:0> in Configuration Word 1

    /* Port Initialization */
    PORTA = 0x03;   //Clear Port A, Write 1 to RG Status LED sinks, ie turn off LEDs
    PORTB = 0x00;   //Clear Port B
    PORTC = 0x00;   //Clear Port C

    /* Port Direction */
    TRISA = 0b00000000; //Port A Directions
            //*****0** = RA2, Yellow Status LED
            //******0* = RA1, Green Status LED
            //*******0 = RA0, Red Status LED
    TRISB = 0b00100001; //Port B Directions
            //**1***** = RB5, Initial setting for CCP3(PWM) setup
            //***0**** = RB4, Ch4 Relay
            //****0*** = RB3, Ch3 Relay
            //*****0** = RB2, Ch2 Relay
            //******0* = RB1, Ch1 Relay
            //*******1 = RB0, Initial setting for CCP4(PWM) setup
    TRISC = 0b10000110; //Port C Directions
            //1******* = RC7, RX
            //*0****** = RC6, TX
            //**0***** = RC5, TX_nRX
            //*****1** = RC2, Initial setting for CCP1(PWM) setup
            //******1* = RC1, Initial setting for CCP2(PWM) setup

    /* Analog Select */
    ANSELA = 0x00;  //All digital ports
    ANSELB = 0x00;  //All digital ports

    /* Port Selection */
    APFCONbits.CCP3SEL = 1; //CCP3:P3A function is on RB5
    APFCONbits.CCP2SEL = 0; //CCP2:P2A function is on

    /* CCP(PWM) */
    initPwm(pwmInst[0], &CCP1CON, &CCPR1L); /* CCP1 */
    initPwm(pwmInst[1], &CCP2CON, &CCPR2L); /* CCP2 */
    initPwm(pwmInst[2], &CCP3CON, &CCPR3L); /* CCP3 */
    initPwm(pwmInst[3], &CCP4CON, &CCPR4L); /* CCP4 */
    CCPTMRS0 = CCPTMRS0_C1TSEL_TMR2 |
            CCPTMRS0_C2TSEL_TMR2 |
            CCPTMRS0_C3TSEL_TMR2 |
            CCPTMRS0_C4TSEL_TMR2;

    PR2 = 0xFF; /* 31.25 kHz from data sheet page 218 */
    T2CON = 0b00000100;
            //*0000*** = T2OUTPS: Timer2 Output Postscaler Select, Does Not Matter for PWM Period
            //*****1** = TMR2ON: Timer2 is on
            //******00 = T2CKPS: Timer2-type Clock Prescale Select, Prescaler is 1

    TRISCbits.TRISC2 = 0; //Clear TRISC2 bit to enable PWM1 output on pin
    TRISCbits.TRISC1 = 0; //Clear TRISC1 bit to enable PWM2 output on pin
    TRISBbits.TRISB5 = 0; //Clear TRISB5 bit to enable PWM3 output on pin
    TRISBbits.TRISB0 = 0; //Clear TRISB0 bit to enable PWM4 output on pin

    /* Timer 6 */
    PR6 = 0x7C; //Timer Period = 1mSec
                //Timer Period = [PRx + 1] * 4 * Tosc * TxCKPS * TxOUTPS
                //PRx = [Timer Period / (4 * Tosc * TxCKPS * TxOUTPS)] - 1
                //PRx = 1mSec / (4 * (1 / 32MHz) * 64 * 1)] - 1
                //PRx = 124 or 7C
                //Note: Tosc = 1 / Fosc

    T6CON = 0b00000111;
            //*0000*** = T6OUTPS: Timer Output Postscaler Select, 1:1 Postscaler
            //*****1** = TMR6ON: Timer6 is on
            //******11 = T6CKPS: Timer2-type Clock Prescaler Select, Prescaler is 64

    /* UART */
    //Set to one for fast speed
    BAUDCONbits.BRG16 = 1; //16-bit Baud Rate Generator, 16-bit Baud Rate Generator is used
    //Set to one for fast speed
    TXSTAbits.BRGH = 1; //High Baud Rate Select, High speed
    SPBRGH = 0x00;  //Nothing in the high register
    SPBRGL = 0x8A;
                    //Desired Baud Rate = 57,600Mb or 115,200Mb
                    //Baud Rate = Fosc / 4(BRG + 1) = 32MHz / 4(138 + 1) = 57,554Mb
                    //BRG = 138 or 0x8A
                    //Error = (Desired Baud Rate - Baud Rate) / Desired Baud Rate
                    //      = 57,600 - 57,554 / 57,600 = 0.08% Error

                    //Desired Baud Rate = 9600 for testing
                    //Baud Rate = Fosc / 64(BRG + 1)
                    //BRG = Fosc / (64 * Baud Rate) - 1
                    //BRG = 32MHz / (64 * 9600) - 1
                    //BRG = 51 or 0x33
                    //9615 Mb

    TXSTAbits.TX9 = 0; /* 9-bit Transmit Enable, De-selects 9-bit transmission */
    RCSTA = 0b10000000;
            //1******* = SPEN: Serial Port Enable, Serial port enabled
            //*0****** = RX9: 9-bit Receive Enable, De-selects 9-bit reception
            //****0*** = ADDEN: Address Detect Enable, Disables address detection

    /* Interrupts */
    TMR6IF = 0;     /* Clear Timer6 interrupt flag */
    TMR6IE = 1;     /* Enable Timer6 interrupts */
}

void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength) {
    switch (function) {
#if SELECTABLE_OUTPUT
        case 2: { /* setup single channel */
            uint8_t channel = message[3] + 1; //0 is channel 1
            uint8_t type    = message[4];
            //type 255 is PWM output so close relay to bypass filter
            WRITE_PIN(&PORTB, channel, type == 255);
            eeprom_write(channel - 1, type);
            apb_sendDefualtResponse();
            break;
        }
#endif
        case 10: { /* read single channel value */
            uint8_t channel = message[3];
            uint16_t value  = getPwmValue(pwmInst[channel]);
            apb_initResponse();
            apb_addToResponse(&value, sizeof(uint16_t));
            apb_sendResponse();

            break;
        }
        case 20: { /* read all channels values */
            uint16_t values[4];
            int i;
            for (i = 0; i < 4; ++i) {
                values[i] = getPwmValue(pwmInst[i]);
            }
            apb_initResponse();
            apb_addToResponse(values, sizeof(uint16_t) * 4);
            apb_sendResponse();
            break;
        }
        case 30: { /* write all channels values */
            uint16_t values[4];
            memcpy(values, &(message[3]), sizeof(uint16_t) * 4);
            int i;
            for (i = 0; i < 4; ++i) {
                setPwmValue(pwmInst[i], values [i]);
            }
            apb_sendDefualtResponse();
            break;
        }
        case 31: { /* write single channel value */
            uint8_t channel = message[3];
            uint16_t value;
            memcpy(&value, &(message[4]), sizeof(uint16_t));
            /* value = (value >> 8) | (value << 8); /* reverse the endianess */
            setPwmValue(pwmInst[channel], value);
            apb_sendDefualtResponse();
            break;
        }
        default:
            break;
    }
}

void writeUart(uint8_t* data, uint8_t length) {
    WRITE_PIN(TX_ENABLE_PORT, TX_ENABLE_PIN, HIGH);
    putsch(data, length);
    WRITE_PIN(TX_ENABLE_PORT, TX_ENABLE_PIN, LOW);
}
