/*******************************************************************************
 * mcp3428.c
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

#include <stdlib.h>     /* null, size_t */
#include <stdint.h>     /* uint8_t, int8_t */
#include "../mcc_generated_files/i2c1.h"
#include "mcp3428.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/* polling called every 1ms */
#define TIMEOUT_SETPOINT    10  /* 10mSec */
#define RETRY_SETPOINT      5  
#define CONVERSION_TIME     67  /* At 15 SPS it will take 66.6mSecs to complete */

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
typedef enum mcp3428StatusEnum {
    STARTUP,
    STANDBY,
    START_CONVERSION,
    WAIT_FOR_CONVERSION,
    WAIT_FOR_STATUS
}mcp3428Status_t;

struct mcp3428ObjStruct {
    uint8_t address;
    uint8_t configWord;
    int16_t result;
    uint8_t response[3];
    mcp3428Status_t mcp3428Status;
    uint8_t timeOutCount;
    uint8_t retryCount;
    I2C1_MESSAGE_STATUS i2cStatus;
};

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/
static struct mcp3428ObjStruct mcp3428Inst;
static uint8_t channelLookup[4] = {0x00, 0x20, 0x40, 0x60};

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
int8_t mcp3428_init(uint8_t addressVar) {
    mcp3428Inst.mcp3428Status = STARTUP;
    mcp3428Inst.address = addressVar;
    /* Setup the MCP3428 for single shot, 15 SPS, x1 gain */
    mcp3428Inst.configWord = 0x08;
    
    I2C1_MasterWrite(&(mcp3428Inst.configWord),
            1,
            mcp3428Inst.address,
            &(mcp3428Inst.i2cStatus));
    
    while (mcp3428Inst.i2cStatus == I2C1_MESSAGE_PENDING);
    
    /* The message was successful */
    if (mcp3428Inst.i2cStatus == I2C1_MESSAGE_COMPLETE) {
        return 1;
    }
    
    /* Return false for everything else */
    return 0;
}

int8_t mcp3428_setChannelAndStartConversion(uint8_t channel) {
    if (mcp3428Inst.mcp3428Status == STANDBY) {
        /* Clear the channel bits in the config word */
        mcp3428Inst.configWord &= 0x9F;
        /* Set the channel bits in the config word */
        mcp3428Inst.configWord |= channelLookup[channel];
        
        /* Set the ready flag */
        mcp3428Inst.configWord |= 0x80;
        
        mcp3428Inst.timeOutCount = 0;
        mcp3428Inst.retryCount = 0;
        I2C1_MasterWrite(&(mcp3428Inst.configWord),
                1,
                mcp3428Inst.address,
                &(mcp3428Inst.i2cStatus));
        
        mcp3428Inst.mcp3428Status = START_CONVERSION;
        return 1;
    }
        
    return 0;
}

/* Call at 1ms */
int16_t mcp3428_polling() {
    switch (mcp3428Inst.mcp3428Status) {
        case START_CONVERSION:
            /* Waiting for the message to be done sending */
            if (mcp3428Inst.i2cStatus == I2C1_MESSAGE_PENDING) {
                mcp3428Inst.timeOutCount++;
                /* Slave has taken too long, quit */
                if (mcp3428Inst.timeOutCount >= TIMEOUT_SETPOINT) {
                    mcp3428Inst.result = -1;
                    mcp3428Inst.mcp3428Status = STANDBY;
                }
                
            /* Message was successful */
            } else if (mcp3428Inst.i2cStatus == I2C1_MESSAGE_COMPLETE) {
                mcp3428Inst.timeOutCount = 0;
                mcp3428Inst.mcp3428Status = WAIT_FOR_CONVERSION;
                
            /* Something went wrong */
            } else {
                mcp3428Inst.retryCount++;
                
                /* Try writing to the slave again */
                if (mcp3428Inst.retryCount < RETRY_SETPOINT) {
                    mcp3428Inst.timeOutCount = 0;
                    I2C1_MasterWrite(&(mcp3428Inst.configWord),
                            1,
                            mcp3428Inst.address,
                            &(mcp3428Inst.i2cStatus));
                   
                /* Retried to many times, quit */
                } else {
                    mcp3428Inst.result = -1;
                    mcp3428Inst.mcp3428Status = STANDBY;
                }
            }
            
            break;
        case WAIT_FOR_CONVERSION:
            /* Wait some time for the conversion to complete */
            mcp3428Inst.timeOutCount++;
            if (mcp3428Inst.timeOutCount >= CONVERSION_TIME) {
                mcp3428Inst.timeOutCount = 0;
                mcp3428Inst.retryCount = 0;
                I2C1_MasterRead(mcp3428Inst.response,
                        3,
                        mcp3428Inst.address,
                        &(mcp3428Inst.i2cStatus));
                
                mcp3428Inst.mcp3428Status = WAIT_FOR_STATUS;
            }
            
            break;
        case WAIT_FOR_STATUS:
            /* Waiting for the message to be done sending */
            if (mcp3428Inst.i2cStatus == I2C1_MESSAGE_PENDING) {
                mcp3428Inst.timeOutCount++;
                /* Slave has taken too long, quit */
                if (mcp3428Inst.timeOutCount >= TIMEOUT_SETPOINT) {
                    mcp3428Inst.result = -1;
                    mcp3428Inst.mcp3428Status = STANDBY;
                }
                
            /* Message was successful */
            } else if (mcp3428Inst.i2cStatus == I2C1_MESSAGE_COMPLETE) {
                /* The conversion isn't done wait a little bit of time and */
                /* request again */
                if (mcp3428Inst.response[2] & 0x80) {
                    mcp3428Inst.timeOutCount = CONVERSION_TIME - 10;
                    mcp3428Inst.mcp3428Status = WAIT_FOR_CONVERSION;
                    
                /* Conversion is complete */
                } else {
                    /* Remove sign bit and shift MSB up 8 bits */
                    mcp3428Inst.result = (mcp3428Inst.response[0] & 0x7F) << 8;
                    /* Append LSB */
                    mcp3428Inst.result |= mcp3428Inst.response[1];
                    
                    /* The sign bit is high */
                    if (mcp3428Inst.response[0] & 0x80) {
                        mcp3428Inst.result *= -1;
                    }
                    
                    mcp3428Inst.mcp3428Status = STANDBY;
                }
                
            /* Something went wrong */
            } else {
                mcp3428Inst.retryCount++;
                
                /* Try reading from the slave again */
                if (mcp3428Inst.retryCount < RETRY_SETPOINT) {
                    mcp3428Inst.timeOutCount = 0;
                    I2C1_MasterRead(mcp3428Inst.response,
                            3,
                            mcp3428Inst.address,
                            &(mcp3428Inst.i2cStatus));
                   
                /* Retried to many times, quit */
                } else {
                    mcp3428Inst.result = -1;
                    mcp3428Inst.mcp3428Status = STANDBY;
                }
            }
            
            break;   
        case STARTUP:
        case STANDBY:
        default:
            break;
    }
    
    return mcp3428_getResult();
}

int8_t mcp3428_getChannel() {
    int8_t channel = -1;
    switch (mcp3428Inst.configWord & 0x60) {
        case 0x00:
            channel = 0;
            break;
        case 0x20:
            channel = 1;
            break;
        case 0x40:
            channel = 2;
            break;
        case 0xC0:
            channel = 3;
            break;
        default:
            break;
    }
    return channel;
}

int16_t mcp3428_getResult() {
    if (mcp3428Inst.mcp3428Status == STANDBY) {
        return mcp3428Inst.result;
    }
    return -1;
}