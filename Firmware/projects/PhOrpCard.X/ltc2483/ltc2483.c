/*******************************************************************************
 * mcp3428->c
 *
 * Goodtime Development 
 * 
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  MCP3428 Analog to digital converter device driver
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
 * of the License, or (at your option) any later version->
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE->  See the
 * GNU General Public License for more details->
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc->, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA->
 *
 * Optionally you can also view the license at <http://www->gnu->org/licenses/>->
 ******************************************************************************/ 

#include <stdlib.h>     /* null, size_t */
#include <stdint.h>     /* uint8_t, int8_t */
#include "../mcc_generated_files/i2c1.h"
#include "ltc2483.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* polling called every 1ms */
#define TIMEOUT_SETPOINT    10  /* 10mSec */
#define RETRY_SETPOINT      5  
#define CONVERSION_TIME     143

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
int8_t ltc2483_init(ltc2483Obj ltc2483Inst, uint8_t addressVar) {
    ltc2483Inst->result = -1;
    ltc2483Inst->address = addressVar;
    ltc2483Inst->ltc2483Status = WAIT_FOR_CONVERSION;
    ltc2483Inst->timeOutCount = 0;
    ltc2483Inst->retryCount = 0;
    return 0;
}

/* Call at 1ms */
int16_t ltc2483_polling(ltc2483Obj ltc2483Inst) {
    switch (ltc2483Inst->ltc2483Status) {
        case WAIT_FOR_CONVERSION:
            /* Wait some time for the conversion to complete */
            ltc2483Inst->timeOutCount++;
            if (ltc2483Inst->timeOutCount >= CONVERSION_TIME) {
                ltc2483Inst->timeOutCount = 0;
                ltc2483Inst->retryCount = 0;
                I2C1_MasterRead(ltc2483Inst->response,
                        3,
                        ltc2483Inst->address,
                        &(ltc2483Inst->i2cStatus));
                
                ltc2483Inst->ltc2483Status = WAIT_FOR_STATUS;
            }
            
            break;
        case WAIT_FOR_STATUS:
            /* Waiting for the message to be done sending */
            if (ltc2483Inst->i2cStatus == I2C1_MESSAGE_PENDING) {
                ltc2483Inst->timeOutCount++;
                /* Slave has taken too long, quit */
                if (ltc2483Inst->timeOutCount >= TIMEOUT_SETPOINT) {
                    ltc2483Inst->result = -1;
                    ltc2483Inst->timeOutCount = 0;
                    ltc2483Inst->ltc2483Status = WAIT_FOR_CONVERSION;
                }
                
            /* Message was successful */
            } else if (ltc2483Inst->i2cStatus == I2C1_MESSAGE_COMPLETE) {
                /* Conversion is complete */
                /* Remove sign bit and MSb and shift MSB up 10 bits */
                ltc2483Inst->result = (ltc2483Inst->response[0] & 0x3F) << 10;
                /* Append Middle Byte */
                ltc2483Inst->result |= ltc2483Inst->response[1] << 2;
                /* Append LSbs */
                /* Remove everything but the 2 MSbs and shift down 6 */
                ltc2483Inst->result |= (ltc2483Inst->response[2] & 0xC0) >> 6;

                /* The LTC2483 automatically starts a conversion on read */
                ltc2483Inst->timeOutCount = 0;
                ltc2483Inst->ltc2483Status = WAIT_FOR_CONVERSION;
                
            /* LTC2483 sends a NACK while conversion is still active */
            } else if (ltc2483Inst->i2cStatus == I2C1_MESSAGE_ADDRESS_NO_ACK) {
                ltc2483Inst->timeOutCount = CONVERSION_TIME - 10;
                ltc2483Inst->ltc2483Status = WAIT_FOR_CONVERSION;
            /* Something went wrong */
            } else {
                ltc2483Inst->retryCount++;
                
                /* Try reading from the slave again */
                if (ltc2483Inst->retryCount < RETRY_SETPOINT) {
                    ltc2483Inst->timeOutCount = 0;
                    I2C1_MasterRead(ltc2483Inst->response,
                            3,
                            ltc2483Inst->address,
                            &(ltc2483Inst->i2cStatus));
                   
                /* Retried to many times, quit */
                } else {
                    ltc2483Inst->result = -1;
                    ltc2483Inst->timeOutCount = 0;
                    ltc2483Inst->ltc2483Status = WAIT_FOR_CONVERSION;
                }
            }
            
            break;   
        default:
            break;
    }
    
    return ltc2483_getResult(ltc2483Inst);
}

int16_t ltc2483_getResult(ltc2483Obj ltc2483Inst) {
    if (ltc2483Inst->ltc2483Status == WAIT_FOR_CONVERSION) {
        return ltc2483Inst->result;
    }
    return -1;
}