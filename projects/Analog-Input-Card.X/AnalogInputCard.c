/* Created by Skyler Brandt on December 2015
 *
 * Analog Input Card
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
#include <pic16f1783.h>
#include "../../lib/aquaPicBus/aquaPicBus.h"
#include "../../lib/common/common.h"
#include "../../lib/adc/adc.h"
#include "bsp.h"

/******************************************************************************/
/* EEPROM                                                                     */
/******************************************************************************/

/******************************************************************************/
/* USER DEFINED                                                               */
/******************************************************************************/
#define FILTER_VALUES   10
#define NUM_CHANNELS    4

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
typedef struct amperage_filter {
    uint16_t sum;
    uint16_t values[FILTER_VALUES];
    uint16_t average;
    uint8_t chsValue : 5;
}amperageFilter;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void initializeHardware (void);
void apbMessageHandler (void);
void writeUartData (uint8_t* data, uint8_t length);
void sendDefualtResponse (void);
void memoryCopy (void* to, void* from, size_t count);

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
struct apbObjStruct apbInstStruct;
apbObj apbInst = &apbInstStruct;
uint16_t commCounter;
uint8_t  commError;
amperageFilter ct[NUM_CHANNELS];
uint8_t  inletPtr;
uint8_t  valuePtr;
uint8_t  timerCounter;

void main (void) {
    initializeHardware ();
    
    inletPtr = 0;
    valuePtr = 0;
    timerCounter = 0;
    
    int i, j;
    for (i = 0; i < NUM_CHANNELS; ++i) {
        ct[i].sum = 0;
        for (j = 0; j < FILTER_VALUES; ++j)
            ct[i].values[j] = 0;
        ct[i].average = 0;
    }
    
    ct[0].chsValue = 0b00000;
    ct[1].chsValue = 0b00001;
    ct[2].chsValue = 0b00010;
    ct[3].chsValue = 0b00011;

    //AquaPic Bus initialization
    apb_init (apbInst, 
            &apbMessageHandler, 
            APB_ADDRESS,
            1,
            transmitEnablePort,
            transmitEnablePin);

    //enable UART
    TXSTAbits.TXEN = 1; //Transmit Enable, Transmit enabled
    RCSTAbits.CREN = 1; //Continuous Receive Enable, Enables receiver
    
    /*Global Interrupts*/
    PEIE = 1; //Enable peripheral interrupts
    GIE = 1; //Enable Global interrupts
    
    yellowLedOff;
    greenLedOn;
    
    while (1) {
        //RCIF is set regardless of the global interrupts 
        //apb_run might take a while so putting it in the main "loop" makes more sense
        if (RCIF) {
            uint8_t data = RCREG;
            apb_run (apbInst, data);
        }
    }
}

void interrupt ISR (void) {
    static uint8_t lastPtr = NUM_CHANNELS - 1; //outletPtr starts at 0 so we'll just initialize this to the end
    
    if (ADIF) {
        ADIF = 0; /*Clear flag*/
        
        ct[inletPtr].sum -= ct[inletPtr].values[valuePtr]; //subtract the oldest value from the sum
        ct[inletPtr].values[valuePtr] = getAdc (); //get the new value
        ct[inletPtr].sum += ct[inletPtr].values[valuePtr]; //add the newest value to the sum
        
        ct[inletPtr].average = ct[inletPtr].sum / FILTER_VALUES; //average the sum
        
        increment(inletPtr, NUM_CHANNELS);
        ADCON0bits.CHS = ct[inletPtr].chsValue; //set the ADC to the new channel
        
        if (inletPtr == 0) //we're back to the beginning of the outlets, increment the value array pointer
            increment(valuePtr, FILTER_VALUES);
    }
    
    if (TMR2IF) {
        TMR2IF = 0; /* Clear flag */
        
        apb_framing (apbInst);
        
        timerCounter = ++timerCounter % 25;
        if (timerCounter == 0) {
            if (inletPtr != lastPtr) { //the ADC isn't finished so don't start it
                startAdc();
                lastPtr = inletPtr;
            }

            if (!commError) {
                ++commCounter;

                if (commCounter >= COMM_ERROR_SP) {
                    commError = -1;
                    greenLedOff;
                    redLedOn;
                    apb_restart (apbInst);
                }
            } else {
                if (commCounter == 0) {
                    commError = 0;
                    redLedOff;
                    greenLedOn;
                }
            }
        }
    }
}

void initializeHardware (void) {
    /*Oscillator*/
    OSCCONbits.SCS = 0b00;  //System Clock Select: Clock determined by FOSC<2:0> in Configuration Word 1

    /*Port Initialization*/
    PORTA = 0x00;   /* Clear Port A */
    PORTB = 0x00;   /* Clear Port B */
    PORTC = 0x12;   /* Clear Port C, Write 1 to RG Status LED sinks, ie turn off LEDs */

    /*Port Direction*/
    TRISA = 0b00001111; //Port A Directions
            //****1*** = RA3, AN3, Input 4
            //*****1** = RA2, AN2, Input 3
            //******1* = RA1, AN1, Input 2
            //*******1 = RA0, AN0, Input 1
    
    TRISB = 0x00; /* Nothing on port B */
    
    TRISC = 0b10000000; /* Port C Directions */
            //1******* = RC7, RX
            //*0****** = RC6, TX
            //**0***** = RC5, TX_nRX
            //****0*** = RC3, Red Status LED
            //*****0** = RC2, Green Status LED
            //******0* = RC1, Yellow Status LED

    /*Analog Select*/
    ANSELA = 0x0F;  /* Lower pins are analog */
    ANSELB = 0x00;  /* All digital ports */
    
    initAdc (CHS_AN0 | AD0_ADON_ENABLE, AD1_ADFM_RIGHT | AD1_ADCS_FOSC_16 | AD1_ADNREF_VSS | AD1_ADPREF_VDD);
    
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
    ADIF = 0;       /* Clear ADC interrupt flag */
    ADIE = 1;       /* Enable ADC interrupts */
    TMR2IF = 0;     /* Clear Timer2 interrupt flag */
    TMR2IE = 1;     /* Enable Timer2 interrupts */
}

void apbMessageHandler (void) {
    commCounter = 0;
    
    switch (apbInst->function) {
        case 10: { //read single channel value
            uint8_t channel = apbInst->message [3];
            uint16_t value  = ct[channel].average;
            
            apb_initResponse (apbInst);
            apb_appendToResponse (apbInst, channel);
            apb_addToResponse (apbInst, &value, sizeof(uint16_t));
            apb_sendResponse (apbInst);
 
            break;
        }
        case 20: { //read all channels values     
            uint16_t values [NUM_CHANNELS];

            int i;
            for (i = 0; i < NUM_CHANNELS; ++i) 
                values [i] = ct[i].average;
            
            apb_initResponse (apbInst);
            apb_addToResponse (apbInst, values, sizeof (uint16_t) * NUM_CHANNELS);
            apb_sendResponse (apbInst);

            break;
        }
        default:
            break;
    }
}