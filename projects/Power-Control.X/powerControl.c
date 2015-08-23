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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>     //For uint8_t, int8_t definition
#include <xc.h>
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
#pragma config VCAPEN = RA5     //Voltage Regulator Capacitor Enable (VCAP functionality is enabled on RA5)
#pragma config PLLEN = ON       //PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      //Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        //Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        //Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

/******************************************************************************/
/* EEPROM                                                                     */
/******************************************************************************/
/*HEX FILE EEPROM INITIAL VALUES*/
__EEPROM_DATA (0,0,0,0,0,0,0,0);

/*Define eeprom read and write functions from xc.h*/
unsigned char eeprom_read (unsigned char address);
void eeprom_write (unsigned char address, unsigned char value);

/*User Defined*/
#define OUTLET_FALLBACK_ADDRESS 0x00

/******************************************************************************/
/* USER DEFINED                                                               */
/******************************************************************************/
#define _XTAL_FREQ      32000000UL  //Used by the __delay_ms(xx) and __delay_us(xx) Methods, 32MHz

#define AC_POWER_AVAIL  PORTCbits.RC3

#define OUTLET1_RELAY   LATDbits.LATD0
#define OUTLET2_RELAY   LATDbits.LATD1
#define OUTLET3_RELAY   LATDbits.LATD2
#define OUTLET4_RELAY   LATDbits.LATD3
#define OUTLET5_RELAY   LATDbits.LATD4
#define OUTLET6_RELAY   LATDbits.LATD5
#define OUTLET7_RELAY   LATDbits.LATD6
#define OUTLET8_RELAY   LATDbits.LATD7

#define startAdc        GO = 1

#define RED_LED         LATCbits.LATC0
#define GREEN_LED       LATCbits.LATC1
#define YELLOW_LED      LATCbits.LATC2

#define rLedOn          RED_LED = 0
#define rLedOff         RED_LED = 1
#define gLedOn          GREEN_LED = 0
#define gLedOff         GREEN_LED = 1
#define yLedOn          YELLOW_LED = 0
#define yLedOff         YELLOW_LED = 1

#define TX_nRX          LATCbits.LATC5
#define APB_ADDRESS     0x10

#define FILTER_VALUES   10
#define NUM_OUTLETS     8

#define COMM_ERROR_SP   200 //25mSec timer interrupt, 5 sec alarm
                            //5000mSec / 25mSec = 200

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
void enableAddressDetection (void);
void disableAddressDetection (void);
void memoryCopy (void* to, void* from, size_t count);
uint16_t getAdc (void);

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
apb_obj apbInst;
amperageFilter ct[NUM_OUTLETS];
uint8_t outletPtr;
uint8_t valuePtr;
uint8_t commCounter;
uint8_t commError;
uint8_t fallbackFlags;

void main (void) {
    initializeHardware ();
    
    outletPtr = 0;
    valuePtr = 0;
    
    int i, j;
    for (i = 0; i < NUM_OUTLETS; ++i) {
        ct[i].sum = 0;
        for (j = 0; j < FILTER_VALUES; ++j)
            ct[i].values[j] = 0;
        ct[i].average = 0;
    }
    
    ct[0].chsValue = 0b00000;
    ct[1].chsValue = 0b00001;
    ct[2].chsValue = 0b00010;
    ct[3].chsValue = 0b00011;
    ct[4].chsValue = 0b00101;
    ct[5].chsValue = 0b00110;
    ct[6].chsValue = 0b00111;
    ct[7].chsValue = 0b01000;
    
    fallbackFlags = eeprom_read (OUTLET_FALLBACK_ADDRESS);
    
    //AquaPic Bus initialization
    apbInst = apb_new ();
    apb_init (apbInst, &apbMessageHandler, &enableAddressDetection, &disableAddressDetection, APB_ADDRESS);
    
    //enable UART
    TX_nRX = 0;
    TXSTAbits.TXEN = 1; //Transmit Enable, Transmit enabled
    RCSTAbits.CREN = 1; //Continuous Receive Enable, Enables receiver
    
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
    static uint8_t lastPtr = NUM_OUTLETS - 1; //outletPtr starts at 0 so we'll just initialize this to the end
    
    if (ADIF) {
        ct[outletPtr].sum -= ct[outletPtr].values[valuePtr]; //subtract the oldest value from the sum
        ct[outletPtr].values[valuePtr] = getAdc (); //get the new value
        ct[outletPtr].sum += ct[outletPtr].values[valuePtr]; //add the newest value to the sum
        
        ct[outletPtr].average = ct[outletPtr].sum / FILTER_VALUES; //average the sum
        
        _increment(outletPtr, NUM_OUTLETS);
        ADCON0bits.CHS = ct[outletPtr].chsValue; //set the ADC to the new channel
        
        if (outletPtr == 0) //we're back to the beginning of the outlets, increment the value array pointer
            _increment(valuePtr, FILTER_VALUES);
        
        ADIF = 0; //Clear flag
    }
    
    if (TMR4IF) {
        if (outletPtr != lastPtr) { //the ADC isn't finished so don't start it
            startAdc;
            lastPtr = outletPtr;
        }
        
        if (!commError) {
            ++commCounter;
            
            if (commCounter >= COMM_ERROR_SP) {
                LATD = fallbackFlags; //set outlets to the fallback
                commError = 1;
                gLedOff;
                rLedOn;
            }
        } else {
            if (commCounter == 0) {
                commError = 0;
                rLedOff;
                gLedOn;
            }
        }
        
        TMR4IF = 0; //Clear flag
    }
}

void initializeHardware (void) {
    /*Oscillator*/
    OSCCONbits.SCS = 0b00;  //System Clock Select: Clock determined by FOSC<2:0> in Configuration Word 1

    /*Port Initialization*/
    PORTA = 0x00;   //Clear Port A
    PORTB = 0x00;   //Clear Port B
    PORTC = 0x03;   //Clear Port C, Write 1 to RG Status LED sinks, ie turn off LEDs
    PORTD = 0x00;   //Clear Port D
    PORTE = 0x00;   //Clear Port E

    /*Port Direction*/
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
            //***0**** = RD5, Outlet 5 relay
            //****0*** = RD3, Outlet 4 relay
            //*****0** = RD2, Outlet 3 relay
            //******0* = RD1, Outlet 2 relay
            //*******0 = RD0, Outlet 1 relay
    
    TRISE = 0b00000111; //Port E Directions
            //*****1** = RE2, AN7, Outlet 7 CT
            //******1* = RE1, AN6, Outlet 6 CT
            //*******1 = RE0, AN5, Outlet 5 CT

    /*Analog Select*/
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
    
    /*ADC*/
    ADCON0 = 0b00000001;
             //*00000** = CHS: Analog Channel Select bits, AN0
             //*******1 = ADON: ADC Enable bit, ADC is enabled

    ADCON1 = 0b11010100;
             //1******* = ADFM: A/D Result Format Select, Right Justified
             //*110**** = ADCS: A/D Conversion Clock Select, FOSC/32
             //*****0** = ADNREF: A/D Negative Voltage Reference Configuration, VREF- is connected to Vss
             //******00 = ADPREF: A/D Positive Voltage Reference Configuration, VREF+ is connected to Vdd
    
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
    //Set BRG16 to one for fast speed
    BAUDCONbits.BRG16 = 1; //16-bit Baud Rate Generator, 16-bit Baud Rate Generator is used
    //Set BRGH to one for fast speed
    TXSTAbits.BRGH = 1; //High Baud Rate Select, High speed
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

    TXSTAbits.TX9 = 1; //9-bit Transmit Enable, De-Selects 9-bit transmission
    RCSTA = 0b11001000;
            //1******* = SPEN: Serial Port Enable, Serial port enabled
            //*1****** = RX9: 9-bit Receive Enable, Selects 9-bit reception
            //****1*** = ADDEN: Address Detect Enable, Enables address detection
    
    ADIF = 0; //Clear ADC interrupt flag
    ADIE = 1; //Enable ADC interrupts
    
    TMR4IF = 0; //Clear Timer4 interrupt flag
    TMR4IE = 1; //Enable Timer4 interrupts

    /*Global Interrupts*/
    PEIE = 1; //Enable peripheral interrupts
    GIE = 1; //Enable Global interrupts
}

void apbMessageHandler (void) {
    commCounter = 0;
    
    switch (apbInst->function) {
        case 2: { //setup single channel
            uint8_t outlet   = apbInst->message [3];
            uint8_t fallback = apbInst->message [4];

            flagSet(fallbackFlags, outlet, fallback);

            eeprom_write (OUTLET_FALLBACK_ADDRESS, fallbackFlags);
            sendDefualtResponse ();
            break;
        }
        case 10: { //read outlet current
#define FUNCTION10_LENGTH 8 //header + outlet + current + crc = 3 + 1 + 2 + 2
            
            uint8_t outlet = apbInst->message[3];
            uint16_t current  = ct[outlet].average;
            
            uint8_t m[FUNCTION10_LENGTH];
            uint8_t crc[2];

            m[0] = apbInst->address;
            m[1] = 10; //function number
            m[2] = FUNCTION10_LENGTH;  //message length
            m[3] = outlet;
            memoryCopy (&(m[4]), &current, sizeof (uint16_t));
            apb_crc16 (m, crc, FUNCTION10_LENGTH);
            m[FUNCTION10_LENGTH - 2] = crc[0];
            m[FUNCTION10_LENGTH - 1] = crc[1];
            
            writeUartData (m, FUNCTION10_LENGTH);
            
            break;
        }
        case 20: { //read status
#define FUNCTION20_LENGTH 7 //header + ac power avail + current mask + crc = 3 + 1 + 1 + 2
            
            uint8_t m[FUNCTION20_LENGTH];
            uint8_t crc[2];
            
            m[0] = apbInst->address;
            m[1] = 20; //function number
            m[2] = FUNCTION20_LENGTH;  //message length
            if (AC_POWER_AVAIL)
                m[3] = 0xFF;
            else
                m[3] = 0x00;
            m[4] = 0xFF;
            apb_crc16 (m, crc, FUNCTION20_LENGTH);
            m[FUNCTION20_LENGTH - 2] = crc[0];
            m[FUNCTION20_LENGTH - 1] = crc[1];
            
            writeUartData (m, FUNCTION20_LENGTH);
            
            break;
        }
        case 21: { //read all current
#define FUNCTION21_LENGTH 21 //header + 8 * current + crc = 3 + (8 * 2) + 2
            
            uint8_t m[FUNCTION21_LENGTH];
            uint8_t crc[2];
            
            m[0] = apbInst->address;
            m[1] = 21; //function number
            m[2] = FUNCTION21_LENGTH; //message length
            int i;
            for (i = 0; i < NUM_OUTLETS; ++i)
                memoryCopy (&(m[i * sizeof (uint16_t) + 3]), &(ct[i].average), sizeof (uint16_t));
            apb_crc16 (m, crc, FUNCTION21_LENGTH);
            m[FUNCTION21_LENGTH - 2] = crc[0];
            m[FUNCTION21_LENGTH - 1] = crc[1];
            
            writeUartData (m, FUNCTION21_LENGTH);
            
            break;
        }
        case 30: { //write outlet
            uint8_t outlet = apbInst->message [3];
            uint8_t state  = apbInst->message [4];
            
            uint8_t dLat = LATD;
            flagSet(dLat, outlet, state);
            LATD = dLat;
            
            sendDefualtResponse ();
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
    uint8_t* m = apb_build_defualt_response (apbInst);
    writeUartData (m, 5);
}

void enableAddressDetection (void) {
    RCSTAbits.ADDEN = 1;
}

void disableAddressDetection (void) {
    RCSTAbits.ADDEN = 0;
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