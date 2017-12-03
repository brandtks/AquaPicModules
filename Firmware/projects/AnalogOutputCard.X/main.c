/*******************************************************************************
 * Analog Output Card - main.c
 *
 * Goodtime Development 
 * 
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  Firmware for the analog output card
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
 ******************************************************************************/

#include <string.h>     /* memcpy */
#include "mcc_generated_files/mcc.h"
#include "../../lib/aquaPicBus/aquaPicBus.h"
#include "../../lib/PIC32MM/nvm/nvm.h"

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

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength);
void putsch(uint8_t* data, uint8_t length);
float getPwmValue(uint8_t channel);
void setPwmValue(uint8_t channel, float value);

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/
int8_t apbLastErrorState;

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
    
    /* Read saved output types and set relays accordingly, read returns all */
    /* 1's if the memory has never been written to */
    (uint8_t)nvm_read(0) == 0x1234 ? RELAY0_SetHigh() : RELAY0_SetLow();
    (uint8_t)nvm_read(1) == 0x1234 ? RELAY1_SetHigh() : RELAY1_SetLow();
    (uint8_t)nvm_read(2) == 0x1234 ? RELAY2_SetHigh() : RELAY2_SetLow();
    (uint8_t)nvm_read(3) == 0x1234 ? RELAY3_SetHigh() : RELAY3_SetLow();
    
    /* Start the PWM modules */
    SCCP4_COMPARE_Start();
    SCCP5_COMPARE_Start();
    SCCP6_COMPARE_Start();
    SCCP7_COMPARE_Start();
    
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
        case 2: { /* setup single channel */
            uint8_t channel = message[3];
            uint8_t type = message[4];
            /* write a specific value to memory so the initialization has */
            /* a specific value to compare against */
            nvm_write(channel, type ? 0x1234 : 0);
            switch(channel) {
                case 0:
                    type ? RELAY0_SetHigh() : RELAY0_SetLow();
                    break;
                case 1:
                    type ? RELAY1_SetHigh() : RELAY1_SetLow();
                    break;
                case 2:
                    type ? RELAY2_SetHigh() : RELAY2_SetLow();
                    break;
                case 3:
                    type ? RELAY3_SetHigh() : RELAY3_SetLow();
                    break;
                default:
                    break;
            }
            apb_sendDefualtResponse();
            break;
        }
        case 10: { /* read single channel value */
            float value = getPwmValue(message[3]);
            apb_initResponse();
            apb_addToResponse(&value, sizeof(float));
            apb_sendResponse();

            break;
        }
        case 20: { /* read all channels values */
            float values[4];
            int i;
            for (i = 0; i < 4; ++i) {
                values[i] = getPwmValue(i);
            }
            apb_initResponse();
            apb_addToResponse(values, sizeof(float) * 4);
            apb_sendResponse();
            break;
        }
        case 30: { /* write all channels values */
            float values[4];
            memcpy(values, &(message[3]), sizeof(float) * 4);
            int i;
            for (i = 0; i < 4; ++i) {
                setPwmValue(i, values[i]);
            }
            apb_sendDefualtResponse();
            break;
        }
        case 31: { /* write single channel value */
            float value;
            memcpy(&value, &(message[4]), sizeof(float));
            /* value = (value >> 8) | (value << 8); /* reverse the endianess */
            setPwmValue(message[3], value);
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

float getPwmValue(uint8_t channel) {
    float value;
    switch(channel) {
        case 0:
            value = (float)CCP4RB / (float)CCP4PR;
            break;
        case 1:
            value = (float)CCP5RB / (float)CCP5PR;
            break;
        case 2:
            value = (float)CCP6RB / (float)CCP6PR;
            break;
        case 3:
            value = (float)CCP7RB / (float)CCP7PR;
            break;
        default:
            break;
    }
    return value;
}

void setPwmValue(uint8_t channel, float value) {
    if (value > 100.0) {
        value = 100.0;
    }
    
    uint16_t setpoint;
    switch(channel) {
        case 0:
            setpoint = (uint16_t)((float)CCP4PR * (value / 100.0));
            CCP4RB = setpoint;
            break;
        case 1:
            setpoint = (uint16_t)((float)CCP5PR * (value / 100.0));
            CCP5RB = setpoint;
            break;
        case 2:
            setpoint = (uint16_t)((float)CCP6PR * (value / 100.0));
            CCP6RB = setpoint;
            break;
        case 3:
            setpoint = (uint16_t)((float)CCP7PR * (value / 100.0));
            CCP7RB = setpoint;
            break;
        default:
            break;
    }
}

/* Defined in tmr1.h */
void TMR1_CallBack() {
    /* AquaPic Bus framing */
    apb_framing();
}