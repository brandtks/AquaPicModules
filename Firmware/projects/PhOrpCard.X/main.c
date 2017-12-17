/*******************************************************************************
 * pH ORP Card - main.c
 *
 * Goodtime Development 
 * 
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  Firmware for the pH ORP card
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
#include "ltc2483/ltc2483.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* General Outputs and Inputs */
/* Red LED is RB2 */
#define RED_LED(state)      state ? N_R_LED_SetLow() : N_R_LED_SetHigh()
/* Green LED is RB1 */
#define GREEN_LED(state)    state ? N_G_LED_SetLow() : N_G_LED_SetHigh()
/* Yellow LED is RB0 */
#define YELLOW_LED(state)   state ? N_Y_LED_SetLow() : N_Y_LED_SetHigh()
#define ON                  1
#define OFF                 0

/* AquaPic Bus Settings */
#define APB_ADDRESS         0x40
#define FRAMING_TIMER       1   /* Framing called from timer 1 at 1mSec */
#define ERROR_TIME          10  /* 10 sec alarm   */

/* LTC2483 and Related Settings */
#define DEFAULT_LPF_FACTOR  5
#define NUM_CHANNELS        2
#define PH_CHANNEL          0
#define PH_ADDRESS          0x27
#define ORP_CHANNEL         1
#define ORP_ADDRESS         0x15

#define ENABLE_PH(state)    state ? EN_PH_SetHigh() : EN_PH_SetLow()
#define ENABLE_ORP(state)   state ? EN_ORP_SetHigh() : EN_ORP_SetLow()

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength);
void putsch(uint8_t* data, uint8_t length);

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/
int8_t apbLastErrorState;
struct ltc2483ObjStruct phLtc2483InstStruct;
ltc2483Obj phLtc2483Inst;
struct ltc2483ObjStruct orpLtc2483InstStruct;
ltc2483Obj orpLtc2483Inst;
int16_t values[NUM_CHANNELS];
uint8_t lpfFactors[NUM_CHANNELS];
uint8_t enables[NUM_CHANNELS];
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
    
    /* pH LTC2483 initialization */
    phLtc2483Inst = &phLtc2483InstStruct;
    ltc2483_init(phLtc2483Inst, PH_ADDRESS);
    
    /* ORP LTC2483 initialization */
    orpLtc2483Inst = &orpLtc2483InstStruct;
    ltc2483_init(orpLtc2483Inst, ORP_ADDRESS);
    
    for (i = 0; i < NUM_CHANNELS; ++i) {
        values[i] = 0;
        uint64_t memory = nvm_read(i);
        /* If the memory has never been written to it returns all 1's */
        if (memory != 0xFFFFFFFFFFFFFFFF) {
            lpfFactors[i] = (memory >> 8) & 0xFF;
            enables[i] = memory & 0xFF;
        }
    }
    
    if (enables[PH_CHANNEL]) {
        ENABLE_PH(1);
    }
    
    if (enables[ORP_CHANNEL]) {
        ENABLE_ORP(1);
    }
    
    /* Enable the Global Interrupts */
    INTERRUPT_GlobalEnable();
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
        case 2: { /* set single channel low pass filter factor */
            uint8_t channel = message[3];
            uint8_t enable = message[4];
            uint8_t lpfFactor = message[5];
            
            lpfFactors[channel] = lpfFactor;
            
            enables[channel] = enable;
            if (channel == PH_CHANNEL) {
                ENABLE_PH(enable);
            } else {
                ENABLE_ORP(enable);
            }
            
            nvm_write(channel, (lpfFactor << 8) & enable);
            
            apb_sendDefualtResponse();
            break;
        }
        case 10: { /* read single channel value */
            uint8_t channel = message[3];
            
            apb_initResponse();
            apb_appendToResponse(channel);
            apb_addToResponse(&(values[channel]), sizeof(int16_t));
            apb_sendResponse();
 
            break;
        }
        case 20: /* read all channels values */
            apb_initResponse();
            apb_addToResponse(values, sizeof (int16_t) * NUM_CHANNELS);
            apb_sendResponse();
            break;
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
    
    if (enables[PH_CHANNEL]) {
        /* pH LTC2483 Polling */
        int16_t result = ltc2483_polling(phLtc2483Inst);
        if (result != -1) {
            values[PH_CHANNEL] = lpfFactors[PH_CHANNEL] * values[PH_CHANNEL] + result;
            values[PH_CHANNEL] /= lpfFactors[PH_CHANNEL] + 1;
        }
    }
    
    if (enables[ORP_CHANNEL]) {
        /* pH LTC2483 Polling */
        int16_t result = ltc2483_polling(orpLtc2483Inst);
        if (result != -1) {
            values[ORP_CHANNEL] = lpfFactors[ORP_CHANNEL] * values[ORP_CHANNEL] + result;
            values[ORP_CHANNEL] /= lpfFactors[ORP_CHANNEL] + 1;
        }
    }
}