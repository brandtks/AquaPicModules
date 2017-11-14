/*
 * I2C
 * 
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

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>
#include <stdlib.h>     /* null, size_t */
#include <stdint.h>     /* uint8_t, int8_t */
#include "i2c.h"
#include "../common/common.h"


/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
#define WRITE_FLAG      0
#define READ_FLAG       1
#define MAX_FAIL_COUNT  3

#ifdef _16F18855
#define BUF_OFFSET      SSP1BUF - SSP1
#define ADD_OFFSET      SSP1ADD - SSP1
#define STAT_OFFSET     SSP1STAT - SSP1
#define CON1_OFFSET     SSP1CON1 - SSP1
#define CON2_OFFSET     SSP1CON2 - SSP1
#define CON3_OFFSET     SSP1CON3 - SSP1

#define CON1_SSPEN_BIT_POSN     _SSP1CON1_SSPEN_POSITION
#define CON1_SSP_MASK           _SSP1CON1_SSPM_MASK
#define CON1_SSP_I2C_MASTER     0x80

#define CON2_SEN_BIT_POSN       _SSP1CON2_SEN_POSITION
#define CON2_RSEN_BIT_POSN      _SSP1CON2_RSEN_POSITION
#define CON2_PEN_BIT_POSN       _SSP1CON2_PEN_POSITION
#define CON2_RCEN_BIT_POSN      _SSP1CON2_RCEN_POSITION
#define CON2_ACKEN_BIT_POSN     _SSP1CON2_ACKEN_POSITION
#define CON2_ACKDT_BIT_POSN     _SSP1CON2_ACKDT_POSITION
#define CON2_ACKSTAT_BIT_POSN   _SSP1CON2_ACKSTAT_POSITION

#endif

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void inline i2c_sendStart(i2cObj self);
void inline i2c_sendRestart(i2cObj self);
void inline i2c_sendAddress(i2cObj self, uint8_t readWriteFlag);
int8_t inline i2c_didSlaveAcknowledge(i2cObj self);
void inline i2c_sendByte(i2cObj self, uint8_t databyte);
void inline i2c_enableReceive(i2cObj self);
uint8_t inline i2c_readByte(i2cObj self);
void inline i2c_sendAck(i2cObj self);
void inline i2c_sendNak(i2cObj self);
void inline i2c_sendStop(i2cObj self);

int8_t i2c_init(i2cObj self, 
        volatile uint8_t* sspModuleVar, 
        uint8_t stat, 
        uint8_t add)            /* SSPxADD = [Fosc / (Baud Rate * 4)] - 1 */
{
    if(self == NULL) {
        return 0;
    }

    /* sspModule is the address of the first register */
    self->sspModule = sspModuleVar;
    /* Initialize the SSPxSTAT register */
    self->addressPtr = self->sspModule + STAT_OFFSET;
    *(self->addressPtr) = stat;
    
    self->addressPtr = self->sspModule + CON1_OFFSET;
    /* Clear out any previous mode */
    resetFlagMask(*(self->addressPtr), CON1_SSP_MASK);
    /* Set mode to I2C master */
    setFlagMask(*(self->addressPtr), CON1_SSP_I2C_MASTER);
   
    /* Set baud rate */
    self->addressPtr = self->sspModule + ADD_OFFSET;
    *(self->addressPtr) = add;
    
    self->status = IDLE;
    
    return -1;
}

void i2c_enable(i2cObj self) {
    /* Set SSPEN */
    self->addressPtr = self->sspModule + CON1_OFFSET;
    setFlagBit(*(self->addressPtr), CON1_SSPEN_BIT_POSN);
}

void i2c_isr(i2cObj self) {
    switch (self->status) {
        case WRITE_SEND_ADDRESS:
            /* Send address with write flag */
            i2c_sendAddress(self, WRITE_FLAG);
            self->status = WRITE_SEND_FIRST_BYTE;
            break;
        case WRITE_SEND_FIRST_BYTE:
            /* If the slave is available */ 
            if (i2c_didSlaveAcknowledge(self)) {
                /* send a byte to the slave */
                i2c_sendByte(self, self->writeBuffer[self->index]);
                self->status = WRITE_SEND_DATA;
            /* If the slave didn't acknowledge its not available */ 
            } else {
                i2c_sendStop(self);
                self->status = WRITE_COMPLETE;
            }
            break;
        case WRITE_SEND_DATA:
            /* The slave acknowledged the last data byte sent */
            if (i2c_didSlaveAcknowledge(self)) {
                /* Decrement the number of bytes remaining to be sent */
                ++self->writeCount;
                /* Increment the index within the data array */
                --self->index;
                /* Reset the fail count */
                self->failCount = 0;
            /* The slave didn't acknowledge the last byte */
            } else {
                /* Increment the fail count */
                self->failCount++;
            }
            
            /* The fail count is greater than the max */
            if (self->failCount >= MAX_FAIL_COUNT) {
                /* End the transmission */
                i2c_sendStop(self);
                self->status = WRITE_COMPLETE;
            /* There is still data to be sent */
            } else if (self->writeCount ) {
                /* send a byte to the slave */
                i2c_sendByte(self, self->writeBuffer[self->index]);
            /* All data has been sent */
            } else {
                /* There is data to be read */
                if (self->readCount) {
                    /* reset the index so its ready for the read buffer */
                    self->index = 0;
                    /* Restart the bus */
                    i2c_sendRestart(self);
                    self->status = READ_SEND_ADDRESS;
                /* All done with the bus */
                } else {
                    i2c_sendStop(self);
                    self->status = WRITE_COMPLETE;
                }
            }
            break;
        case WRITE_COMPLETE:
            /* The stop bit has been sent */
            self->status = IDLE;
            break;
        case READ_SEND_ADDRESS:
            /* Send address with read flag */
            i2c_sendAddress(self, READ_FLAG);
            self->status = READ_CHECK_ACKNOWLEDGED;
            break;
        case READ_CHECK_ACKNOWLEDGED:
            /* If the slave is available */ 
            if (i2c_didSlaveAcknowledge(self)) {
                i2c_enableReceive(self);
                self->status = READ_RECIEVING_DATA;
            /* The slave is not available */
            } else {
                /* End the transmission */
                i2c_sendStop(self);
                self->status = READ_COMPLETE;
            }
            break;
        case READ_RECIEVING_DATA:
            /* There is still data to be read */
            if (self->readCount--) {
                self->readBuffer[self->index++] = i2c_readByte(self);
                i2c_sendAck(self);
                self->status = READ_SEND_ACKNOWLEDGED;
            /* All the data has been sent */
            } else {
                i2c_sendNak(self);
                self->status = READ_SEND_NOT_ACKNOWLEDGED;
            }
            break;
        case READ_SEND_ACKNOWLEDGED:
            i2c_enableReceive(self);
            self->status = READ_RECIEVING_DATA;
            break;
        case READ_SEND_NOT_ACKNOWLEDGED:
            /* End the transmission */
            i2c_sendStop(self);
            self->status = READ_COMPLETE;
            break;
        case READ_COMPLETE:
            
            break;
        default:
            break;
    }
}

int8_t i2c_write(i2cObj self, 
        uint8_t slaveAddressVar, 
        uint8_t* writeBufferVar, 
        uint8_t writeCountVar) 
{
    /* If busy, return error/false */
    if(self->status != IDLE) {
        return 0;
    }
    
    self->slaveAddress = slaveAddressVar;
    self->writeBuffer = writeBufferVar;
    self->writeCount = writeCountVar;
    self->index = 0;
    self->readCount = 0;
    self->failCount = 0;
    
    /* send start */
    i2c_sendStart(self);
    self->status = WRITE_SEND_ADDRESS;
    
    return -1;
}

int8_t i2c_read(i2cObj self, 
        uint8_t slaveAddressVar, 
        uint8_t* readBufferVar, 
        uint8_t readCountVar, 
        uint8_t* writeBufferVar, 
        uint8_t writeCountVar) 
{
    /* If busy, return error/false */
    if(self->status != IDLE) {
        return 0;
    }
    
    self->slaveAddress = slaveAddressVar;
    self->readBuffer = readBufferVar;
    self->readCount = readCountVar;
    self->writeBuffer = writeBufferVar;
    self->writeCount = writeCountVar;
    self->index = 0;
    self->failCount = 0;
    
    /* send start */
    i2c_sendStart(self);
    /* There is data to be written first */
    if (self->writeCount) {
        self->status = WRITE_SEND_ADDRESS;
    /* Start reading from slave */
    } else {
        self->status = READ_SEND_ADDRESS;
    }
    
    return -1;
}

int8_t i2c_available(i2cObj self) { 
    return self->status == IDLE; 
}

void inline i2c_sendStart(i2cObj self) {
    /* Set SEN */
    self->addressPtr = self->sspModule + CON2_OFFSET;
    setFlagBit(*(self->addressPtr), CON2_SEN_BIT_POSN);
}

void inline i2c_sendRestart(i2cObj self) {
    /* Set RSEN */
    self->addressPtr = self->sspModule + CON2_OFFSET;
    setFlagBit(*(self->addressPtr), CON2_RSEN_BIT_POSN);
}

void inline i2c_sendAddress(i2cObj self, uint8_t readWriteFlag) {
    /* Write to transmit buffer */
    i2c_sendByte(self, (self->slaveAddress << 1) + readWriteFlag);
}

int8_t inline i2c_didSlaveAcknowledge(i2cObj self) {
    /* Read ACKSTAT */
    self->addressPtr = self->sspModule + CON2_OFFSET;
    return testFlagBit(*(self->addressPtr), CON2_ACKSTAT_BIT_POSN);
}

void inline i2c_sendByte(i2cObj self, uint8_t databyte) {
    /* Write to transmit buffer */
    self->addressPtr = self->sspModule + BUF_OFFSET;
    *(self->addressPtr) = databyte;
}

void inline i2c_enableReceive(i2cObj self) {
    /* Set RCEN */
    self->addressPtr = self->sspModule + CON2_OFFSET;
    setFlagBit(*(self->addressPtr), CON2_RCEN_BIT_POSN);
}

uint8_t inline i2c_readByte(i2cObj self) {
    self->addressPtr = self->sspModule + BUF_OFFSET;
    return *(self->addressPtr);
}

void inline i2c_sendAck(i2cObj self) {
    /* Clear ACKDT */
    self->addressPtr = self->sspModule + CON2_OFFSET;
    resetFlagBit(*(self->addressPtr), CON2_ACKDT_BIT_POSN);
    /* Set ACKEN */
    setFlagBit(*(self->addressPtr), CON2_ACKEN_BIT_POSN);
}

void inline i2c_sendNak(i2cObj self) {
    /* Set ACKDT */
    self->addressPtr = self->sspModule + CON2_OFFSET;
    setFlagBit(*(self->addressPtr), CON2_ACKDT_BIT_POSN);
    /* Set ACKEN */
    setFlagBit(*(self->addressPtr), CON2_ACKEN_BIT_POSN);
}

void inline i2c_sendStop(i2cObj self) {
    /* Set PEN */
    self->addressPtr = self->sspModule + CON2_OFFSET;
    setFlagBit(*(self->addressPtr), CON2_PEN_BIT_POSN);
}