/*******************************************************************************
 * Analog Input Card - main.c
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
#include "mcp3428/mcp3428.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* General Outputs and Inputs */
/* Red LED is RB0 */
#define RED_LED(state)      state ? IO_RB0_SetHigh() : IO_RB0_SetLow()
/* Green LED is RB1 */
#define GREEN_LED(state)    state ? IO_RB1_SetHigh() : IO_RB1_SetLow()
/* Yellow LED is RB2 */
#define YELLOW_LED(state)   state ? IO_RB2_SetHigh() : IO_RB2_SetLow()
#define ON                  0
#define OFF                 1

/* AquaPic Bus Settings */
#define APB_ADDRESS         0x50
#define FRAMING_TIMER       1   /* Framing called from timer 1 at 1mSec */
#define ERROR_TIME          10  /* 10 sec alarm   */

/* MCP3428 Settings */
#define NUM_CHANNELS        4
#define MCP3428_ADDRESS     0x68
#define DEFAULT_LPF_FACTOR  5

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void apbMessageHandler(uint8_t function, uint8_t* message, uint8_t messageLength);
void putsch(uint8_t* data, uint8_t length);

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/
int8_t apbLastErrorState;
int16_t values[NUM_CHANNELS];
uint8_t lpfFactors[NUM_CHANNELS];
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
    
    for (i = 0; i < NUM_CHANNELS; ++i) {
        lpfFactors[i] = DEFAULT_LPF_FACTOR;
    }
    
    /* Enable the Global Interrupts */
    INTERRUPT_GlobalEnable();
    
    /* MCP3428 initialization */
    if (!mcp3428_init(MCP3428_ADDRESS)) {
        while (1);
    }
    
    /* Start the timer, this has to happen after the MCP3428 starts because */
    /* the timer interrupt accesses the MCP3428 memory and could cause a race */
    /* condition */
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
        case 1: { /* set all channels low pass filter factors */
            for (i = 0; i < NUM_CHANNELS; ++i) {
                lpfFactors[i] = message[i + 3];
            }
            apb_sendDefualtResponse();
            break;
        }
        case 2: /* set single channel low pass filter factor */
            lpfFactors[message [3]] = message[4];
            apb_sendDefualtResponse();
            break;
        case 10: { /* read single channel value */
            uint8_t channel = message [3];
            
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
    
    /* MCP3428 Polling */
    int16_t result = mcp3428_polling();;
    if (result != -1) {
        uint8_t channel = mcp3428_getChannel();
        values[channel] = (lpfFactors[channel] * values[channel] + result);
        values[channel] /= (lpfFactors[channel] + 1);
        channel = channel++ % NUM_CHANNELS;
        mcp3428_setChannelAndStartConversion(channel);
    }
}