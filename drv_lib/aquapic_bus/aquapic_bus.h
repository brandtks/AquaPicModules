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

#ifndef DRV_AQUAPIC_BUS_H
#define DRV_AQUAPIC_BUS_H

#include <stdint.h>

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
/*******Errors List************************************************************/
#define ERR_NOERROR 0 // no error has occured
#define ERR_UNKNOWN -1 // unknown error
#define ERR_NOMEM   -2 // out of memory

/*******User Defined***********************************************************/
#define MESSAGE_BUFFER_LENGTH   32

#define STANDBY                 1
#define ADDRESS_RECIEVED        2
#define FUNCTION_RECIEVED       3
#define MESSAGE_LENGTH_RECIEVED 4

//#define CHIPKIT

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
struct apb_obj_struct {
    void (*message_handler)(void);
    void (*enable_address_detection)(void);
    void (*disable_address_detection)(void);
    uint8_t address;
    uint8_t function;
    uint8_t message_length;
    uint8_t message[MESSAGE_BUFFER_LENGTH];
    uint8_t message_count;
    uint8_t apb_status;
};

typedef struct apb_obj_struct* apb_obj;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/*****Constructor**************************************************************/
apb_obj apb_new(void);

/*****Initialize***************************************************************/
int8_t apb_init(apb_obj inst, 
                void (*message_handler_var)(void), 
                void (*enable_address_detection_var)(void),
                void (*disable_address_detection_var)(void),
                uint8_t address_var); 

/*****Run Time*****************************************************************/
void apb_run(apb_obj inst, uint8_t byte_received);

void apb_clear_message_buffer(apb_obj inst);
int8_t apb_check_crc(uint8_t* message, int length);
void apb_crc16(uint8_t* message, uint8_t* crc, int length);
uint8_t* apb_build_defualt_response(apb_obj inst);

#endif // DRV_AQUAPIC_BUS_H