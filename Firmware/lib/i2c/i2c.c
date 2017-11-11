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
/* Functions                                                                  */
/******************************************************************************/
int8_t i2c_init(i2cObj self, uint16_t config1, uint16_t baudDivider,  uint8_t intPriority) {
    if(self == NULL) {
        return 0;
    }

    I2C_CON_REG = config1;
    I2C_BRG_REG = baudDivider;

    self->idle_B = 1;

    I2C_INT_PRI = intPriority;
    I2C_INT_FLAG = 0;
    I2C_INT_EN = 1;
}

void i2c_irqRoutine(i2cObj self) {
    uint8_t status = 0;

    I2C_INT_FLAG = 0; // clear interrupt flag

    // if not in idle condition
    if(!self->idle_B) { 
        // receiving data from slave
        if(self->rcvNWRT_B) { 
            // if reading from slave
            if(self->reading_B) {
                // if address not sent
                if(!self->addSent_B) {
                    I2C_sendAddress(self, _READ); // send address
                // address has been sent
                } else  {
                    // if last condition was an acknowledgement
                    if(self->acking_B)  {
                        self->acking_B = 0; // clear the acknowledgement bit
                        // if not reading anymore bytes
                        if(!self->count)  {
                            I2C_sendStopBit(self); // generate stop condition
                        }
                    // last state was byte sent completed from slave
                    } else {
                        *(self->writeBuffer + self->index) = I2C_RCV_BUFF; // save received register to pointer memory
                        self->count--; // decrease count of number of bytes to read
                        self->index++; // increment index
                        // if there are still bytes to read
                        if(self->count) { 
                            i2c_sendACK(self); // send an acknowledge last byte
                        // no more bytes to read
                        } else {
                            i2c_sendNAK(self); // send the final acknowledge
                        }
                    }
                }
            // sending register pointer before reading
            } else {
                // address not sent
                if(!self->addSent_B) {
                    I2C_sendAddress(self, _WRITE); // send address
                // address was sent
                } else {
                    if(self->register_B) // register sending required
                    {
                        status = I2C_sendByte(self->registerReq); // send register
                        if(!status) self->register_B = 0; // if no errors returned clear register send required
                    }
                    else
                    {
                        self->addSent_B = 0; // clear address sent bit
                        self->reading_B = 1; // set to reading state
                        I2C_RESTART_EN = 1; // generate repeated start condition
                    }
                }
            }
        }
        else // writing to slave
        {
            if(!self->addSent_B) // address has not been sent
            {
                I2C_sendAddress(self, _WRITE); // send address
            }
            else // address has been sent
            {
                if(self->register_B) // register needs to be sent
                {
                    status = I2C_sendByte(self->registerReq); // send register
                    if(!status) self->register_B = 0; // if no errors returned clear register send required
                }
                else // no register to send or has been sent
                {
                    if(self->count) // if bytes to send
                    {
                        status = I2C_sendByte(*(self->writeBuffer + self->index)); // send byte in pointer memory
                        if(!status) // no errors returned
                        {
                            self->count--; // decrement num of bytes to send
                            self->index++; // increment index
                        }
                    }
                    else // if no more bytes to send
                    {
                        I2C_sendStopBit(self); // stop comms
                    }
                }
            }
        }
    }
}

void i2c_irqRoutine(i2cObj self) {
    switch (self->status) {
        case WRITE_SEND_ADDRESS:
            /* Send address with write flag */
            i2c_sendAddress(self, WRITE_FLAG);
            self->status = WRITE_SEND_DATA;
            break;
        case WRITE_SEND_DATA:
            /* i2c_sendByte returns true when byte loaded into buffer */
            if (i2c_sendByte(self->writeBuffer[self->index])) {
                /* decrement number of bytes to be sent */
                --self->writeCount;
                /* increment current index with data */
                ++self->index;
            }
            
            /* all data has been sent */
            if (self->writeCount == 0) {
                self->status = WRITE_COMPLETE;
            }
            break;
        case WRITE_COMPLETE:
            /* stop communication */
            i2c_sendStopBit();
            self->status = IDLE;
            break;
        case READ_SEND_ADDRESS:
            /* Send address with write flag */
            i2c_sendAddress(self, READ_FLAG);
            /* Some data needs to be sent before reading */
            if (self->writeCount != 0) {
                self->status = READ_SEND_DATA;
            /* There's no extra data go straight to reading */
            } else {
                self->status = READ_RECIEVING_DATA;
            }
            break;
        case READ_SEND_DATA:
            /* i2c_sendByte returns true when byte loaded into buffer */
            if (i2c_sendByte(self->writeBuffer[self->index])) {
                /* decrement number of bytes to be sent */
                --self->writeCount;
                /* increment current index with data */
                ++self->index;
            }
            
            /* all data has been sent */
            if (self->writeCount == 0) {
                self->status = READ_RECIEVING_DATA;
            }
            break;
        case READ_RECIEVING_DATA:
            
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
    
    self->status = WRITE_SEND_ADDRESS;
    self->slaveAddress = slaveAddressVar;
    self->writeBuffer = writeBufferVar;
    self->writeCount = writeCountVar;
    self->index = 0;
    
    I2C_START_EN = 1;
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

    self->status = READ_SEND_ADDRESS;
    self->slaveAddress = slaveAddressVar;
    self->readBuffer = readBufferVar;
    self->readCount = readCountVar;
    self->writeBuffer = writeBufferVar;
    self->writeCount = writeCountVar;
    self->index = 0;
    
    I2C_START_EN = 1;
    return -1;
}

int8_t i2c_send_Wreg(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr, uint8_t databyte)
{
    if(!self->idle_B) return I2C_ERR_BUSY;

    static uint8_t byteA[1];

    self->idle_B = 0;
    //self->started_B = 0;
    self->rcvNWRT_B = _WRITE;
    self->register_B = 1;
    self->sAddress = deviceAddress;
    self->registerReq = registerPtr;
    self->count = 1;
    self->index = 0;
    byteA[0] = databyte;
    self->writeBuffer = byteA;
    //I2C_sendStartBit(self);
    I2C_START_EN = 1;

    return ERR_NOERROR;
}

int8_t i2c_sendArray(i2cObj self, uint8_t deviceAddress, uint8_t *databytePtr, uint16_t arraySize)
{
    if(!self->idle_B) return I2C_ERR_BUSY;

    self->idle_B = 0;
    //self->started_B = 0;
    self->rcvNWRT_B = _WRITE;
    self->register_B = 0;
    self->sAddress = deviceAddress;
    self->count = arraySize;
    self->index = 0;
    self->writeBuffer = databytePtr;
    //I2C_sendStartBit(self);
    I2C_START_EN = 1;

    return ERR_NOERROR;
}

int8_t i2c_sendArray_Wreg(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr, uint8_t *databytePtr, uint16_t arraySize)
{
    if(!self->idle_B) return I2C_ERR_BUSY;

    self->idle_B = 0;
    //self->started_B = 0;
    self->rcvNWRT_B = _WRITE;
    self->register_B = 1;
    self->sAddress = deviceAddress;
    self->registerReq = registerPtr;
    self->count = arraySize;
    self->index = 0;
    self->writeBuffer = databytePtr;
    //I2C_sendStartBit(self);
    I2C_START_EN = 1;

    return ERR_NOERROR;
}

int8_t i2c_read(i2cObj self, uint8_t deviceAddress, uint8_t *databytePtr) {
    if(!self->idle_B) return I2C_ERR_BUSY;

    self->idle_B = 0;
    //self->started_B = 0;
    self->rcvNWRT_B = _READ;
    self->reading_B = 1;
    self->register_B = 0;
    self->sAddress = deviceAddress;
    self->count = 1;
    self->index = 0;
    #ifdef qp_port_h
        self->writeBuffer = (uint8_t*)calloc(1, sizeof(uint8_t));
    #else
        self->writeBuffer = databytePtr;
    #endif
    //I2C_sendStartBit(self);
    I2C_START_EN = 1;

    return ERR_NOERROR;
}

int8_t i2c_read_Wreg(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr, uint8_t *databytePtr)
{
    if(!self->idle_B) return I2C_ERR_BUSY;

    self->idle_B = 0;
    //self->started_B = 0;
    self->rcvNWRT_B = _READ;
    self->reading_B = 0;
    self->register_B = 1;
    self->sAddress = deviceAddress;
    self->registerReq = registerPtr;
    self->count = 1;
    self->index = 0;
    self->writeBuffer = databytePtr;
    //I2C_sendStartBit(self);
    I2C_START_EN = 1;

    return ERR_NOERROR;
}

int8_t i2c_readArray(i2cObj self, uint8_t deviceAddress, uint8_t *databytePtr, uint16_t arraySize)
{
    if(!self->idle_B) return I2C_ERR_BUSY;

    self->idle_B = 0;
    //self->started_B = 0;
    self->rcvNWRT_B = _READ;
    self->reading_B = 1;
    self->register_B = 0;
    self->sAddress = deviceAddress;
    self->count = arraySize;
    self->index = 0;
    #ifdef qp_port_h
        self->writeBuffer = (uint8_t*)calloc(arraySize, sizeof(uint8_t));
    #else
        self->writeBuffer = databytePtr;
    #endif
    //I2C_sendStartBit(self);
    I2C_START_EN = 1;

    return ERR_NOERROR;
}

int8_t i2c_readArray_Wreg(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr, uint8_t *databytePtr, uint16_t arraySize) {
    if(!self->idle_B) return I2C_ERR_BUSY;

    self->idle_B = 0;
    //self->started_B = 0;
    self->rcvNWRT_B = _READ;
    self->reading_B = 0;
    self->register_B = 1;
    self->sAddress = deviceAddress;
    self->registerReq = registerPtr;
    self->count = arraySize;
    self->index = 0;
    self->writeBuffer = databytePtr;
    //I2C_sendStartBit(self);
    I2C_START_EN = 1;

    return ERR_NOERROR;
}

int8_t i2c_sendRegisterPtr(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr) {
    if(!self->idle_B) return I2C_ERR_BUSY;

    self->idle_B = 0;
    self->rcvNWRT_B = _WRITE;
    self->register_B = 1;
    self->sAddress = deviceAddress;
    self->registerReq = registerPtr;
    self->count = 0;
    self->index = 0;
    I2C_START_EN = 1;

    return ERR_NOERROR;
}

int8_t i2c_waitForSlave(i2cObj self, uint8_t deviceAddress) {
    if(!self->idle_B) return I2C_ERR_BUSY;

    I2C_INT_EN = 0; // disable I2C interrupts
    uint8_t ACK_bit = 1;
    while (ACK_bit)
    {
        I2C_INT_FLAG = 0; // clear the interrupt flag
        I2C_START_EN = 1; // initiate a start
        while(!I2C_INT_FLAG); // wait for interrupt flag
        I2C_INT_FLAG = 0; // clear interrupt flag
        I2C_sendByte((deviceAddress << 1) + _WRITE); //send address
        while(!I2C_INT_FLAG); //wait for interrupt flag
        ACK_bit = I2C_ACK_STAT; //Ack bit will come back low when the write is complete
    }
    I2C_INT_FLAG = 0; // clear the interrupt flag
    I2C_INT_EN = 1; // enable I2C interrupts

    return ERR_NOERROR;
}

uint8_t i2c_available(i2cObj self) { 
    return self->idle_B; 
}
void i2c_waitForModule(i2cObj self) { 
    while(!self->idle_B); 
}

int8_t i2c_sendByte(uint8_t databyte) {
    /* transmit buffer still full */
    if (I2C_TRANS_STAT) {
        return 0;
    }
    I2C_TRN_BUFF = databyte;
    return -1;
}

void i2c_sendAddress(i2cObj self, uint8_t readWriteFlag) {
    I2C_TRN_BUFF = ((self->slaveAddress << 1) + readWriteFlag);
}

void i2c_sendStopBit() {
    I2C_STOP_EN = 1;
}

void i2c_sendACK(i2cObj self) {
    I2C_ACK_DATA = 0;
    I2C_ACK_EN = 1;
    self->acking_B = 1;
}

void i2c_sendNAK(i2cObj self) {
    I2C_ACK_DATA = 1;
    I2C_ACK_EN = 1;
    self->acking_B = 1;
}