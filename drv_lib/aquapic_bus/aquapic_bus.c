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

#include <stdlib.h> // for calloc()
#include <stdint.h> // for int8_t and uint8_t

#include "aquapic_bus.h"

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/*****Constructor**************************************************************/
apb_obj apb_new (void) {
#ifdef CHIPKIT
    apb_obj inst = (apb_obj)calloc(1, sizeof(struct apb_obj_struct));
    return inst;
#else
    static struct apb_obj_struct inst;
    return &inst;
#endif
}

/*****Initialize***************************************************************/
int8_t apb_init (apb_obj inst, 
                void (*message_handler_var)(void), 
                void (*enable_address_detection_var)(void),
                void (*disable_address_detection_var)(void),
                uint8_t address_var)
{
    if (inst == NULL) return ERR_NOMEM;
    
    inst->message_handler = message_handler_var;
    inst->enable_address_detection = enable_address_detection_var;
    inst->disable_address_detection = disable_address_detection_var;
    inst->address = address_var;
    inst->function = 0;
    inst->message_length = 0;
    inst->message_count = 0;
    apb_clear_message_buffer(inst);
    
#ifdef CHIPKIT
    inst->apb_status = ADDRESS_RECIEVED;
#else
    inst->apb_status = STANDBY;
#endif
    
    return ERR_NOERROR;
}

/*****Run Time*****************************************************************/
void apb_run (apb_obj inst, uint8_t byte_received) {
    switch (inst->apb_status) {
#ifndef CHIPKIT
        case STANDBY:
            if (byte_received == inst->address) {
                inst->message[0] = inst->address;
                inst->message_count = 1;
                inst->apb_status = ADDRESS_RECIEVED;
                if (inst->disable_address_detection != NULL)
                    inst->disable_address_detection();
            }
            
            break;
#endif
        case ADDRESS_RECIEVED:
#ifdef CHIPKIT
            inst->message[0] = inst->address;
#endif
            inst->function = byte_received;
            inst->message[1] = inst->function;
            inst->message_count = 2;
            inst->apb_status = FUNCTION_RECIEVED;
            break;
        case FUNCTION_RECIEVED:
            inst->message_length = byte_received;
            inst->message[2] = inst->message_length;
            inst->message_count = 3;
            inst->apb_status = MESSAGE_LENGTH_RECIEVED;
            break;
        case MESSAGE_LENGTH_RECIEVED:
            inst->message[inst->message_count++] = byte_received;
            if (inst->message_count == inst->message_length) {
                if (apb_check_crc(inst->message, inst->message_count)) {
                    if (inst->message_handler != NULL)
                        inst->message_handler();
                }
                
                inst->message_count = 0;
                apb_clear_message_buffer(inst);
                
#ifdef CHIPKIT
                inst->apb_status = ADDRESS_RECIEVED;
#else
                inst->apb_status = STANDBY;
#endif
                
                if (inst->enable_address_detection != NULL)
                    inst->enable_address_detection();
            }
            
            break;
        default:
            break;
    }
}

void apb_clear_message_buffer (apb_obj inst) {
    int i;
    for (i = 0; i < MESSAGE_BUFFER_LENGTH; ++i)
        inst->message[i] = 0;
}

int8_t apb_check_crc (uint8_t* message, int length) {
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

uint8_t* apb_build_defualt_response (apb_obj inst) {
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