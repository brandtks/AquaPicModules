/*
 * Analog Input Card - main.c
 *
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  Firmware for the analog input card
 * 
 * Device:
 *  PIC32MM0064GPM028
 */

/*
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

#include "mcc_generated_files/mcc.h"
#include "../../lib/aquaPicBus/aquaPicBus.h"
#include "../../lib/led/led.h"

/*
 * Defines
 */

/* General Outputs and Inputs */
#define RED_LED_PORT    &LATB
#define GREEN_LED_PORT  &LATB
#define YELLOW_LED_PORT &LATB

#define RED_LED_PIN     0
#define GREEN_LED_PIN   1
#define YELLOW_LED_PIN  2

/* AquaPic Bus Settings */
#define APB_ADDRESS     0x50
#define FRAMING_TIMER   1   /* Framing called from timer 2 at 1mSec */
#define ERROR_TIME      10  /* 10 sec alarm   */

/*
 * Functions
 */
void initializeHardware(void);
void apbMessageHandler(void);
void setTransmitPin(uint8_t value);

/*
 * Variables
 */
struct apbObjStruct apbInstStruct;
apbObj apbInst = &apbInstStruct;
uint16_t commCounter;
uint8_t  apbError;

void main(void) {
    /* initialize the device */
    SYSTEM_Initialize();

    /* AquaPic Bus initialization */
    if (!apb_init(apbInst, 
            &apbMessageHandler, 
            &putsch,
            APB_ADDRESS,
            FRAMING_TIMER,
            ERROR_TIME)) {
        while (1);
    }
    
    /* Enable the Global Interrupts */
    INTERRUPT_GlobalEnable();

    SET_LED(YELLOW_LED_PORT, YELLOW_LED_PIN, OFF);
    SET_LED(GREEN_LED_PORT, GREEN_LED_PIN, ON);
    
    while (1) {
        while (!UART1_ReceiveBufferIsEmpty()) {
            apb_run(apbInst, UART1_Read());
        }
    }
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

void putsch(uint8_t* data, uint8_t length) {
    UART1_WriteBuffer(data, length);
}