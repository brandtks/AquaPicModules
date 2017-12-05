/*******************************************************************************
 * aquaPicBus.c
 *
 * Goodtime Development 
 * 
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  AquaPic Bus Slave driver
 * 
 * Device:
 *  Any
 ******************************************************************************/

/******************************************************************************/
/* License                                                                    */
/******************************************************************************/
/*******************************************************************************
 * Copyright (c) 2017 Goodtime Development
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 ******************************************************************************/

#include <stdlib.h>     /* null, size_t */
#include <stdint.h>     /* uint8_t, int8_t */
#include <string.h>     /* memcpy */
#include "aquaPicBus.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define MESSAGE_BUFFER_LENGTH   32
#define FRAMING_TIME            10  /* Framing time between messages in milliseconds */

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
typedef enum abpStatusEnum {
    WAIT_FOR_FRAMING,
    WAIT_FOR_ADDRESS,
    WAIT_FOR_FUNCTION,
    WAIT_FOR_LENGTH,
    RECEIVE_MESSAGE
}apbStatus_t;

struct apbObjStruct {
    void (*messageHandler)(uint8_t, uint8_t*, uint8_t);
    void (*putsch)(uint8_t*, uint8_t);
    uint8_t address;
    uint8_t function;
    uint8_t messageLength;
    uint8_t message[MESSAGE_BUFFER_LENGTH];
    uint8_t messageCount;
    apbStatus_t apbStatus;
    uint8_t framingTick;
    uint8_t framingSetpoint;
    int8_t error;
    uint32_t errorTick;
    uint32_t errorSetpoint;
};

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/
static struct apbObjStruct apbInst;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void apb_restart();
void apb_clearMessageBuffer();
int8_t apb_checkCrc(uint8_t* message, int length);
void apb_crc16(uint8_t* message, uint8_t* crc, int length);

/*****Initialize***************************************************************/
int8_t apb_init(void (*messageHandlerVar)(uint8_t, uint8_t*, uint8_t),
        void (*putschVar)(uint8_t*, uint8_t),
        uint8_t addressVar,
        uint8_t framingTimerTime,               /* In milliseconds */
        uint16_t errorTime)                     /* In seconds */
{
    if (messageHandlerVar == NULL) {
        return 0;
    }
    apbInst.messageHandler = messageHandlerVar;
    
    apbInst.address = addressVar;
    apbInst.framingSetpoint = FRAMING_TIME / framingTimerTime;
    apbInst.error = 0;
    apbInst.errorTick = 0;
    apbInst.errorSetpoint = errorTime * 1000 / framingTimerTime;
    
    if (putschVar == NULL) {
        return 0;
    }
    apbInst.putsch = putschVar;

    apb_restart();
    return 1;
}

/*****Run Time*****************************************************************/
void apb_run(const uint8_t byteRecieved) {
    /* Reset the framing tick */
    apbInst.framingTick = 0;
    
    switch (apbInst.apbStatus) {
        case WAIT_FOR_ADDRESS:
            if (byteRecieved == apbInst.address) {
                apbInst.message[0] = apbInst.address;
                apbInst.messageCount = 1;
                apbInst.apbStatus = WAIT_FOR_FUNCTION;
            } else {
                apbInst.apbStatus = WAIT_FOR_FRAMING;
            }
            
            break;
        case WAIT_FOR_FUNCTION:
            apbInst.function = byteRecieved;
            apbInst.message[1] = apbInst.function;
            apbInst.messageCount = 2;
            apbInst.apbStatus = WAIT_FOR_LENGTH;
            break;
        case WAIT_FOR_LENGTH:
            apbInst.messageLength = byteRecieved;
            apbInst.message[2] = apbInst.messageLength;
            apbInst.messageCount = 3;
            apbInst.apbStatus = RECEIVE_MESSAGE;
            break;
        case RECEIVE_MESSAGE:
            apbInst.message[apbInst.messageCount++] = byteRecieved;
            if (apbInst.messageCount == apbInst.messageLength) {
                if (apb_checkCrc(apbInst.message, apbInst.messageCount)) {
                    /* Handle the message */
                    apbInst.messageHandler(apbInst.function,
                            apbInst.message,
                            apbInst.messageLength);
                    /* Reset the error tick */
                    apbInst.errorTick = 0;
                }

                apbInst.apbStatus = WAIT_FOR_FRAMING;
            }

            break;
        default:
            break;
    }
}

void apb_framing() {
    /* If not waiting for an address */
    if (apbInst.apbStatus != WAIT_FOR_ADDRESS) {
        /* Increase the framing count */
        /* Framing tick is reset whenever a byte is received */
        apbInst.framingTick++;

        /* Framing count greater than frame setpoint */
        if (apbInst.framingTick >= apbInst.framingSetpoint) {
            /* Setup to receive message */
            apbInst.messageCount = 0;
            apbInst.messageLength = 0;
            apbInst.function = 0;
            apbInst.apbStatus = WAIT_FOR_ADDRESS;
        }
    }
    
    /* If not currently faulted */
    if (!apbInst.error) {
        /* Increment the error tick */
        apbInst.errorTick++;
        
        /* Error tick is greater than the error setpoint */
        if (apbInst.errorTick >= apbInst.errorSetpoint) {
            /* set communication fault flag */
            apbInst.error = 1;
            /* restart the module */
            apb_restart();
        }
    /* Currently faulted */
    } else {
        /* errorTick set to 0 if a message is successfully received */
        if (apbInst.errorTick == 0) {
            /* clear communication fault flag */
            apbInst.error = 0;
        }
    }
}

int8_t apb_isErrored() {
    return apbInst.error;
}

/* This clears the message buffer so make sure you have all the data stored from the command */
void apb_sendDefualtResponse() {
    apb_initResponse ();
    apb_sendResponse ();
}

/* This clears the message buffer so make sure you have all the data stored from the command */
void apb_initResponse() {
    if (apbInst.apbStatus == RECEIVE_MESSAGE) {
        apb_clearMessageBuffer();
        apbInst.message[0] = apbInst.address;
        apbInst.message[1] = apbInst.function;
        apbInst.messageLength = 3; /* 3 to include the length */
    }
}

void apb_appendToResponse(uint8_t data) {
    apbInst.message[apbInst.messageLength++] = data;
}

void apb_addToResponse(void* data, size_t length) {
    if (apbInst.apbStatus == RECEIVE_MESSAGE) {
        memcpy(&(apbInst.message[apbInst.messageLength]), data, length);
        apbInst.messageLength += (uint8_t)length;
    }
}

void apb_sendResponse() {
    uint8_t crc[2];
    apbInst.messageLength += 2; /* 2 for crc */
    apbInst.message[2] = apbInst.messageLength;
    apb_crc16(apbInst.message, crc, apbInst.messageLength);
    apbInst.message[apbInst.messageLength - 2] = crc[0];
    apbInst.message[apbInst.messageLength - 1] = crc[1];
    apbInst.putsch(apbInst.message, apbInst.messageLength);
}

void apb_restart() {
    apbInst.messageCount = 0;
    apbInst.messageLength = 0;
    apbInst.function = 0;
    apbInst.framingTick = 0;
    apbInst.apbStatus = WAIT_FOR_FRAMING;
    apb_clearMessageBuffer();
}

void apb_clearMessageBuffer() {
    int i;
    for (i = 0; i < MESSAGE_BUFFER_LENGTH; ++i) {
        apbInst.message[i] = 0;
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
