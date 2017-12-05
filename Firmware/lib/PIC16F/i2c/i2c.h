/*
 * I2C
 * 
 */

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

#ifndef I2C_H
#define	I2C_H

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
#ifdef _16F18855
#define SSP1                &SSP1BUF
#define SSP2                &SSP2BUF

#define STAT_SMP_STANDARD   0x80    /* SMP: SPI Data Input Sample bit, Slew rate control disabled for Standard Speed mode (100 kHz and 1 MHz) */
#define STAT_SMP_HIGH_SPEED 0x00    /* SMP: SPI Data Input Sample bit, Slew rate control enabled for High-Speed mode (400 kHz) */
#define STAT_CKE_SMP        0x40    /* CKE: SPI Clock Edge Select bit, Enable input logic so that thresholds are compliant with SMBus specification */
#define STAT_CKE_STANDARD   0x00    /* CKE: SPI Clock Edge Select bit, Disable SMBus specific inputs */
#endif

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/
typedef enum i2cStatusEnum {
    IDLE,
    WRITE_SEND_ADDRESS,
    WRITE_SEND_FIRST_BYTE,
    WRITE_SEND_DATA,
    WRITE_SEND_STOP,
    WRITE_COMPLETE,
    READ_SEND_ADDRESS,
    READ_CHECK_ACKNOWLEDGED,
    READ_RECIEVING_DATA,
    READ_SEND_ACKNOWLEDGED,
    READ_SEND_NOT_ACKNOWLEDGED,
    READ_COMPLETE
}i2cStatus_t;

struct i2cObjStruct {
    uint8_t slaveAddress;
    uint8_t index;
    uint8_t* writeBuffer;
    uint8_t writeCount;
    uint8_t* readBuffer;
    uint8_t readCount;
    uint8_t failCount;
    i2cStatus_t status;
    volatile uint8_t* sspModule;
    uint8_t* addressPtr;
};

typedef struct i2cObjStruct * i2cObj;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
int8_t i2c_init(i2cObj self, 
        volatile uint8_t* sspModuleVar, 
        uint8_t stat, 
        uint8_t add);           /* SSPxADD = [Fosc / (Baud Rate * 4)] - 1 */

void i2c_enable(i2cObj self);
void i2c_isr(i2cObj self);

int8_t i2c_write(i2cObj self, 
        uint8_t slaveAddressVar, 
        uint8_t* writeBufferVar, 
        uint8_t writeCountVar);

int8_t i2c_read(i2cObj self, 
        uint8_t slaveAddressVar, 
        uint8_t* readBufferVar, 
        uint8_t readCountVar, 
        uint8_t* writeBufferVar, 
        uint8_t writeCountVar);

int8_t i2c_available(i2cObj self);

#endif	/* I2C_H */

