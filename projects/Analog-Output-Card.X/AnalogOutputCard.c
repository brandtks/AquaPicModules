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
/*HEX FILE EEPROM INITIAL VALUES*/
__EEPROM_DATA (0,0,0,0,0,0,0,0);

/*Define eeprom read and write functions from xc.h*/
unsigned char eeprom_read (unsigned char address);
void eeprom_write (unsigned char address, unsigned char value);

/*User Defined*/
#define CHANNEL_1_ADDRESS 0x00
#define CHANNEL_2_ADDRESS 0x01
#define CHANNEL_3_ADDRESS 0x02
#define CHANNEL_4_ADDRESS 0x03

/******************************************************************************/
/* USER DEFINED                                                               */
/******************************************************************************/
#define _XTAL_FREQ      32000000UL  //Used by the __delay_ms(xx) and __delay_us(xx) Methods, 32MHz

#define CHANNEL_1_RELAY LATBbits.LATB1
#define CHANNEL_2_RELAY LATBbits.LATB2
#define CHANNEL_3_RELAY LATBbits.LATB3
#define CHANNEL_4_RELAY LATBbits.LATB4

#define RED_LED         LATAbits.LATA0
#define GREEN_LED       LATAbits.LATA1
#define YELLOW_LED      LATAbits.LATA2

#define rLedOn          RED_LED = 0
#define rLedOff         RED_LED = 1
#define gLedOn          GREEN_LED = 0
#define gLedOff         GREEN_LED = 1
#define yLedOn          YELLOW_LED = 0
#define yLedOff         YELLOW_LED = 1

#define TX_nRX          LATCbits.LATC5
#define APB_ADDRESS     0x20

#define COMM_ERROR_SP   400 //25mSec timer interrupt, 10 sec alarm
                            //10,000mSec / 25mSec = 400

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
typedef struct pwm_port {
    volatile uint8_t* ccpr;
    volatile uint8_t* ccpcon;
}pwmPort;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void initializeHardware (void);
void apbMessageHandler (void);
void writeUartData (uint8_t* data, uint8_t length);
void sendDefualtResponse (void);
void memoryCopy (void* to, void* from, size_t count);
uint16_t getChannelValue (uint8_t channel);
void setChannelValue (uint8_t channel, uint16_t value);

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
struct apbObjStruct apbInstStruct;
apbObj apbInst = &apbInstStruct;
pwmPort  pwm [4];
uint16_t commCounter;
uint8_t  commError;

void main (void) {
    initializeHardware ();

    pwm [0].ccpr = &CCPR1L;
    pwm [0].ccpcon = &CCP1CON;
    pwm [1].ccpr = &CCPR2L;
    pwm [1].ccpcon = &CCP2CON;
    pwm [2].ccpr = &CCPR3L;
    pwm [2].ccpcon = &CCP3CON;
    pwm [3].ccpr = &CCPR4L;
    pwm [3].ccpcon = &CCP4CON;

    int i;
    for (i = 0; i < 4; ++i) {
        uint8_t type = eeprom_read (i);

        //type 255 is PWM output so close relay to bypass filter
        if (type == 255)
            PORTB |= 1 << (i + 1);
    }

    //AquaPic Bus initialization
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
    if (TMR6IF) {
        TMR6IF = 0; //Clear flag
        apb_framing (apbInst);
    }
    
    if (TMR4IF) {
        TMR4IF = 0; //Clear flag
        
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
    PORTA = 0x03;   //Clear Port A, Write 1 to RG Status LED sinks, ie turn off LEDs
    PORTB = 0x00;   //Clear Port B
    PORTC = 0x00;   //Clear Port C

    /*Port Direction*/
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

    /*Analog Select*/
    ANSELA = 0x00;  //All digital ports
    ANSELB = 0x00;  //All digital ports

    /*Port Selection*/
    APFCONbits.CCP3SEL = 1; //CCP3:P3A function is on RB5
    APFCONbits.CCP2SEL = 0; //CCP2:P2A function is on

    /*CCP(PWM)*/
    PR2 = 0xFF; //31.25 kHz from data sheet page 218

    /*CCP1*/
    CCP1CON = 0b00001100;
              //00****** = P1M: Enhanced PWM Output Configuration, Single output; PxA modulated; PxB, PxC, PxD assigned as port pins
              //**00**** = DC1B: PWM Duty Cycle Least Significant bits
              //****1100 = CCP1M: ECCP1 Mode Select, PWM mode
    CCPR1L = 0x00;  //Duty Cycle set to 0

    /*CCP2*/
    CCP2CON = 0b00001100;
              //00****** = P2M: Enhanced PWM Output Configuration, Single output; PxA modulated; PxB, PxC, PxD assigned as port pins
              //**00**** = DC2B: PWM Duty Cycle Least Significant bits
              //****1100 = CCP2M: ECCP2 Mode Select, PWM mode
    CCPR2L = 0x00;  //Duty Cycle set to 0;

    /*CCP3*/
    CCP3CON = 0b00001100;
              //00****** = P3M: Enhanced PWM Output Configuration, Single output; PxA modulated; PxB, PxC, PxD assigned as port pins
              //**00**** = DC3B: PWM Duty Cycle Least Significant bits
              //****1100 = CCP3M: ECCP3 Mode Select, PWM mode
    CCPR3L = 0x00;  //Duty Cycle set to 0;

    /*CCP4*/
    CCP4CON = 0b00001100;
              //00****** = P4M: Enhanced PWM Output Configuration, Single output; PxA modulated; PxB, PxC, PxD assigned as port pins
              //**00**** = DC4B: PWM Duty Cycle Least Significant bits
              //****1100 = CCP4M: CCP4 Mode Select, PWM mode
    CCPR4L = 0x00;  //Duty Cycle set to 0;

    CCPTMRS0 = 0b00000000;  //CCP<1:4> Modules based on Timer2

    T2CON = 0b00000100;
            //*0000*** = T2OUTPS: Timer2 Output Postscaler Select, Does Not Matter for PWM Period
            //*****1** = TMR2ON: Timer2 is on
            //******00 = T2CKPS: Timer2-type Clock Prescale Select, Prescaler is 1

    TRISCbits.TRISC2 = 0; //Clear TRISC2 bit to enable PWM1 output on pin
    TRISCbits.TRISC1 = 0; //Clear TRISC1 bit to enable PWM2 output on pin
    TRISBbits.TRISB5 = 0; //Clear TRISB5 bit to enable PWM3 output on pin
    TRISBbits.TRISB0 = 0; //Clear TRISB0 bit to enable PWM4 output on pin
    
    /*Timer 4*/
    PR4 = 0xC2; //Timer Period = 25mSec
                //Timer Period = [PRx + 1] * 4 * Tosc * TxCKPS * TxOUTPS
                //PRx = [Timer Period / (4 * Tosc * TxCKPS * TxOUTPS)] - 1
                //PRx = 25mSec / (4 * (1 / 32MHz) * 64 * 16)] - 1
                //PRx = 194.3125 or 194 or C2
                //Note: Tosc = 1 / Fosc

    T4CON = 0b01111111;
            //*1111*** = T4OUTPS: Timer Output Postscaler Select, 1:16 Postscaler
            //*****1** = TMR4ON: Timer4 is on
            //******11 = T4CKPS: Timer2-type Clock Prescale Select, Prescaler is 64
    
    /****Timer 6****/
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
    TMR4IF = 0;     /* Clear Timer4 interrupt flag */
    TMR4IE = 1;     /* Enable Timer2 interrupts */
    TMR6IF = 0;     /* Clear Timer6 interrupt flag */
    TMR6IE = 1;     /* Enable Timer6 interrupts */
}

void apbMessageHandler (void) {
    commCounter = 0;
    
    switch (apbInst->function) {
#if SELECTABLE_OUTPUT
        case 2: { //setup single channel
            uint8_t channel = apbInst->message [3] + 1; //0 is channel 1
            uint8_t type    = apbInst->message [4];

            //type 255 is PWM output so close relay to bypass filter
            if (type == 255)
                PORTB |= 1 << channel;
            else
                PORTB &= ~(1 << channel);

            eeprom_write (channel - 1, type);

            sendDefualtResponse ();

            break;
        }
#endif
        case 10: { //read single channel value
            uint8_t channel = apbInst->message [3];
            uint16_t value  = getChannelValue (channel);
            
            uint8_t m [7];
            uint8_t crc [2];

            m [0] = apbInst->address;
            m [1] = 10; //function number
            m [2] = 7;  //message length
            memoryCopy (&(m [3]), &value, sizeof (uint16_t));
            apb_crc16 (m, crc, 7);
            m [5] = crc [0];
            m [6] = crc [1];

            writeUartData (m, 7);

            break;
        }
        case 20: { //read all channels values
            uint16_t values [4];

            int i;
            for (i = 0; i < 4; ++i) 
                values [i] = getChannelValue (i);

            uint8_t m [13];
            uint8_t crc [2];

            m [0] = apbInst->address;
            m [1] = 20; //function number
            m [2] = 13; //message length
            memoryCopy (&(m [3]), values, sizeof (uint16_t) * 4);
            apb_crc16 (m, crc, 13);
            m [11] = crc [0];
            m [12] = crc [1];

            writeUartData (m, 13);

            break;
        }
        case 30: { //write all channels values
            uint16_t values [4];
            memoryCopy (values, &(apbInst->message[3]), sizeof (uint16_t) * 4);

            int i;
            for (i = 0; i < 4; ++i)
                setChannelValue (i, values [i]);

            sendDefualtResponse ();

            break;
        }
        case 31: { //write single channel value
            uint8_t channel = apbInst->message [3];
            uint16_t value;
            memoryCopy (&value, &(apbInst->message[4]), sizeof (uint16_t));
            
            //value = (value >> 8) | (value << 8); // reverse the endianess
            
            setChannelValue (channel, value);

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
    uint8_t* m = apb_buildDefualtResponse (apbInst);
    writeUartData (m, 5);
}

void memoryCopy (void* to, void* from, size_t count) {
    uint8_t* ptr_to = (uint8_t*)to;
    uint8_t* ptr_from = (uint8_t*)from;

    while (count--)
        *ptr_to++ = *ptr_from++;
}

uint16_t getChannelValue (uint8_t channel) {
    uint16_t value = *(pwm [channel].ccpr) << 2;
    value |= (*(pwm [channel].ccpcon) & 0x30) >> 4; //bits 5:4 in CCPxCON are the two LSBs
    return value;
}

void setChannelValue (uint8_t channel, uint16_t value) {
    uint8_t upper = (uint8_t)((value >> 2) & 0xFF);
    *(pwm [channel].ccpr) = upper;

    if (value & 0x01) //if LSB set bit 4 in CCPxCON register
        *(pwm [channel].ccpcon) |= 0x10;
    else
        *(pwm [channel].ccpcon) &= ~0x10;

    if (value & 0x02) //if 2nd LSD set bit 5 in CCPxCON register
        *(pwm [channel].ccpcon) |= 0x20;
    else
        *(pwm [channel].ccpcon) &= ~0x20;
}