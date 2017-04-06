/*
 * AquaPic Bus
 * 
 * Copyright (c) 2017 Skyler Brandt
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

#include <stdlib.h>     /* null, size_t */
#include <stdint.h>     /* uint8_t, int8_t */
#include "aquaPicBus.h"
#include "../uart/uart.h"
#include "../pins/pins.h"
#include "../common/common.h"

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/*****Initialize***************************************************************/
int8_t apb_init(apbObj inst,
        void (*messageHandlerVar)(void),
        uint8_t addressVar,
        uint8_t framingTimerTime,
        volatile uint8_t* transmitEnablePort,
        uint8_t transmitEnablePin)
{
    if (inst == NULL) return ERR_NOMEM;
    
    inst->messageHandler = messageHandlerVar;
    inst->address = addressVar;
    inst->framingSetpoint = FRAMING_TIME / framingTimerTime;
    inst->transmitEnablePort = transmitEnablePort;
    inst->transmitEnablePin = transmitEnablePin;
    
    apb_restart (inst);
    
    return ERR_NOERROR;
}

/*****Run Time*****************************************************************/
void apb_run (apbObj inst, uint8_t byte_received) {
    inst->framingCount = 0;
    
    switch (inst->apbStatus) {
        case WAIT_FOR_ADDRESS:
            if (byte_received == inst->address) {
                inst->message[0] = inst->address;
                inst->messageCount = 1;
                inst->apbStatus = ADDRESS_RECIEVED;
            } else {
                inst->apbStatus = WAIT_FOR_FRAMING;
            }
            
            break;
        case ADDRESS_RECIEVED:
            inst->function = byte_received;
            inst->message[1] = inst->function;
            inst->messageCount = 2;
            inst->apbStatus = FUNCTION_RECIEVED;
            break;
        case FUNCTION_RECIEVED:
            inst->messageLength = byte_received;
            inst->message[2] = inst->messageLength;
            inst->messageCount = 3;
            inst->apbStatus = MESSAGE_LENGTH_RECIEVED;
            break;
        case MESSAGE_LENGTH_RECIEVED:
            inst->message[inst->messageCount++] = byte_received;
            if (inst->messageCount == inst->messageLength) {
                if (apb_checkCrc(inst->message, inst->messageCount)) {
                    if (inst->messageHandler != NULL)
                        inst->messageHandler();
                }
                
                inst->apbStatus = WAIT_FOR_FRAMING;
            }
            
            break;
        default:
            break;
    }
}

void apb_framing (apbObj inst) {
    if (inst->apbStatus != WAIT_FOR_ADDRESS) {
        inst->framingCount++;
        
        if (inst->framingCount >= inst->framingSetpoint) {
            apb_setupMessage (inst);
        }
    }
}

/* This clears the message buffer so make sure you have all the data stored from the command */
void apb_sendDefualtResponse (apbObj inst) {
    apb_initResponse (inst);
    apb_sendResponse (inst);
}

/* This clears the message buffer so make sure you have all the data stored from the command */
void apb_initResponse (apbObj inst) {
    if (inst->apbStatus == MESSAGE_LENGTH_RECIEVED) {
        apb_clearMessageBuffer (inst);
        inst->message[0] = inst->address;
        inst->message[1] = inst->function;
        inst->messageLength = 3; /* 3 to include the length */
    }
}

void apb_appendToResponse (apbObj inst, uint8_t data) {
    inst->message[inst->messageLength] = data;
    inst->messageLength += 1;
}

void apb_addToResponse (apbObj inst, void* data, size_t length) {
    if (inst->apbStatus == MESSAGE_LENGTH_RECIEVED) {
        memoryCopy (&(inst->message[inst->messageLength]), data, length);
        inst->messageLength += (uint8_t)length;
    }
}

void apb_sendResponse (apbObj inst) {
    uint8_t crc[2];
    inst->messageCount += 2; /* 2 for crc */
    apb_crc16(inst->message, crc, inst->messageLength);
    inst->message[inst->messageLength - 2] = crc[0];
    inst->message[inst->messageLength - 1] = crc[1];
    apb_sendMessage (inst, inst->message, inst->messageCount);
}

/* Depreciated */
/* 
uint8_t* apb_buildDefualtResponse (apbObj inst) {
    static uint8_t response[5] = { 0 };
    uint8_t crc[2] =  { 0 };

    response[0] = inst->address;
    response[1] = inst->function;
    response[2] = 5;
    apb_crc16(response, crc, 5);
    response[3] = crc[0];
    response[4] = crc[1];

    return response;
}
*/

/* Private */
void apb_restart (apbObj inst) {
    inst->messageCount = 0;
    inst->messageLength = 0;
    inst->function = 0;
    inst->framingCount = 0;
    inst->apbStatus = WAIT_FOR_FRAMING;
    apb_clearMessageBuffer (inst);
    writePin (inst->transmitEnablePort, inst->transmitEnablePin, LOW);
}

void apb_setupMessage (apbObj inst) {
    inst->messageCount = 0;
    inst->messageLength = 0;
    inst->function = 0;
    inst->apbStatus = WAIT_FOR_ADDRESS;
    apb_clearMessageBuffer (inst);
}

void apb_clearMessageBuffer (apbObj inst) {
    int i;
    for (i = 0; i < MESSAGE_BUFFER_LENGTH; ++i) {
        inst->message[i] = 0;
    }
}

int8_t apb_checkCrc (uint8_t* message, int length) {
    uint8_t crc[2] = {0,0};
    apb_crc16(message, crc, length);
    if ((message[length - 2] == crc[0]) && (message[length - 1] == crc[1]))
        return -1;  // true
    else
        return 0;   // false
}

void apb_crc16 (uint8_t* message, uint8_t* crc, int length) {
    uint16_t crc_full = 0xFFFF;
    uint16_t crc_lsb;

    int i;
    for (i = 0; i < length - 2; ++i) {
        crc_full ^= message[i];

        int j;
        for (j = 0; j < 8; ++j) {
            crc_lsb = crc_full & 0x0001;
            crc_full = (crc_full >> 1) & 0x7FFF;

            if (crc_lsb == 1)
                crc_full = crc_full ^ 0xA001;
        }
    }

    crc[1] = (uint8_t)((crc_full >> 8) & 0xFF);
    crc[0] = (uint8_t)(crc_full & 0xFF);
}

void apb_sendMessage (apbObj inst, uint8_t* message, uint8_t length) {
    writePin (inst->transmitEnablePort, inst->transmitEnablePin, HIGH);
    putsch (message, length);
    writePin (inst->transmitEnablePort, inst->transmitEnablePin, LOW);
}