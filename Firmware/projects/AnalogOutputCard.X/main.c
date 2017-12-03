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
#define APB_ADDRESS         0x20
#define FRAMING_TIMER       1   /* Framing called from timer 1 at 1mSec */
#define ERROR_TIME          10  /* 10 sec alarm   */

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength);
void putsch(uint8_t* data, uint8_t length);

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/
int8_t apbLastErrorState;

void main(void) {
    /* initialize the device */
    SYSTEM_Initialize();
    /* SYSTEM_Initialize enables the global interrupts but we want to setup a */
    /* few things before interrupts can occur */
    INTERRUPT_GlobalDisable();
    
    /* AquaPic Bus initialization */
    if (!apb_init(&apbMessageHandler, 
            &putsch,
            APB_ADDRESS,
            FRAMING_TIMER,
            ERROR_TIME)) {
        while (1);
    }
    apbLastErrorState = 0;
    
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
        } else if (apbError && !apbLastErrorState) {
            RED_LED(OFF);
            GREEN_LED(ON);
        }
        apbLastErrorState = apbError;
    }
}

void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength) {
    switch (function) {
#if SELECTABLE_OUTPUT
        case 2: { /* setup single channel */
            uint8_t channel = message[3] + 1; //0 is channel 1
            uint8_t type    = message[4];
            //type 255 is PWM output so close relay to bypass filter
            WRITE_PIN(&PORTB, channel, type == 255);
            eeprom_write(channel - 1, type);
            apb_sendDefualtResponse();
            break;
        }
#endif
        case 10: { /* read single channel value */
            uint8_t channel = message[3];
            uint16_t value  = getPwmValue(pwmInst[channel]);
            apb_initResponse();
            apb_addToResponse(&value, sizeof(uint16_t));
            apb_sendResponse();

            break;
        }
        case 20: { /* read all channels values */
            uint16_t values[4];
            int i;
            for (i = 0; i < 4; ++i) {
                values[i] = getPwmValue(pwmInst[i]);
            }
            apb_initResponse();
            apb_addToResponse(values, sizeof(uint16_t) * 4);
            apb_sendResponse();
            break;
        }
        case 30: { /* write all channels values */
            uint16_t values[4];
            memcpy(values, &(message[3]), sizeof(uint16_t) * 4);
            int i;
            for (i = 0; i < 4; ++i) {
                setPwmValue(pwmInst[i], values [i]);
            }
            apb_sendDefualtResponse();
            break;
        }
        case 31: { /* write single channel value */
            uint8_t channel = message[3];
            uint16_t value;
            memcpy(&value, &(message[4]), sizeof(uint16_t));
            /* value = (value >> 8) | (value << 8); /* reverse the endianess */
            setPwmValue(pwmInst[channel], value);
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

/* Defined in tmr1.h */
void TMR1_CallBack() {
    /* AquaPic Bus framing */
    apb_framing();
}