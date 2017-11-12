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
#include <string.h>     /* memcpy */
#include "aquaPicBus.h"
#include "../common/common.h"

#ifdef TEST
#include "../uart/test/uart.h"
#else
#include "../uart/uart.h"
#endif

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/*****Initialize***************************************************************/
int8_t apb_init(apbObj inst,
        void (*messageHandlerVar)(void),
        void (*setTransmitPinVar)(uint8_t),
        uint8_t addressVar,
        uint8_t framingTimerTime,               /* In milliseconds */
        uint16_t errorTime)                     /* In seconds */
{
    if (inst == NULL) { 
        return 0;
    }
    
    if (messageHandlerVar == NULL) {
        return -1;
    }
    inst->messageHandler = messageHandlerVar;
    
    inst->address = addressVar;
    inst->framingSetpoint = FRAMING_TIME / framingTimerTime;
    
    inst->errorSetpoint = errorTime * 4 / framingTimerTime;
    
    if (setTransmitPinVar == NULL) {
        return -1;
    }
    inst->setTransmitPin = setTransmitPinVar;

    apb_restart(inst);

    return -1;
}

/*****Run Time*****************************************************************/
void apb_run(apbObj inst, uint8_t byte_received) {
    inst->framingTick = 0;
    
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
                    /* Handle the message */
                    inst->messageHandler();
                    /* Reset the error tick */
                    inst->errorTick = 0;
                }

                inst->apbStatus = WAIT_FOR_FRAMING;
            }

            break;
        default:
            break;
    }
}

void apb_framing(apbObj inst) {
    /* If not waiting for an address */
    if (inst->apbStatus != WAIT_FOR_ADDRESS) {
        /* Increase the framing count */
        inst->framingTick++;

        /* Framing count greater than frame setpoint */
        if (inst->framingTick >= inst->framingSetpoint) {
            /* Setup to receive message */
            inst->messageCount = 0;
            inst->messageLength = 0;
            inst->function = 0;
            inst->apbStatus = WAIT_FOR_ADDRESS;
        }
    }
    
    inst->timingTick = ++inst->timingTick % 250;
}

int8_t apb_errorChecking(apbObj inst) {
    /* Every 250mSecs */
    if (inst->timingTick == 0) {
        /* If not currently faulted */
        if (!inst->error) {
            /* Increment the error counter */
            ++inst->errorTick;

            /* If the counter is greater than the setpoint */
            if (inst->errorTick >= inst->errorSetpoint) {
                /* set communication fault flag */
                inst->error = -1;
                /* restart the module */
                apb_restart(inst);
            }
        /* If currently faulted */
        } else {
            /* errorTick set to 0 if a message is successfully received */
            if (inst->errorTick == 0) {
                /* clear communication fault flag */
                inst->error = 0;
            }
        }
    }
    
    return inst->error;
}

/* This clears the message buffer so make sure you have all the data stored from the command */
void apb_sendDefualtResponse(apbObj inst) {
    apb_initResponse (inst);
    apb_sendResponse (inst);
}

/* This clears the message buffer so make sure you have all the data stored from the command */
void apb_initResponse(apbObj inst) {
    if (inst->apbStatus == MESSAGE_LENGTH_RECIEVED) {
        apb_clearMessageBuffer (inst);
        inst->message[0] = inst->address;
        inst->message[1] = inst->function;
        inst->messageLength = 3; /* 3 to include the length */
    }
}

void apb_appendToResponse(apbObj inst, uint8_t data) {
    inst->message[inst->messageLength++] = data;
}

void apb_addToResponse(apbObj inst, void* data, size_t length) {
    if (inst->apbStatus == MESSAGE_LENGTH_RECIEVED) {
        memcpy(&(inst->message[inst->messageLength]), data, length);
        inst->messageLength += (uint8_t)length;
    }
}

void apb_sendResponse(apbObj inst) {
    uint8_t crc[2];
    inst->messageLength += 2; /* 2 for crc */
    inst->message[2] = inst->messageLength;
    apb_crc16(inst->message, crc, inst->messageLength);
    inst->message[inst->messageLength - 2] = crc[0];
    inst->message[inst->messageLength - 1] = crc[1];
    apb_sendMessage (inst, inst->message, inst->messageLength);
}

/* Private */
void apb_restart(apbObj inst) {
    inst->messageCount = 0;
    inst->messageLength = 0;
    inst->function = 0;
    inst->framingTick = 0;
    inst->apbStatus = WAIT_FOR_FRAMING;
    apb_clearMessageBuffer(inst);
    inst->setTransmitPin(0);
}

void apb_clearMessageBuffer(apbObj inst) {
    int i;
    for (i = 0; i < MESSAGE_BUFFER_LENGTH; ++i) {
        inst->message[i] = 0;
    }
}

int8_t apb_checkCrc(uint8_t* message, int length) {
    uint8_t crc[2] = {0,0};
    apb_crc16(message, crc, length);
    if ((message[length - 2] == crc[0]) && (message[length - 1] == crc[1]))
        return -1;  // true
    else
        return 0;   // false
}

void apb_crc16(uint8_t* message, uint8_t* crc, int length) {
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

void apb_sendMessage(apbObj inst, uint8_t* message, uint8_t length) {
    inst->setTransmitPin(1);
    putsch(message, length);
    inst->setTransmitPin(0);
}
