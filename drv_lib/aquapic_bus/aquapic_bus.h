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

//#define STANDBY                 1
//#define ADDRESS_RECIEVED        2
//#define FUNCTION_RECIEVED       3
//#define MESSAGE_LENGTH_RECIEVED 4

typedef enum abpStatusEnum {
    STANDBY,
    ADDRESS_RECIEVED,
    FUNCTION_RECIEVED,
    MESSAGE_LENGTH_RECIEVED
}apbStatus_t;

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
struct apbObjStruct {
    void (*messageHandler)(void);
    void (*enableAddressDetection)(void);
    void (*disableAddressDetection)(void);
    uint8_t address;
    uint8_t function;
    uint8_t messageLength;
    uint8_t message[MESSAGE_BUFFER_LENGTH];
    uint8_t messageCount;
    apbStatus_t apbStatus;
};

typedef struct apbObjStruct* apbObj;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/*****Constructor**************************************************************/
//apbObj apb_new(void);

/*****Initialize***************************************************************/
int8_t apb_init (apbObj inst, 
                void (*messageHandlerVar)(void), 
                void (*enableAddressDetectionVar)(void),
                void (*disableAddressDetectionVar)(void),
                uint8_t address_var);

/*****Run Time*****************************************************************/
void apb_run (apbObj inst, uint8_t byte_received, int8_t ninthBit);

void apb_restart (apbObj inst);
void apb_clearMessageBuffer (apbObj inst);
int8_t apb_checkCrc (uint8_t* message, int length);
void apb_crc16 (uint8_t* message, uint8_t* crc, int length);
uint8_t* apb_buildDefualtResponse (apbObj inst);

#endif // DRV_AQUAPIC_BUS_H