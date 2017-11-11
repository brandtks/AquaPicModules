/*******************************************************************************
 * I2C MASTER DRIVER CODE
 * Author: Chris Best
 * Microchip Technologies
 * DATE: 07/11/2013
 *
 *******************************************************************************
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the 'Company') is intended and supplied to you, the Company's customer, for
 * use solely and exclusively with products manufactured by the Company.
 *
 * The software is owned by the Company and/or its supplier, and is protected
 * under applicable copyright laws. All rights are reserved. Any use in violation
 * of the foregoing restrictions may subject the user to criminal sanctions under
 * applicable laws, as well as to civil liability for the breach of the terms and
 * conditions of this license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN 'AS IS' CONDITION. NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO
 * THIS SOFTWARE. THE COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 ******************************************************************************/

#ifndef I2C_H
#define	I2C_H

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/

#define DEFAULT_MCONFIG_I2C (I2C_ON & I2C_IDLE_CON & I2C_CLK_HLD & I2C_IPMI_DIS & I2C_7BIT_ADD & I2C_SLW_DIS & I2C_SM_DIS)

#define WRITE_FLAG      0
#define READ_FLAG       1

#define I2C_ON                      0xFFFF /*I2C module enabled */
#define I2C_OFF                     0x7FFF /*I2C module disabled */

#define I2C_IDLE_STOP               0xFFFF /*stop I2C module in Idle mode */
#define I2C_IDLE_CON                0xDFFF /*continue I2C module in Idle mode */

#define I2C_CLK_REL                 0xFFFF /*release clock */
#define I2C_CLK_HLD                 0xEFFF /*hold clock  */

#define I2C_IPMI_EN                 0xFFFF /*IPMI mode enabled */
#define I2C_IPMI_DIS                0xF7FF /*IPMI mode not enabled */

#define I2C_10BIT_ADD               0xFFFF /*I2CADD is 10-bit address */
#define I2C_7BIT_ADD                0xFBFF /*I2CADD is 7-bit address */

#define I2C_SLW_DIS                 0xFFFF /*Disable Slew Rate Control for 100KHz */
#define I2C_SLW_EN                  0xFDFF /*Enable Slew Rate Control for 400KHz */

#define I2C_SM_EN                   0xFFFF /*Enable SM bus specification */
#define I2C_SM_DIS                  0xFEFF /*Disable SM bus specification */

#define I2C_GCALL_EN                0xFFFF /*Enable Interrupt when General call address is received. */
#define I2C_GCALL_DIS               0xFF7F /*Disable General call address. */

#define I2C_STR_EN                  0xFFFF /*Enable clock stretching */
#define I2C_STR_DIS                 0xFFBF /*disable clock stretching */

#define I2C_ACK                     0xFFDF /*Transmit 0 to send ACK as acknowledge */
#define I2C_NACK                    0xFFFF /*Transmit 1 to send NACK as acknowledge*/

#define I2C_ACK_EN                  0xFFFF /*Initiate Acknowledge sequence */
#define I2C_ACK_DIS                 0xFFEF /*Acknowledge condition Idle */

#define I2C_RCV_EN                  0xFFFF /*Enable receive mode */
#define I2C_RCV_DIS                 0xFFF7 /*Receive sequence not in progress */

#define I2C_STOP_EN                 0xFFFF /*Initiate Stop sequence */
#define I2C_STOP_DIS                0xFFFB /*Stop condition Idle */

#define I2C_RESTART_EN              0xFFFF /*Initiate Restart sequence */
#define I2C_RESTART_DIS             0xFFFD /*Start condition Idle */

#define I2C_START_EN                0xFFFF /*Initiate Start sequence */
#define I2C_START_DIS               0xFFFE /*Start condition Idle */

#ifndef I2C_MODULE_ONE || I2C_MODULE_TWO || I2C_MODULE_THREE
    #define I2C_MODULE_ONE
#endif

#ifdef I2C_MODULE_ONE
    /* Bit Macros */
    #define I2C_TRN_BUFF    I2C1TRN
    #define I2C_RCV_BUFF    I2C1RCV
    #define I2C_CON_REG     I2C1CON
    #define I2C_STAT_REG    I2C1STAT
    #define I2C_BRG_REG     I2C1BRG
    #define I2C_INT_FLAG    IFS1bits.MI2C1IF
    #define I2C_START_EN    I2C1CONbits.SEN
    #define I2C_RESTART_EN  I2C1CONbits.RSEN
    #define I2C_STOP_EN     I2C1CONbits.PEN
    #define I2C_RECEIVE_EN  I2C1CONbits.RCEN
    #define I2C_ACK_DATA    I2C1CONbits.ACKDT
    #define I2C_ACK_EN      I2C1CONbits.ACKEN
    #define I2C_ACK_STAT    I2C1STATbits.ACKSTAT
    #define I2C_TRANS_STAT  I2C1STATbits.TRSTAT
    #define I2C_INT_EN      IEC1bits.MI2C1IE
    #define I2C_INT_PRI     IPC4bits.MI2C1P
#endif

/******************************************************************************/
/* Variable Definition                                                        */
/******************************************************************************/
typedef enum i2cStatusEnum {
    IDLE,
    WRITE_SEND_ADDRESS,
    WRITE_SEND_DATA,
    WRITE_COMPLETE,
    READ_SEND_ADDRESS,
    READ_SEND_DATA,
    READ_RECIEVING_DATA,
    READ_COMPLETE
            
}i2cStatus_t;

struct i2cObjStruct {
    uint8_t slaveAddress;
    uint8_t index;
    uint8_t* writeBuffer;
    uint8_t writeCount;
    uint8_t* readBuffer;
    uint8_t readCount;
    i2cStatus_t status;
};

typedef struct i2cObjStruct * i2cObj;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

i2cObj I2C_new(void);

#ifdef qp_port_h
int8_t I2C_init(i2cObj self, uint16_t config1, uint16_t baudDivider, uint16_t i2cRcvEvt_var);
#else
int8_t I2C_init(i2cObj self, uint16_t config1, uint16_t baudDivider);
#endif

void I2C_IRQroutine(i2cObj self);
int8_t I2C_send(i2cObj self, uint8_t deviceAddress, uint8_t databyte);
int8_t I2C_send_Wreg(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr, uint8_t databyte);
int8_t I2C_sendArray(i2cObj self, uint8_t deviceAddress, uint8_t *databytePtr, uint16_t arraySize);
int8_t I2C_sendArray_Wreg(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr, uint8_t *databytePtr, uint16_t arraySize);

#ifdef qp_port_h
int8_t I2C_read(i2cObj self, uint8_t deviceAddress);
#else
int8_t I2C_read(i2cObj self, uint8_t deviceAddress, uint8_t *databytePtr);
#endif

#ifdef qp_port_h
int8_t I2C_read_Wreg(i2cObj self, uint8_t registerPtr, uint8_t deviceAddress);
#else
int8_t I2C_read_Wreg(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr, uint8_t *databytePtr);
#endif

#ifdef qp_port_h
int8_t I2C_readArray(i2cObj self, uint8_t deviceAddress, uint16_t arraySize);
#else
int8_t I2C_readArray(i2cObj self, uint8_t deviceAddress, uint8_t *databytePtr, uint16_t arraySize);
#endif

#ifdef qp_port_h
int8_t I2C_readArray_Wreg(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr, uint16_t arraySize);
#else
int8_t I2C_readArray_Wreg(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr, uint8_t *databytePtr, uint16_t arraySize);
#endif

int8_t I2C_sendRegisterPtr(i2cObj self, uint8_t deviceAddress, uint8_t registerPtr);
void I2C_waitForSlave(uint8_t deviceAddress);
uint8_t I2C_available(i2cObj self);
void I2C_waitForModule(i2cObj self);
int8_t I2C_sendByte(uint8_t databyte);
void I2C_sendAddress(i2cObj self, uint8_t RW_bit);
void I2C_sendStopBit(i2cObj self);
void I2C_sendACK(i2cObj self);
void I2C_sendNAK(i2cObj self);

#endif	/* I2C_H */

