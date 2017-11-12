/*
 * Analog Input Card Version 2
 * 
 * Copyright (c) 2017 Goodtime Development
 *  
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * Optionally you can also view the license at <http://www.gnu.org/licenses/>.
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>
#include <stdlib.h>     /* null */
#include <stdint.h>     /* uint8_t, int8_t */
#include "config.h"
#include "../../lib/aquaPicBus/aquaPicBus.h"
#include "../../lib/common/common.h"
#include "../../lib/led/led.h"
#include "../../lib/timer/timer.h"
#include "../../lib/uart/uart.h"
#include "../../lib/i2c/i2c.h"

/******************************************************************************/
/* EEPROM                                                                     */
/******************************************************************************/

/******************************************************************************/
/* USER DEFINED                                                               */
/******************************************************************************/
#define _XTAL_FREQ      32000000UL  /* Used by the __delay_ms(xx) and __delay_us(xx) Methods, 32MHz */

/* Port Initialization settings */
#define PORTA_VAR       0b00000011  /* Clear Port A except for LEDs, 1 is off, 0 is on */
                        /******0**  RA2: Yellow LED off        */
                        /*******1*  RA1: Green LED off      */
                        /********1  RA0: Red LED on      */
#define PORTB_VAR       0x00        /* Clear Port B         */
#define PORTC_VAR       0x00        /* Clear Port C         */

#define TRISA_VAR       0b00000000  /* Port A Directions    */
                        /******0**  RA2: Yellow LED            */
                        /*******0*  RA1: Green LED          */
                        /********0  RA0: Red LED         */
#define TRISB_VAR       0x00        /* Port B Directions    */
#define TRISC_VAR       0b10011000  /* Port C Directions    */
                        /*1*******  RC7: RX                 */
                        /**0******  RC6: TX                 */
                        /***0*****  RC5: TX_nRX             */
                        /****1****  RC4: SDA                */
                        /*****1***  RC3: SCL                */

/* General Outputs and Inputs */
#define RED_LED_PORT    &LATA
#define GREEN_LED_PORT  &LATA
#define YELLOW_LED_PORT &LATA

#define RED_LED_PIN     0
#define GREEN_LED_PIN   1
#define YELLOW_LED_PIN  2

/* AquaPic Bus Settings */
#define APB_ADDRESS     0x50
#define FRAMING_TIMER   1   /* Framing called from timer 2 at 1mSec */
#define ERROR_TIME      10  /* 10 sec alarm   */
#define TX_ENABLE_PORT  &LATC
#define TX_ENABLE_PIN   5

/*  */
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
void initializeHardware(void);
void apbMessageHandler(void);
void setTransmitPin(uint8_t value);

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/
struct apbObjStruct apbInstStruct;
apbObj apbInst = &apbInstStruct;
uint16_t commCounter;
uint8_t  apbError;

struct i2cObjStruct i2cInstStruct;
i2cObj i2cInst = &i2cInstStruct;

amperageFilter ct[NUM_CHANNELS];
uint8_t  inletPtr;
uint8_t  valuePtr;

void main(void) {
    initializeHardware();
    
    inletPtr = 0;
    valuePtr = 0;
    
    int i, j;
    for (i = 0; i < NUM_CHANNELS; ++i) {
        ct[i].sum = 0;
        for (j = 0; j < FILTER_VALUES; ++j)
            ct[i].values[j] = 0;
        ct[i].average = 0;
    }

    /* AquaPic Bus initialization */
    apb_init(apbInst, 
            &apbMessageHandler, 
            &setTransmitPin,
            APB_ADDRESS,
            FRAMING_TIMER,
            ERROR_TIME);

    uart_enable();
    i2c_enable(i2cInst);
    
    /* Interrupts */
    /* Clear Timer2 interrupt flag */
    TMR2IF = 0;     
    /* Enable Timer2 interrupts */
    TMR2IE = 1;   
    /* Clear SSP1 interrupt flag */
    SSP1IF = 0;
    /* Enable SSP1 interrupts */
    SSP1IE = 0;
    /* Enable peripheral interrupts */
    PEIE = 1;
    /* Enable Global interrupts */  
    GIE = 1;    
    
    SET_LED(YELLOW_LED_PORT, YELLOW_LED_PIN, OFF);
    SET_LED(GREEN_LED_PORT, GREEN_LED_PIN, ON);
    
    while (1) {
        /* RCIF is set regardless of the global interrupts. apb_run might     */
        /* take a while so putting it in the main "loop" makes more sense     */
        if (RCIF) {
            uint8_t data = RCREG;
            apb_run(apbInst, data);
        }
        
        if (apb_errorChecking(apbInst)) {
            SET_LED(GREEN_LED_PORT, GREEN_LED_PIN, OFF);
            SET_LED(RED_LED_PORT, RED_LED_PIN, ON);
        } else {
            SET_LED(RED_LED_PORT, RED_LED_PIN, OFF);
            SET_LED(GREEN_LED_PORT, GREEN_LED_PIN, ON);
        }
    }
}

void interrupt ISR (void) {
    /* Timer 2 */
    if (TMR2IF) {
        /* Clear flag */
        TMR2IF = 0; 
        apb_framing(apbInst);
    }
    
    /* SSP1 */
    if (SSP1IF) {
        /* Clear flag */
        SSP1IF = 0;
        i2c_isr(i2cInst);
    }
}

void initializeHardware (void) {
    PORTA = PORTA_VAR;
    PORTB = PORTB_VAR;
    PORTC = PORTC_VAR;
    
    TRISA = TRISA_VAR;
    TRISB = TRISB_VAR;
    TRISC = TRISC_VAR;
    
    /* Timer 2 */
    timer_init8bit(T2, 
            CLKCON_CS_FOSC4, 
            CON_ON_ON | CON_CKPS_64 | CON_OUTPS_1, 
            HLT_MODE_PPSG, 
            0x00, 
            0x7C);
    /* Timer Period = 1mSec                                             */
    /* Timer Period = [PRx + 1] * 4 * Tosc * TxCKPS * TxOUTPS           */
    /* PRx = [Timer Period / (4 * Tosc * TxCKPS * TxOUTPS)] - 1         */
    /* PRx = [1mSec / (4 * (1 / 32MHz) * 64 * 1)] - 1                   */
    /* PRx = 124 or 0x7C                                                */
    /* Note: Tosc = 1 / Fosc                                            */

    /* UART */
    uart_init(TX_BRGH_HIGH, RC_SPEN_ENABLE, BAUD_BRG16_16BIT, 0x8A, 0x00);
    /* Desired Baud Rate = 57,600                                       */
    /* Baud Rate = Fosc / [(BRG + 1) * 4]                               */
    /* BRG = [Fosc / (4 * Baud Rate)] - 1                               */
    /* BRG = [32MHz / (4 * 57,600)] - 1                                 */
    /* BRG = 138 or 0x8A                                                */
    /* Baud Rate = Fosc / [(BRG + 1) * 4]                               */
    /* Baud Rate = 32MHz / [(138 + 1) * 4] = 57,554                     */
    /* Baud Rate = 57,554                                               */
    /* Error = (Desired Baud Rate - Baud Rate) / Desired Baud Rate      */
    /* Error = (57,600 - 57,554) / 57,600                               */
    /* Error = 0.08% Error                                              */
    
    /* I2C */
    i2c_init(i2cInst, 
            SSP1,
            STAT_SMP_HIGH_SPEED, 
            0x13);
    /* Baud Rate = 400kHz                                               */
    /* Baud Rate = Fosc / [(SSPxADD + 1) * 4]                           */
    /* SSPxADD = [Fosc / (Baud Rate * 4)] - 1                           */
    /* SSPxADD = [32Mhz / (400kHz * 4)] - 1                             */
    /* SSPxADD = 19 or 0x13                                             */
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

void setTransmitPin(uint8_t value) {
    WRITE_PIN(TX_ENABLE_PORT, TX_ENABLE_PIN, value);
}