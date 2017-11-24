/* Created by Skyler Brandt on December 2015
 *
 * Digital Input Card
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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>     //For uint8_t, int8_t definition
#include <xc.h>
#include <pic16f1936.h>
#include "../../lib/aquaPicBus/aquaPicBus.h"
#include "../../lib/common/common.h"
#include "../../lib/led/led.h"
#include "../../lib/pins/pins.h"
#include "../../lib/PIC16F/uart/uart.h"
#include "bsp.h"

/******************************************************************************/
/* EEPROM                                                                     */
/******************************************************************************/

/******************************************************************************/
/* USER DEFINED                                                               */
/******************************************************************************/
#define NUM_CHANNELS    6

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
int8_t apbLastErrorState;

void main (void) {
    initializeHardware ();

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
        SET_LED(CH1_LED_PORT, CH1_LED_PIN, READ_PIN(CH1_INPUT_PORT, CH1_INPUT_PIN));
        SET_LED(CH2_LED_PORT, CH2_LED_PIN, READ_PIN(CH2_INPUT_PORT, CH2_INPUT_PIN));
        SET_LED(CH3_LED_PORT, CH3_LED_PIN, READ_PIN(CH3_INPUT_PORT, CH3_INPUT_PIN));
        SET_LED(CH4_LED_PORT, CH4_LED_PIN, READ_PIN(CH4_INPUT_PORT, CH4_INPUT_PIN));
        SET_LED(CH5_LED_PORT, CH5_LED_PIN, READ_PIN(CH5_INPUT_PORT, CH5_INPUT_PIN));
        SET_LED(CH6_LED_PORT, CH6_LED_PIN, READ_PIN(CH6_INPUT_PORT, CH6_INPUT_PIN));
        
        /*RCIF is set regardless of the global interrupts*/
        /*apb_run might take a while so putting it in the main "loop" makes more sense*/
        if (RCIF) {
            uint8_t data = RCREG;
            apb_run (data);
        }
    }
}

void interrupt ISR (void) {
    if (TMR2IF) {
        TMR2IF = 0; /* Clear flag */

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
    /*Oscillator*/
    OSCCONbits.SCS = 0b00;  //System Clock Select: Clock determined by FOSC<2:0> in Configuration Word 1

    /*Port Initialization*/
    PORTA = 0x03;   /*Clear Port A, Write 1 to RG Status LED sinks, ie turn off LEDs*/
    PORTB = 0x00;   /*Clear Port B*/
    PORTC = 0x00;   /*Clear Port C*/

    /*Port Direction*/
    TRISA = 0b00000000; //Port A Directions
            //***0**** = RA4, Input Channel 2 LED
            //****0*** = RA3, Input Channel 1 LED
            //*****0** = RA2, Yellow Status LED
            //******0* = RA1, Green Status LED
            //*******0 = RA0, Red Status LED
    TRISB = 0b00111111; //Port B Directions
            //**1***** = RB5, Input 6
            //***1**** = RB4, Input 5
            //****1*** = RB3, Input 4
            //*****1** = RB2, Input 3
            //******1* = RB1, Input 2
            //*******1 = RB0, Input 1
    TRISC = 0b10000000; //Port C Directions
            //1******* = RC7, RX
            //*0****** = RC6, TX
            //**0***** = RC5, TX_nRX
            //****0*** = RC3, Input Channel 6 LED
            //*****0** = RC2, Input Channel 5 LED
            //******0* = RC1, Input Channel 4 LED
            //*******0 = RC0, Input Channel 3 LED

    /*Analog Select*/
    ANSELA = 0x00;  /*All digital ports*/
    ANSELB = 0x00;  /*All digital ports*/
    
    /*Port B*/
    WPUB = 0b00111111; /*Port B Pull-up Resistors*/
           //**1***** = RB5, Input 6
           //***1**** = RB4, Input 5
           //****1*** = RB3, Input 4
           //*****1** = RB2, Input 3
           //******1* = RB1, Input 2
           //*******1 = RB0, Input 1

#if IOC    
    IOCBP = 0b00111111; /*Port B Interrupt on Positive Edge*/
            //**1***** = RB5, Input 6
            //***1**** = RB4, Input 5
            //****1*** = RB3, Input 4
            //*****1** = RB2, Input 3
            //******1* = RB1, Input 2
            //*******1 = RB0, Input 1
    
    IOCBN = 0b00111111; /*Port B Interrupt on Positive Edge*/
            //**1***** = RB5, Input 6
            //***1**** = RB4, Input 5
            //****1*** = RB3, Input 4
            //*****1** = RB2, Input 3
            //******1* = RB1, Input 2
            //*******1 = RB0, Input 1
    
    IOCIF = 0;  /*Clear IOC interrupt flag*/
    IOCIE = 0;  /*Enable IOC interrupts*/
#endif  
    
    /****Timer 2****/
    PR2 = 0x7C; //Timer Period = 1mSec
                //Timer Period = [PRx + 1] * 4 * Tosc * TxCKPS * TxOUTPS
                //PRx = [Timer Period / (4 * Tosc * TxCKPS * TxOUTPS)] - 1
                //PRx = 1mSec / (4 * (1 / 32MHz) * 64 * 1)] - 1
                //PRx = 124 or 7C
                //Note: Tosc = 1 / Fosc

    T2CON = 0b00000111;
            //*0000*** = T2OUTPS: Timer Output Postscaler Select, 1:1 Postscaler
            //*****1** = TMR2ON: Timer2 is on
            //******11 = T2CKPS: Timer2-type Clock Prescaler Select, Prescaler is 64

    /*UART*/
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
    
    /*Interrupts*/
    TMR2IF = 0;     /* Clear Timer2 interrupt flag */
    TMR2IE = 1;     /* Enable Timer2 interrupts */
}

void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength) {
    switch (function) {
        case 10: { /* read single channel value */
            uint8_t channel = message[3];
            uint8_t value  = READ_PIN(&PORTB, channel);
            
            apb_initResponse();
            apb_appendToResponse(value);
            apb_sendResponse();
            break;
        }
        case 20: { /* read all channels values */
            uint8_t values = 0x00;            
            int i;
            for (i = 0; i < NUM_CHANNELS; ++i) {
                values |= READ_PIN(&PORTB, i);
            }

            apb_initResponse();
            apb_appendToResponse(values);
            apb_sendResponse();
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
