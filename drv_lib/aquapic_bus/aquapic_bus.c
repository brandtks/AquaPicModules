/*
  AquaPic Bus

  Created by Skyler Brandt on January 2015

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/
*/

#include <stdlib.h> // for NULL
#include <stdint.h> // for int8_t and uint8_t

#include "aquapic_bus.h"

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/*****Constructor**************************************************************/
/*
apbObj apb_new (void) {
#ifdef CHIPKIT
    apbObj inst = (apbObj)calloc(1, sizeof(struct apbObjStruct));
    return inst;
#else
    static struct apbObjStruct inst;
    return &inst;
#endif
}
*/

/*****Initialize***************************************************************/
int8_t apb_init(apbObj inst, 
                void (*messageHandlerVar)(void), 
                void (*enableAddressDetectionVar)(void),
                void (*disableAddressDetectionVar)(void),
                int8_t (*checkNinthBitVar)(void),
                uint8_t addressVar)
{
    if (inst == NULL) return ERR_NOMEM;
    
    inst->messageHandler = messageHandlerVar;
    inst->enableAddressDetection = enableAddressDetectionVar;
    inst->disableAddressDetection = disableAddressDetectionVar;
    inst->checkNinthBit = checkNinthBitVar;
    inst->address = addressVar;
    
    apb_restart (inst);
    
    return ERR_NOERROR;
}

/*****Run Time*****************************************************************/
void apb_run (apbObj inst, uint8_t byte_received) {
    if (inst->checkNinthBit != NULL) {
        if (inst->checkNinthBit ()) {
            inst->apbStatus = STANDBY;
        }
    }
    
    switch (inst->apbStatus) {
        case STANDBY:
            if (byte_received == inst->address) {
                if (inst->disableAddressDetection != NULL)
                    inst->disableAddressDetection();
                inst->message[0] = inst->address;
                inst->messageCount = 1;
                inst->apbStatus = ADDRESS_RECIEVED;
            } else {
                apb_restart (inst);
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
                
                apb_restart (inst);
            }
            
            break;
        default:
            break;
    }
}

void apb_restart (apbObj inst) {
    inst->messageCount = 0;
    inst->messageLength = 0;
    inst->function = 0;
    inst->apbStatus = STANDBY;
    apb_clearMessageBuffer (inst);
    if (inst->enableAddressDetection != NULL)
        inst->enableAddressDetection();
}

void apb_clearMessageBuffer (apbObj inst) {
    int i;
    for (i = 0; i < MESSAGE_BUFFER_LENGTH; ++i)
        inst->message[i] = 0;
}

int8_t apb_checkCrc (uint8_t* message, int length) {
    uint8_t crc[2] = { 0 };
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