/* Created by Skyler Brandt on August 2015
 *
 * Power Control
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
#include <pic16f1937.h>
#include "../../lib/adc/adc.h"
#include "../../lib/aquaPicBus/aquaPicBus.h"
#include "../../lib/common/common.h"
#include "../../lib/led/led.h"
#include "../../lib/pins/pins.h"
#include "bsp.h"

/******************************************************************************/
/* EEPROM                                                                     */
/******************************************************************************/
/*HEX FILE EEPROM INITIAL VALUES*/
__EEPROM_DATA(0,0,0,0,0,0,0,0);

/*Define eeprom read and write functions from xc.h*/
unsigned char eeprom_read(unsigned char address);
void eeprom_write(unsigned char address, unsigned char value);

/*User Defined*/
#define OUTLET_FALLBACK_ADDRESS 0x00

/******************************************************************************/
/* USER DEFINED                                                               */
/******************************************************************************/
#define FILTER_VALUES   10
#define NUM_OUTLETS     8
#define ENABLE_CURRENT

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
#ifdef ENABLE_CURRENT
typedef struct amperage_filter {
    uint16_t sum;
    uint16_t values[FILTER_VALUES];
    uint16_t average;
    uint8_t chsValue : 5;
}amperageFilter;
#endif

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void initializeHardware(void);
void apbMessageHandler(void);

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
struct apbObjStruct apbInstStruct;
apbObj apbInst = &apbInstStruct;

uint16_t commCounter;
int8_t   commError;
uint8_t  fallbackFlags;

#ifdef ENABLE_CURRENT
amperageFilter ct[NUM_OUTLETS];
uint8_t outletPtr;
uint8_t valuePtr;
#endif

void main (void) {
    initializeHardware ();
    
#ifdef ENABLE_CURRENT
    outletPtr = 0;
    valuePtr = 0;
 
    int i, j;
    for (i = 0; i < NUM_OUTLETS; ++i) {
        ct[i].sum = 0;
        for (j = 0; j < FILTER_VALUES; ++j)
            ct[i].values[j] = 0;
        ct[i].average = 0;
    }
    
    ct[0].chsValue = CHS_AN0;
    ct[1].chsValue = CHS_AN1;
    ct[2].chsValue = CHS_AN2;
    ct[3].chsValue = CHS_AN3;
    ct[4].chsValue = CHS_AN5;
    ct[5].chsValue = CHS_AN6;
    ct[6].chsValue = CHS_AN7;
    ct[7].chsValue = CHS_AN8;
#endif
    
    fallbackFlags = eeprom_read(OUTLET_FALLBACK_ADDRESS);
    
    /* AquaPicBus Initialization */
    apbInst = &apbInstStruct;
    apb_init(apbInst, 
            &apbMessageHandler, 
            APB_ADDRESS,
            1,
            TX_ENABLE_PORT,
            TX_ENABLE_PIN);
    
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
            apb_run (apbInst, data);
        }
    }
}

void interrupt ISR(void) {
#ifdef ENABLE_CURRENT
    static uint8_t lastPtr = NUM_OUTLETS - 1; //outletPtr starts at 0 so we'll just initialize this to the end
    
    if (ADIF) {
        ct[outletPtr].sum -= ct[outletPtr].values[valuePtr]; //subtract the oldest value from the sum
        ct[outletPtr].values[valuePtr] = getAdc (); //get the new value
        ct[outletPtr].sum += ct[outletPtr].values[valuePtr]; //add the newest value to the sum
        
        ct[outletPtr].average = ct[outletPtr].sum / FILTER_VALUES; //average the sum
        
        increment(outletPtr, NUM_OUTLETS);
        ADCON0bits.CHS = ct[outletPtr].chsValue; //set the ADC to the new channel
        
        if (outletPtr == 0) //we're back to the beginning of the outlets, increment the value array pointer
            increment(valuePtr, FILTER_VALUES);
        
        ADIF = 0; //Clear flag
    }
#endif
    
    if (TMR2IF) {
        TMR2IF = 0; //Clear flag
        apb_framing (apbInst);
    }
    
    if (TMR4IF) {
        TMR4IF = 0; //Clear flag
        
#ifdef ENABLE_CURRENT
        if (outletPtr != lastPtr) { /* the ADC isn't finished so don't start it */
            START_ADC();
            lastPtr = outletPtr;
        }
#endif
        
        if (!commError) {
            ++commCounter;
            
            if (commCounter >= COMM_ERROR_SP) {
                LATD = fallbackFlags; /* set outlets to the fallback */
                commError = -1;
                SET_LED(GREEN_LED_PORT, GREEN_LED_PIN, OFF);
                SET_LED(RED_LED_PORT, RED_LED_PIN, ON);
                apb_restart(apbInst);
            }
        } else {
            if (commCounter == 0) {
                commError = 0;
                SET_LED(RED_LED_PORT, RED_LED_PIN, OFF);
                SET_LED(GREEN_LED_PORT, GREEN_LED_PIN, ON);
            }
        }
    }
}

void initializeHardware(void) {
    /****Oscillator****/
    OSCCONbits.SCS = 0b00;  //System Clock Select: Clock determined by FOSC<2:0> in Configuration Word 1

    /****Port Initialization****/
    PORTA = 0x00;   //Clear Port A
    PORTB = 0x00;   //Clear Port B
    PORTC = 0x03;   //Clear Port C, Write 1 to RG Status LED sinks, ie turn off LEDs
    PORTD = 0x00;   //Clear Port D
    PORTE = 0x00;   //Clear Port E

    /****Port Direction****/
    TRISA = 0b00001111; //Port A Directions
            //****1*** = RA3, AN3, Outlet 4 CT
            //*****1** = RA2, AN2, Outlet 3 CT
            //******1* = RA1, AN1, Outlet 2 CT
            //*******1 = RA0, AN0, Outlet 1 CT

    TRISB = 0b00000100; //Port B Directions
            //*****1** = RB2, AN8, Outlet 8 CT

    TRISC = 0b10001000; //Port C Directions
            //1******* = RC7, RX
            //*0****** = RC6, TX
            //**0***** = RC5, TX_nRX
            //****1*** = RC3, AC Power Available
            //*****0** = RC2, Yellow Status LED
            //******0* = RC1, Green Status LED
            //*******0 = RC0, Red Status LED
    
    TRISD = 0b00000000; //Port D Directions
            //0******* = RD7, Outlet 8 relay
            //*0****** = RD6, Outlet 7 relay
            //**0***** = RD5, Outlet 6 relay
            //***0**** = RD4, Outlet 5 relay
            //****0*** = RD3, Outlet 4 relay
            //*****0** = RD2, Outlet 3 relay
            //******0* = RD1, Outlet 2 relay
            //*******0 = RD0, Outlet 1 relay
    
    TRISE = 0b00000111; //Port E Directions
            //*****1** = RE2, AN7, Outlet 7 CT
            //******1* = RE1, AN6, Outlet 6 CT
            //*******1 = RE0, AN5, Outlet 5 CT

#ifdef ENABLE_CURRENT 
    /****Analog Select****/
    ANSELA = 0b00001111;
             //****1*** = RA3, AN3, Outlet 4 CT
             //*****1** = RA2, AN2, Outlet 3 CT
             //******1* = RA1, AN1, Outlet 2 CT
             //*******1 = RA0, AN0, Outlet 1 CT

    ANSELB = 0b00000100;
             //*****1** = RB2, AN8, Outlet 8 CT
    
    ANSELE = 0b00000111;
             //*****1** = RE2, AN7, Outlet 7 CT
             //******1* = RE1, AN6, Outlet 6 CT
             //*******1 = RE0, AN5, Outlet 5 CT

    initAdc(AD0_CHS_AN0 | AD0_ADON_ENABLE, AD1_ADFM_RIGHT | AD1_ADCS_FOSC_16 | AD1_ADNREF_VSS | AD1_ADPREF_VDD);
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
    
    /****Timer 4****/
    PR4 = 0xC2; //Timer Period = 25mSec
                //Timer Period = [PRx + 1] * 4 * Tosc * TxCKPS * TxOUTPS
                //PRx = [Timer Period / (4 * Tosc * TxCKPS * TxOUTPS)] - 1
                //PRx = 25mSec / (4 * (1 / 32MHz) * 64 * 16)] - 1
                //PRx = 194.3125 or 194 or C2
                //Note: Tosc = 1 / Fosc

    T4CON = 0b01111111;
            //*1111*** = T4OUTPS: Timer Output Postscaler Select, 1:16 Postscaler
            //*****1** = TMR4ON: Timer4 is on
            //******11 = T4CKPS: Timer4-type Clock Prescale Select, Prescaler is 64

    /****UART****/
    /* Set BRG16 to one for fast speed */
    BAUDCONbits.BRG16 = 1; //16-bit Baud Rate Generator, 16-bit Baud Rate Generator is used
    /* Set BRGH to one for fast speed */
    TXSTAbits.BRGH = 1; /* High Baud Rate Select, High speed */
    SPBRGH = 0x00;  //Nothing in the high register
    SPBRGL = 0x8A;  
                    //Desired Baud Rate = ***57,600Mb*** or 115,200Mb
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
#ifdef ENABLE_CURRENT
    ADIF = 0;       /* Clear ADC interrupt flag */
    ADIE = 1;       /* Enable ADC interrupts */
#endif
    TMR2IF = 0;     /* Clear Timer2 interrupt flag */
    TMR2IE = 1;     /* Enable Timer2 interrupts */
    TMR4IF = 0;     /* Clear Timer4 interrupt flag */
    TMR4IE = 1;     /* Enable Timer4 interrupts */
}

void apbMessageHandler(void) {
    commCounter = 0;
    
    switch (apbInst->function) {
        case 2: { /* setup single channel */
            uint8_t outlet   = apbInst->message[3];
            uint8_t fallback = apbInst->message[4];
            bitFlagSet(fallbackFlags, outlet, fallback);
            eeprom_write (OUTLET_FALLBACK_ADDRESS, fallbackFlags);
            apb_sendDefualtResponse(apbInst);
            break;
        }
#ifdef ENABLE_CURRENT
        case 10: { /* read outlet current */
            uint8_t outlet   = apbInst->message[3];
            uint16_t current = ct[outlet].average;
            apb_initResponse(apbInst);
            apb_appendToResponse(apbInst, outlet);
            apb_addToResponse(apbInst, &current, sizeof(uint16_t));
            apb_sendResponse(apbInst);
            break;
        }
#endif
        case 20: { /* read status */
            apb_initResponse(apbInst);
            READ_PIN(POWER_AVAIL_PORT, POWER_AVAIL_PIN) ? apb_appendToResponse(apbInst, 0xFF) : apb_appendToResponse(apbInst, 0x00);
            apb_appendToResponse(apbInst, LATD);
#ifdef ENABLE_CURRENT
            apb_appendToResponse(apbInst, 0xFF);
#else
            apb_appendToResponse(apbInst, 0x00);
#endif
            apb_sendResponse(apbInst);
            break;
        }
#ifdef ENABLE_CURRENT
        case 21: { /* read all current */
            int i;
            apb_initResponse(apbInst);
            for (i = 0; i < NUM_OUTLETS; ++i)
                apb_addToResponse(apbInst, &(ct[i].average), sizeof(uint16_t));
            apb_sendResponse(apbInst);
            break;
        }
#endif
        case 30: { /* write outlet */
            uint8_t outlet = apbInst->message[3];
            uint8_t state  = apbInst->message[4];
            WRITE_PIN(&LATD, outlet, state);
            apb_sendDefualtResponse(apbInst);
            break;
        }
        default:
            break;
    }
}