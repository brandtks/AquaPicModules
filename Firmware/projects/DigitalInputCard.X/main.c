/*******************************************************************************
 * pH ORP Card - main.c
 *
 * Goodtime Development 
 * 
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  Firmware for the analog input card
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
#define APB_ADDRESS         0x40
#define FRAMING_TIMER       1   /* Framing called from timer 1 at 1mSec */
#define ERROR_TIME          10  /* 10 sec alarm   */

/* Digital Inputs */
#define NUM_INPUTS          6
#define DECOUNCE_LIMIT      25 
#define DEBOUNCE_INIT       128
#define ALL_LEDS_OFF        255

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength);
void putsch(uint8_t* data, uint8_t length);
uint32_t readInputValue(uint8_t input);
void turnOnInputLed(uint8_t led);

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
typedef enum inputStateEnum {
    OPEN = 0,
    CLOSE = 1
}inputState_t;

struct inputObjStruct {
    inputState_t state;
    uint8_t debounceTime;
};

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/
struct inputObjStruct inputs[NUM_INPUTS];
uint8_t ledIndex;
int8_t apbLastErrorState;
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
    
    /* Input initialization */
    for (i = 0; i < NUM_INPUTS; ++i) {
        inputs[i].state = OPEN;
        inputs[i].debounceTime = DEBOUNCE_INIT;
    }
    ledIndex = 0;
    
    /* Enable the Global Interrupts */
    INTERRUPT_GlobalEnable();
    /* Start the timer */
    TMR1_Start();
    
    YELLOW_LED(OFF);
    GREEN_LED(ON);
    
    while (1) {
        while (!UART1_ReceiveBufferIsEmpty()) {
            apb_run(UART1_Read());
        }
        
        int8_t apbError = apb_isErrored();
        if (apbError && !apbLastErrorState) {
            GREEN_LED(OFF);
            RED_LED(ON);
        } else if (!apbError && apbLastErrorState) {
            RED_LED(OFF);
            GREEN_LED(ON);
        }
        apbLastErrorState = apbError;
    }
}

void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength) {
    switch (function) {
        case 10: { /* read single channel value */
            uint8_t channel = message[3];
            uint8_t value = 0;
            if (channel >= 0)
            if (channel < NUM_INPUTS) {
                value = (uint8_t)inputs[channel].state;
            }
            
            apb_initResponse();
            apb_appendToResponse(value);
            apb_sendResponse();
            break;
        }
        case 20: { /* read all channels values */
            uint8_t values = 0x00;            
            int i;
            for (i = 0; i < NUM_INPUTS; ++i) {
                values |= (uint8_t)inputs[i].state << i;
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

void putsch(uint8_t* data, uint8_t length) {
    UART1_WriteBuffer(data, length);
}

/* Defined in tmr1.h */
void TMR1_CallBack() {
    /* AquaPic Bus framing */
    apb_framing();
    
    /* Debounce inputs */
    for (i = 0; i < NUM_INPUTS; ++i) {
        /* Inputs are active low */
        if (!readInputValue(i)) {
            inputs[i].debounceTime++;
            if (inputs[i].debounceTime > (DEBOUNCE_INIT + DECOUNCE_LIMIT)) {
                inputs[i].debounceTime = DEBOUNCE_INIT + DECOUNCE_LIMIT;
                inputs[i].state = CLOSE;
            }
        } else {
            inputs[i].debounceTime--;
            if (inputs[i].debounceTime < (DEBOUNCE_INIT - DECOUNCE_LIMIT)) {
                inputs[i].debounceTime = DEBOUNCE_INIT - DECOUNCE_LIMIT;
                inputs[i].state = OPEN;
            }
        }
    }
    
    /* Multiplex LEDs */
    inputs[i].state ? turnOnInputLed(ledIndex) : turnOnInputLed(ALL_LEDS_OFF);
    ledIndex = ++ledIndex % NUM_INPUTS;
}

uint32_t readInputValue(uint8_t input) {
    uint32_t value;
    switch(input) {
        case 0:
            value = INPUT_1_GetValue();
            break;
        case 1:
            value = INPUT_2_GetValue();
            break;
        case 2:
            value = INPUT_3_GetValue();
            break;
        case 3:
            value = INPUT_4_GetValue();
            break;
        case 4:
            value = INPUT_5_GetValue();
            break;
        case 5:
            value = INPUT_6_GetValue();
            break;
        default:
            /* Inputs are active low */
            value = 1;
            break;
    }
    return value;
}

void turnOnInputLed(uint8_t led) {
    switch(led) {
        case 0:
            /* Disable line 3 */
            LED_3_SetDigitalInput();
            /* Enable lines 1 and 2 */
            LED_1_SetDigitalOutput();
            LED_2_SetDigitalOutput();
            /* Line 1 high, line 2 low */
            LED_1_SetHigh();
            LED_2_SetLow();
            break;
        case 1:
            /* Disable line 3 */
            LED_3_SetDigitalInput();
            /* Enable lines 1 and 2 */
            LED_1_SetDigitalOutput();
            LED_2_SetDigitalOutput();
            /* Line 2 high, line 1 low */
            LED_2_SetHigh();
            LED_1_SetLow();
            break;
        case 2:
            /* Disable line 1 */
            LED_1_SetDigitalInput();
            /* Enable lines 2 and 3 */
            LED_2_SetDigitalOutput();
            LED_3_SetDigitalOutput();
            /* Line 2 high, line 3 low */
            LED_2_SetHigh();
            LED_3_SetLow();
            break;
        case 3:
            /* Disable line 1 */
            LED_1_SetDigitalInput();
            /* Enable lines 2 and 3 */
            LED_2_SetDigitalOutput();
            LED_3_SetDigitalOutput();
            /* Line 3 high, line 2 low */
            LED_3_SetHigh();
            LED_2_SetLow();
            break;
        case 4:
            /* Disable line 2 */
            LED_2_SetDigitalInput();
            /* Enable lines 1 and 3 */
            LED_1_SetDigitalOutput();
            LED_3_SetDigitalOutput();
            /* Line 1 high, line 3 low */
            LED_1_SetHigh();
            LED_3_SetLow();
            break;
        case 5:
            /* Disable line 2 */
            LED_2_SetDigitalInput();
            /* Enable lines 1 and 3 */
            LED_1_SetDigitalOutput();
            LED_3_SetDigitalOutput();
            /* Line 3 high, line 1 low */
            LED_3_SetHigh();
            LED_1_SetLow();
            break;
        /* turn everything off */
        default:
            LED_1_SetDigitalInput();
            LED_2_SetDigitalInput();
            LED_3_SetDigitalInput();
            break;
    }
}