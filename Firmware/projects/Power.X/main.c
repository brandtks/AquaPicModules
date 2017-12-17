/*******************************************************************************
 * Power - main.c
 *
 * Goodtime Development 
 * 
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  Firmware for the power strip
 * 
 * Device:
 *  PIC32MM0064GPM028
 ******************************************************************************/

/******************************************************************************/
/* License                                                                    */
/******************************************************************************/
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

#include "mcc_generated_files/mcc.h"
#include "../../lib/aquaPicBus/aquaPicBus.h"
#include "../../lib/PIC32MM/nvm/nvm.h"
#include "../../lib/common/common.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* General Outputs and Inputs */
/* Red LED is RB0 */
#define RED_LED(state)      state ? N_R_LED_SetLow() : N_R_LED_SetHigh()
/* Green LED is RB1 */
#define GREEN_LED(state)    state ? N_G_LED_SetLow() : N_G_LED_SetHigh()
/* Yellow LED is RB2 */
#define YELLOW_LED(state)   state ? N_Y_LED_SetLow() : N_Y_LED_SetHigh()
#define ON                  1
#define OFF                 0

/* AquaPic Bus Settings */
#define APB_ADDRESS         0x20
#define FRAMING_TIMER       1   /* Framing called from timer 1 at 1mSec */
#define ERROR_TIME          10  /* 10 sec alarm   */

/* Application */
#define NUM_OUTLETS         8
//#define ENABLE_CURRENT      
#define LPF_FACTOR          5

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength);
void putsch(uint8_t* data, uint8_t length);
void setOutletState(uint8_t input, uint8_t state);
uint8_t getOutletStates();

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/
int8_t apbLastErrorState;
uint8_t fallbackFlags;
int16_t currents[NUM_OUTLETS];
uint8_t i;

void main(void) {
    /* initialize the device */
    SYSTEM_Initialize();
    
    /* AquaPic Bus initialization */
    if (!apb_init(&apbMessageHandler, 
            &putsch,
            APB_ADDRESS,
            FRAMING_TIMER,
            ERROR_TIME)) {
        while (1);
    }
    apbLastErrorState = 0;
    
    /* Read fallback flags from memory */
    fallbackFlags = (uint8_t)nvm_read(0);

#ifdef ENABLE_CURRENT
    for (i = 0; i < NUM_OUTLETS; ++i) {
        currents[i] = 0;
    }
    
    /* Start the ADC */
    AD1CON1SET = 1 << _AD1CON1_ON_POSITION;
    AD1CON1SET = 1 << _AD1CON1_SAMP_POSITION;
    /* Clear the interrupt flag */
    IFS1CLR = 1 << _IFS1_AD1IF_POSITION;
#endif
    
    /* Enable the Global Interrupts */
    INTERRUPT_GlobalEnable();
    
    /* Start timer 1 */
    TMR1_Start();
    
    YELLOW_LED(OFF);
    GREEN_LED(ON);
    
    while (1) {
        while (!UART1_ReceiveBufferIsEmpty()) {
            apb_run(UART1_Read());
        }
        
#ifdef ENABLE_CURRENT
        /* The conversion scan is complete */
        if (IFS1bits.AD1IF) {
            uint16_t results[NUM_OUTLETS];
            /* Clear the interrupt flag */
            IFS1CLR = 1 << _IFS1_AD1IF_POSITION;
            /* Get the results from ADC buffers */
            ADC1_ConversionResultBufferGet(results);
            /* Filter results */
            for (i = 0; i < NUM_OUTLETS; ++i) {
                currents[i] = LPF_FACTOR * currents[i] + results[i];
                currents[i] /= LPF_FACTOR + 1;
            }
        }
#endif 
        
        int8_t apbError = apb_isErrored();
        /* Error rising edge */
        if (apbError && !apbLastErrorState) {
            /* Set outlets to fallback state */
            for (i = 0; i < NUM_OUTLETS; ++i) {
                setOutletState(i, testFlagBit(fallbackFlags, i));
            }
            /* Show red led */
            GREEN_LED(OFF);
            RED_LED(ON);
        /* Error falling edge */
        } else if (!apbError && apbLastErrorState) {
            /*  */
            RED_LED(OFF);
            GREEN_LED(ON);
        }
        /* Store error value */
        apbLastErrorState = apbError;
    }
}

void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength) {
    switch (function) {
        case 2: { /* setup single channel */
            uint8_t oldFallbackFlags = fallbackFlags;
            assignFlagStateBit(fallbackFlags, message[3], message[4]);
            /* only write to flash if different */
            if (oldFallbackFlags != fallbackFlags) {
                nvm_write(0, fallbackFlags);
            }
            apb_sendDefualtResponse();
            break;
        }
#ifdef ENABLE_CURRENT
        case 10: { /* read outlet current */
            uint8_t outlet = message[3];
            uint16_t current = currents[outlet];
            apb_initResponse();
            apb_appendToResponse(outlet);
            apb_addToResponse(&current, sizeof(uint16_t));
            apb_sendResponse();
            break;
        }
#endif
        case 20: { /* read status */
            apb_initResponse();
            AC_AVAIL_GetValue() ? apb_appendToResponse(0xFF) : apb_appendToResponse(0x00);
            apb_appendToResponse(getOutletStates());
#ifdef ENABLE_CURRENT
            apb_appendToResponse(0xFF);
#else
            apb_appendToResponse(0x00);
#endif
            apb_sendResponse();
            break;
        }
#ifdef ENABLE_CURRENT
        case 21: { /* read all current */
            int i;
            apb_initResponse();
            apb_addToResponse(&(currents[i]), sizeof(uint16_t) * NUM_OUTLETS);
            apb_sendResponse();
            break;
        }
#endif
        case 30: { /* write outlet */
            setOutletState(message[3], message[4]);
            apb_sendDefualtResponse();
            break;
        }
        default:
            break;
    }
}

void putsch(uint8_t* data, uint8_t length) {
    UART1_WriteBuffer(data, length);
}

void setOutletState(uint8_t input, uint8_t state) {
    switch(input) {
        case 0:
            state ? RELAY_1_SetHigh() : RELAY_1_SetLow();
            break;
        case 1:
            state ? RELAY_2_SetHigh() : RELAY_2_SetLow();
            break;
        case 2:
            state ? RELAY_3_SetHigh() : RELAY_3_SetLow();
            break;
        case 3:
            state ? RELAY_4_SetHigh() : RELAY_4_SetLow();
            break;
        case 4:
            state ? RELAY_5_SetHigh() : RELAY_5_SetLow();
            break;
        case 5:
            state ? RELAY_6_SetHigh() : RELAY_6_SetLow();
            break;
        case 6:
            state ? RELAY_7_SetHigh() : RELAY_7_SetLow();
            break;
        case 7:
            state ? RELAY_8_SetHigh() : RELAY_8_SetLow();
            break;
        default:
            break;
    }
}

uint8_t getOutletStates() {
    uint8_t states = 0;
    assignFlagStateBit(states, 0, RELAY_1_GetValue());
    assignFlagStateBit(states, 1, RELAY_2_GetValue());
    assignFlagStateBit(states, 2, RELAY_3_GetValue());
    assignFlagStateBit(states, 3, RELAY_4_GetValue());
    assignFlagStateBit(states, 4, RELAY_5_GetValue());
    assignFlagStateBit(states, 5, RELAY_6_GetValue());
    assignFlagStateBit(states, 6, RELAY_7_GetValue());
    assignFlagStateBit(states, 7, RELAY_8_GetValue());
    return states;
}

/* Defined in tmr1.h */
void TMR1_CallBack() {
    /* AquaPic Bus framing */
    apb_framing();
}