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
#pragma config LVP = OFF         //Low-Voltage Programming Enable (Low-voltage programming enabled)

/******************************************************************************/
/* EEPROM                                                                     */
/******************************************************************************/

/******************************************************************************/
/* USER DEFINED                                                               */
/******************************************************************************/
#define _XTAL_FREQ      32000000UL  //Used by the __delay_ms(xx) and __delay_us(xx) Methods, 32MHz

#define RED_LED         LATAbits.LATA0
#define GREEN_LED       LATAbits.LATA1
#define YELLOW_LED      LATAbits.LATA2

#define rLedOn          RED_LED = 0
#define rLedOff         RED_LED = 1
#define gLedOn          GREEN_LED = 0
#define gLedOff         GREEN_LED = 1
#define yLedOn          YELLOW_LED = 0
#define yLedOff         YELLOW_LED = 1

#define NUM_CHANNELS    6

#define INPUT1          PORTBbits.RB0
#define INPUT2          PORTBbits.RB1
#define INPUT3          PORTBbits.RB2
#define INPUT4          PORTBbits.RB3
#define INPUT5          PORTBbits.RB4
#define INPUT6          PORTBbits.RB5

#define CH1_LED         LATAbits.LATA3
#define CH2_LED         LATAbits.LATA4
#define CH3_LED         LATCbits.LATC0
#define CH4_LED         LATCbits.LATC1
#define CH5_LED         LATCbits.LATC2
#define CH6_LED         LATCbits.LATC3

#define TX_nRX          LATCbits.LATC5
#define APB_ADDRESS     0x30

#define COMM_ERROR_SP   200 //25mSec timer interrupt, 5 sec alarm
                            //5000mSec / 25mSec = 200

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/

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

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
apb_obj apbInst;
uint8_t commCounter;
uint8_t commError;

void main (void) {
    initializeHardware ();

    //AquaPic Bus initialization
    apbInst = apb_new ();
    apb_init (apbInst, &apbMessageHandler, &enableAddressDetection, &disableAddressDetection, APB_ADDRESS);

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
        if (INPUT1) {
            CH1_LED = 1;
        } else {
            CH1_LED = 0;
        }
        
        if (INPUT2) {
            CH2_LED = 1;
        } else {
            CH2_LED = 0;
        }
        
        if (INPUT3) {
            CH3_LED = 1;
        } else {
            CH3_LED = 0;
        }
        
        if (INPUT4) {
            CH4_LED = 1;
        } else {
            CH4_LED = 0;
        }
        
        if (INPUT5) {
            CH5_LED = 1;
        } else {
            CH5_LED = 0;
        }
        
        if (INPUT6) {
            CH6_LED = 1;
        } else {
            CH6_LED = 0;
        }
        
        /*RCIF is set regardless of the global interrupts*/
        /*apb_run might take a while so putting it in the main "loop" makes more sense*/
        if (RCIF) {
            uint8_t data = RCREG;
            apb_run (apbInst, data);
        }
    }
}

void interrupt ISR (void) {
    if (TMR4IF) {
        if (!commError) {
            ++commCounter;
            
            if (commCounter >= COMM_ERROR_SP) {
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

    TMR4IF = 0; //Clear Timer4 interrupt flag
    TMR4IE = 1; //Enable Timer4 interrupts

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

    TXSTAbits.TX9 = 1; //9-bit Transmit Enable, De-Selects 9-bit transmission
    RCSTA = 0b11001000;
            //1******* = SPEN: Serial Port Enable, Serial port enabled
            //*1****** = RX9: 9-bit Receive Enable, Selects 9-bit reception
            //****1*** = ADDEN: Address Detect Enable, Enables address detection
}

void apbMessageHandler (void) {
    commCounter = 0;
    
    switch (apbInst->function) {
        case 10: { //read single channel value
            #define FUNCTION10_LENGTH 7 //header + channel + value + crc = 3 + 1 + 1 + 2

            uint8_t channel = apbInst->message [3];
            uint16_t value  = PORTB & (0x01 << channel);
            
            uint8_t m [FUNCTION10_LENGTH];
            uint8_t crc [2];

            m [0] = apbInst->address;
            m [1] = 10; //function number
            m [2] = FUNCTION10_LENGTH;  //message length
            m [3] = channel;
            m [4] = value;
            apb_crc16 (m, crc, FUNCTION10_LENGTH);
            m [FUNCTION10_LENGTH - 2] = crc [0];
            m [FUNCTION10_LENGTH - 1] = crc [1];

            writeUartData (m, FUNCTION10_LENGTH);

            break;
        }
        case 20: { //read all channels values
            #define FUNCTION20_LENGTH 6 //header + values + crc = 3 + 1 + 2
            
            uint8_t values = 0x00;
            uint8_t m [FUNCTION20_LENGTH];
            uint8_t crc [2];
            
            int i;
            for (i = 0; i < NUM_CHANNELS; ++i) {
                values |= (PORTB & (0x01 << i));
            }

            m [0] = apbInst->address;
            m [1] = 20; //function number
            m [2] = FUNCTION20_LENGTH; //message length
            m [3] = values;
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