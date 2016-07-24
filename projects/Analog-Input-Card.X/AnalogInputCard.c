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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>     //For uint8_t, int8_t definition
#include <xc.h>
#include <pic16f1827.h>
#include "../../drv_lib/aquapic_bus/aquapic_bus.h"
#include "../../drv_lib/common/slib_com.h"

/******************************************************************************/
/* CONFIGUTION WORDS                                                          */
/******************************************************************************/
/*CONFIG1*/
#pragma config FOSC = HS        //Oscillator Selection (HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins)
#pragma config WDTE = OFF       //Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      //Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       //MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         //Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        //Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       //Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   //Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        //Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       //Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

/*CONFIG2*/
#pragma config WRT = OFF        //Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       //PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      //Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        //Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF         //Low-Voltage Programming Enable (Low-voltage programming enabled)

/******************************************************************************/
/* EEPROM                                                                     */
/******************************************************************************/

/******************************************************************************/
/* USER DEFINED                                                               */
/******************************************************************************/
#define _XTAL_FREQ      32000000UL  //Used by the __delay_ms(xx) and __delay_us(xx) Methods, 32MHz

#define RED_LED         LATBbits.LATB3
#define GREEN_LED       LATBbits.LATB1
#define YELLOW_LED      LATBbits.LATB0

#define rLedOn          RED_LED = 0
#define rLedOff         RED_LED = 1
#define gLedOn          GREEN_LED = 0
#define gLedOff         GREEN_LED = 1
#define yLedOn          YELLOW_LED = 0
#define yLedOff         YELLOW_LED = 1

#define FILTER_VALUES   10
#define NUM_CHANNELS    4
#define startAdc        GO = 1

#define TX_nRX          LATBbits.LATB4
#define APB_ADDRESS     0x50

#define COMM_ERROR_SP   400 //25mSec timer interrupt, 10 sec alarm
                            //10,000mSec / 25mSec = 200

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
uint16_t getAdc (void);

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
struct apbObjStruct apbInstStruct;
apbObj apbInst = &apbInstStruct;
uint16_t commCounter;
uint8_t  commError;
amperageFilter ct[NUM_CHANNELS];
uint8_t  outletPtr;
uint8_t  valuePtr;

void main (void) {
    initializeHardware ();
    
    outletPtr = 0;
    valuePtr = 0;
 
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
            1);

    //enable UART
    TX_nRX = 0;
    TXSTAbits.TXEN = 1; //Transmit Enable, Transmit enabled
    RCSTAbits.CREN = 1; //Continuous Receive Enable, Enables receiver
    
    /*Global Interrupts*/
    PEIE = 1; //Enable peripheral interrupts
    GIE = 1; //Enable Global interrupts
    
    yLedOff;
    gLedOn;
    
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
        
        ct[outletPtr].sum -= ct[outletPtr].values[valuePtr]; //subtract the oldest value from the sum
        ct[outletPtr].values[valuePtr] = getAdc (); //get the new value
        ct[outletPtr].sum += ct[outletPtr].values[valuePtr]; //add the newest value to the sum
        
        ct[outletPtr].average = ct[outletPtr].sum / FILTER_VALUES; //average the sum
        
        _increment(outletPtr, NUM_CHANNELS);
        ADCON0bits.CHS = ct[outletPtr].chsValue; //set the ADC to the new channel
        
        if (outletPtr == 0) //we're back to the beginning of the outlets, increment the value array pointer
            _increment(valuePtr, FILTER_VALUES);
    }
    
    if (TMR2IF) {
        TMR2IF = 0; /* Clear flag */
        apb_framing (apbInst);
    }
    
    if (TMR4IF) {
        TMR4IF = 0; /* Clear flag */
        
        if (outletPtr != lastPtr) { //the ADC isn't finished so don't start it
            startAdc;
            lastPtr = outletPtr;
        }
        
        if (!commError) {
            ++commCounter;
            
            if (commCounter >= COMM_ERROR_SP) {
                commError = -1;
                gLedOff;
                rLedOn;
                apb_restart (apbInst);
            }
        } else {
            if (commCounter == 0) {
                commError = 0;
                rLedOff;
                gLedOn;
            }
        }
    }
}

void initializeHardware (void) {
    /*Oscillator*/
    OSCCONbits.SCS = 0b00;  //System Clock Select: Clock determined by FOSC<2:0> in Configuration Word 1

    /*Port Initialization*/
    PORTA = 0x00;   //Clear Port A
    PORTB = 0x18;   //Clear Port B, Write 1 to RG Status LED sinks, ie turn off LEDs

    /*Port Direction*/
    TRISA = 0b00001111; //Port A Directions
            //****1*** = RA3, AN3, Input 4
            //*****1** = RA2, AN2, Input 3
            //******1* = RA1, AN1, Input 2
            //*******1 = RA0, AN0, Input 1
    TRISB = 0b00000100; //Port B Directions
            //**0***** = RB5, TX
            //***0**** = RB4, TX_nRX
            //****0*** = RB3, Red Status LED
            //*****1** = RB2, RX
            //******0* = RB1, Green Status LED
            //*******0 = RB0, Yellow Status LED

    /*Analog Select*/
    ANSELA = 0x0F;  //Lower pins are analog
    ANSELB = 0x00;  //All digital ports

    /*Port Selection*/
    APFCON0bits.RXDTSEL = 1; //RX function is on RB2
    APFCON1bits.TXCKSEL = 1; //TX function is on RB5
    
    /*ADC*/
    ADCON0 = 0b00000001;
             //*00000** = CHS: Analog Channel Select bits, AN0
             //*******1 = ADON: ADC Enable bit, ADC is enabled

    ADCON1 = 0b11010100;
             //1******* = ADFM: A/D Result Format Select, Right Justified
             //*101**** = ADCS: A/D Conversion Clock Select, FOSC/16
             //*****0** = ADNREF: A/D Negative Voltage Reference Configuration, VREF- is connected to Vss
             //******00 = ADPREF: A/D Positive Voltage Reference Configuration, VREF+ is connected to Vdd
    
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
    
    /*Timer 4*/
    PR4 = 0xC2; //Timer Period = 25mSec
                //Timer Period = [PRx + 1] * 4 * Tosc * TxCKPS * TxOUTPS
                //PRx = [Timer Period / (4 * Tosc * TxCKPS * TxOUTPS)] - 1
                //PRx = 25mSec / (4 * (1 / 32MHz) * 64 * 16)] - 1
                //PRx = 194.3125 or 194 or C2
                //Note: Tosc = 1 / Fosc

    T4CON = 0b01111111;
            //*1111*** = T2OUTPS: Timer Output Postscaler Select, 1:16 Postscaler
            //*****1** = TMR2ON: Timer2 is on
            //******11 = T2CKPS: Timer2-type Clock Prescale Select, Prescaler is 64

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
    TMR4IF = 0;     /* Clear Timer4 interrupt flag */
    TMR4IE = 1;     /* Enable Timer2 interrupts */
}

void apbMessageHandler (void) {
    commCounter = 0;
    
    switch (apbInst->function) {
        case 10: { //read single channel value
            #define FUNCTION10_LENGTH 8 //header + channel + value + crc = 3 + 1 + 2 + 2

            uint8_t channel = apbInst->message [3];
            uint16_t value  = ct[channel].average;
            
            uint8_t m [FUNCTION10_LENGTH];
            uint8_t crc [2];

            m [0] = apbInst->address;
            m [1] = 10; //function number
            m [2] = FUNCTION10_LENGTH;  //message length
            m [3] = channel;
            memoryCopy (&(m [4]), &value, sizeof (uint16_t));
            apb_crc16 (m, crc, FUNCTION10_LENGTH);
            m [FUNCTION10_LENGTH - 2] = crc [0];
            m [FUNCTION10_LENGTH - 1] = crc [1];

            writeUartData (m, FUNCTION10_LENGTH);

            break;
        }
        case 20: { //read all channels values
            #define FUNCTION20_LENGTH 13 //header + value * 4 + crc = 3 + 2 * 4 + 2
            
            uint16_t values [NUM_CHANNELS];
            uint8_t m [FUNCTION20_LENGTH];
            uint8_t crc [2];

            int i;
            for (i = 0; i < NUM_CHANNELS; ++i) 
                values [i] = ct[i].average;

            m [0] = apbInst->address;
            m [1] = 20; //function number
            m [2] = FUNCTION20_LENGTH; //message length
            memoryCopy (&(m [3]), values, sizeof (uint16_t) * NUM_CHANNELS);
            apb_crc16 (m, crc, FUNCTION20_LENGTH);
            m [FUNCTION20_LENGTH - 2] = crc [0];
            m [FUNCTION20_LENGTH - 1] = crc [1];

            writeUartData (m, FUNCTION20_LENGTH);

            break;
        }
        default:
            break;
    }
}

void writeUartData (uint8_t* data, uint8_t length) {
    TX_nRX = 1; //RS-485 chip transmit enable is high
    
    int i;
    for (i = 0; i < length; ++i) {
        TXREG = *data;
        ++data;
        while (!TXIF) //TXIF is set when the TXREG is empty
            continue;
    }

    while (!TRMT) //wait for transmit shift register to be empty
        continue;
    
    TX_nRX = 0; //RS-485 chip receive enable is low
}

void sendDefualtResponse (void) {
    uint8_t* m = apb_buildDefualtResponse (apbInst);
    writeUartData (m, 5);
}

void memoryCopy (void* to, void* from, size_t count) {
    uint8_t* ptr_to = (uint8_t*)to;
    uint8_t* ptr_from = (uint8_t*)from;

    while (count--)
        *ptr_to++ = *ptr_from++;
}

uint16_t getAdc (void) {
    uint16_t counts;

    counts = ADRESH << 8; //grab highest 2 bits shift right 8
    counts |= ADRESL;

    return counts;
}