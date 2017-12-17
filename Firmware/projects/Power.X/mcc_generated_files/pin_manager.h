/**
  System Interrupts Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the generated manager file for the MPLAB(c) Code Configurator device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for MPLAB(c) Code Configurator interrupts.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.26
        Device            :  PIC32MM0064GPM048
    The generated drivers are tested against the following:
        Compiler          :  XC32 1.42
        MPLAB             :  MPLAB X 3.60

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/

#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H
/**
    Section: Includes
*/
#include <xc.h>
/**
    Section: Device Pin Macros
*/
/**
  @Summary
    Sets the GPIO pin, RA0, high using LATAbits.LATA0.

  @Description
    Sets the GPIO pin, RA0, high using LATAbits.LATA0.

  @Preconditions
    The RA0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA0 high (1)
    CURRENT_1_SetHigh();
    </code>

*/
#define CURRENT_1_SetHigh()          ( LATASET = (1 << 0) )
/**
  @Summary
    Sets the GPIO pin, RA0, low using LATAbits.LATA0.

  @Description
    Sets the GPIO pin, RA0, low using LATAbits.LATA0.

  @Preconditions
    The RA0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA0 low (0)
    CURRENT_1_SetLow();
    </code>

*/
#define CURRENT_1_SetLow()           ( LATACLR = (1 << 0) )
/**
  @Summary
    Toggles the GPIO pin, RA0, using LATAbits.LATA0.

  @Description
    Toggles the GPIO pin, RA0, using LATAbits.LATA0.

  @Preconditions
    The RA0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA0
    CURRENT_1_Toggle();
    </code>

*/
#define CURRENT_1_Toggle()           ( LATAINV = (1 << 0) )
/**
  @Summary
    Reads the value of the GPIO pin, RA0.

  @Description
    Reads the value of the GPIO pin, RA0.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA0
    postValue = CURRENT_1_GetValue();
    </code>

*/
#define CURRENT_1_GetValue()         PORTAbits.RA0
/**
  @Summary
    Configures the GPIO pin, RA0, as an input.

  @Description
    Configures the GPIO pin, RA0, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA0 as an input
    CURRENT_1_SetDigitalInput();
    </code>

*/
#define CURRENT_1_SetDigitalInput()   ( TRISASET = (1 << 0) )
/**
  @Summary
    Configures the GPIO pin, RA0, as an output.

  @Description
    Configures the GPIO pin, RA0, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA0 as an output
    CURRENT_1_SetDigitalOutput();
    </code>

*/
#define CURRENT_1_SetDigitalOutput()   ( TRISACLR = (1 << 0) )
/**
  @Summary
    Sets the GPIO pin, RA1, high using LATAbits.LATA1.

  @Description
    Sets the GPIO pin, RA1, high using LATAbits.LATA1.

  @Preconditions
    The RA1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA1 high (1)
    CURRENT_2_SetHigh();
    </code>

*/
#define CURRENT_2_SetHigh()          ( LATASET = (1 << 1) )
/**
  @Summary
    Sets the GPIO pin, RA1, low using LATAbits.LATA1.

  @Description
    Sets the GPIO pin, RA1, low using LATAbits.LATA1.

  @Preconditions
    The RA1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA1 low (0)
    CURRENT_2_SetLow();
    </code>

*/
#define CURRENT_2_SetLow()           ( LATACLR = (1 << 1) )
/**
  @Summary
    Toggles the GPIO pin, RA1, using LATAbits.LATA1.

  @Description
    Toggles the GPIO pin, RA1, using LATAbits.LATA1.

  @Preconditions
    The RA1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA1
    CURRENT_2_Toggle();
    </code>

*/
#define CURRENT_2_Toggle()           ( LATAINV = (1 << 1) )
/**
  @Summary
    Reads the value of the GPIO pin, RA1.

  @Description
    Reads the value of the GPIO pin, RA1.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA1
    postValue = CURRENT_2_GetValue();
    </code>

*/
#define CURRENT_2_GetValue()         PORTAbits.RA1
/**
  @Summary
    Configures the GPIO pin, RA1, as an input.

  @Description
    Configures the GPIO pin, RA1, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA1 as an input
    CURRENT_2_SetDigitalInput();
    </code>

*/
#define CURRENT_2_SetDigitalInput()   ( TRISASET = (1 << 1) )
/**
  @Summary
    Configures the GPIO pin, RA1, as an output.

  @Description
    Configures the GPIO pin, RA1, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA1 as an output
    CURRENT_2_SetDigitalOutput();
    </code>

*/
#define CURRENT_2_SetDigitalOutput()   ( TRISACLR = (1 << 1) )
/**
  @Summary
    Sets the GPIO pin, RA10, high using LATAbits.LATA10.

  @Description
    Sets the GPIO pin, RA10, high using LATAbits.LATA10.

  @Preconditions
    The RA10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA10 high (1)
    RELAY_4_SetHigh();
    </code>

*/
#define RELAY_4_SetHigh()          ( LATASET = (1 << 10) )
/**
  @Summary
    Sets the GPIO pin, RA10, low using LATAbits.LATA10.

  @Description
    Sets the GPIO pin, RA10, low using LATAbits.LATA10.

  @Preconditions
    The RA10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA10 low (0)
    RELAY_4_SetLow();
    </code>

*/
#define RELAY_4_SetLow()           ( LATACLR = (1 << 10) )
/**
  @Summary
    Toggles the GPIO pin, RA10, using LATAbits.LATA10.

  @Description
    Toggles the GPIO pin, RA10, using LATAbits.LATA10.

  @Preconditions
    The RA10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA10
    RELAY_4_Toggle();
    </code>

*/
#define RELAY_4_Toggle()           ( LATAINV = (1 << 10) )
/**
  @Summary
    Reads the value of the GPIO pin, RA10.

  @Description
    Reads the value of the GPIO pin, RA10.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA10
    postValue = RELAY_4_GetValue();
    </code>

*/
#define RELAY_4_GetValue()         PORTAbits.RA10
/**
  @Summary
    Configures the GPIO pin, RA10, as an input.

  @Description
    Configures the GPIO pin, RA10, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA10 as an input
    RELAY_4_SetDigitalInput();
    </code>

*/
#define RELAY_4_SetDigitalInput()   ( TRISASET = (1 << 10) )
/**
  @Summary
    Configures the GPIO pin, RA10, as an output.

  @Description
    Configures the GPIO pin, RA10, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA10 as an output
    RELAY_4_SetDigitalOutput();
    </code>

*/
#define RELAY_4_SetDigitalOutput()   ( TRISACLR = (1 << 10) )
/**
  @Summary
    Sets the GPIO pin, RA15, high using LATAbits.LATA15.

  @Description
    Sets the GPIO pin, RA15, high using LATAbits.LATA15.

  @Preconditions
    The RA15 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA15 high (1)
    RELAY_8_SetHigh();
    </code>

*/
#define RELAY_8_SetHigh()          ( LATASET = (1 << 15) )
/**
  @Summary
    Sets the GPIO pin, RA15, low using LATAbits.LATA15.

  @Description
    Sets the GPIO pin, RA15, low using LATAbits.LATA15.

  @Preconditions
    The RA15 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA15 low (0)
    RELAY_8_SetLow();
    </code>

*/
#define RELAY_8_SetLow()           ( LATACLR = (1 << 15) )
/**
  @Summary
    Toggles the GPIO pin, RA15, using LATAbits.LATA15.

  @Description
    Toggles the GPIO pin, RA15, using LATAbits.LATA15.

  @Preconditions
    The RA15 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA15
    RELAY_8_Toggle();
    </code>

*/
#define RELAY_8_Toggle()           ( LATAINV = (1 << 15) )
/**
  @Summary
    Reads the value of the GPIO pin, RA15.

  @Description
    Reads the value of the GPIO pin, RA15.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA15
    postValue = RELAY_8_GetValue();
    </code>

*/
#define RELAY_8_GetValue()         PORTAbits.RA15
/**
  @Summary
    Configures the GPIO pin, RA15, as an input.

  @Description
    Configures the GPIO pin, RA15, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA15 as an input
    RELAY_8_SetDigitalInput();
    </code>

*/
#define RELAY_8_SetDigitalInput()   ( TRISASET = (1 << 15) )
/**
  @Summary
    Configures the GPIO pin, RA15, as an output.

  @Description
    Configures the GPIO pin, RA15, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA15 as an output
    RELAY_8_SetDigitalOutput();
    </code>

*/
#define RELAY_8_SetDigitalOutput()   ( TRISACLR = (1 << 15) )
/**
  @Summary
    Sets the GPIO pin, RA7, high using LATAbits.LATA7.

  @Description
    Sets the GPIO pin, RA7, high using LATAbits.LATA7.

  @Preconditions
    The RA7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA7 high (1)
    RELAY_3_SetHigh();
    </code>

*/
#define RELAY_3_SetHigh()          ( LATASET = (1 << 7) )
/**
  @Summary
    Sets the GPIO pin, RA7, low using LATAbits.LATA7.

  @Description
    Sets the GPIO pin, RA7, low using LATAbits.LATA7.

  @Preconditions
    The RA7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA7 low (0)
    RELAY_3_SetLow();
    </code>

*/
#define RELAY_3_SetLow()           ( LATACLR = (1 << 7) )
/**
  @Summary
    Toggles the GPIO pin, RA7, using LATAbits.LATA7.

  @Description
    Toggles the GPIO pin, RA7, using LATAbits.LATA7.

  @Preconditions
    The RA7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA7
    RELAY_3_Toggle();
    </code>

*/
#define RELAY_3_Toggle()           ( LATAINV = (1 << 7) )
/**
  @Summary
    Reads the value of the GPIO pin, RA7.

  @Description
    Reads the value of the GPIO pin, RA7.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA7
    postValue = RELAY_3_GetValue();
    </code>

*/
#define RELAY_3_GetValue()         PORTAbits.RA7
/**
  @Summary
    Configures the GPIO pin, RA7, as an input.

  @Description
    Configures the GPIO pin, RA7, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA7 as an input
    RELAY_3_SetDigitalInput();
    </code>

*/
#define RELAY_3_SetDigitalInput()   ( TRISASET = (1 << 7) )
/**
  @Summary
    Configures the GPIO pin, RA7, as an output.

  @Description
    Configures the GPIO pin, RA7, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA7 as an output
    RELAY_3_SetDigitalOutput();
    </code>

*/
#define RELAY_3_SetDigitalOutput()   ( TRISACLR = (1 << 7) )
/**
  @Summary
    Sets the GPIO pin, RB0, high using LATBbits.LATB0.

  @Description
    Sets the GPIO pin, RB0, high using LATBbits.LATB0.

  @Preconditions
    The RB0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB0 high (1)
    CURRENT_3_SetHigh();
    </code>

*/
#define CURRENT_3_SetHigh()          ( LATBSET = (1 << 0) )
/**
  @Summary
    Sets the GPIO pin, RB0, low using LATBbits.LATB0.

  @Description
    Sets the GPIO pin, RB0, low using LATBbits.LATB0.

  @Preconditions
    The RB0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB0 low (0)
    CURRENT_3_SetLow();
    </code>

*/
#define CURRENT_3_SetLow()           ( LATBCLR = (1 << 0) )
/**
  @Summary
    Toggles the GPIO pin, RB0, using LATBbits.LATB0.

  @Description
    Toggles the GPIO pin, RB0, using LATBbits.LATB0.

  @Preconditions
    The RB0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB0
    CURRENT_3_Toggle();
    </code>

*/
#define CURRENT_3_Toggle()           ( LATBINV = (1 << 0) )
/**
  @Summary
    Reads the value of the GPIO pin, RB0.

  @Description
    Reads the value of the GPIO pin, RB0.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB0
    postValue = CURRENT_3_GetValue();
    </code>

*/
#define CURRENT_3_GetValue()         PORTBbits.RB0
/**
  @Summary
    Configures the GPIO pin, RB0, as an input.

  @Description
    Configures the GPIO pin, RB0, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB0 as an input
    CURRENT_3_SetDigitalInput();
    </code>

*/
#define CURRENT_3_SetDigitalInput()   ( TRISBSET = (1 << 0) )
/**
  @Summary
    Configures the GPIO pin, RB0, as an output.

  @Description
    Configures the GPIO pin, RB0, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB0 as an output
    CURRENT_3_SetDigitalOutput();
    </code>

*/
#define CURRENT_3_SetDigitalOutput()   ( TRISBCLR = (1 << 0) )
/**
  @Summary
    Sets the GPIO pin, RB1, high using LATBbits.LATB1.

  @Description
    Sets the GPIO pin, RB1, high using LATBbits.LATB1.

  @Preconditions
    The RB1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB1 high (1)
    CURRENT_4_SetHigh();
    </code>

*/
#define CURRENT_4_SetHigh()          ( LATBSET = (1 << 1) )
/**
  @Summary
    Sets the GPIO pin, RB1, low using LATBbits.LATB1.

  @Description
    Sets the GPIO pin, RB1, low using LATBbits.LATB1.

  @Preconditions
    The RB1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB1 low (0)
    CURRENT_4_SetLow();
    </code>

*/
#define CURRENT_4_SetLow()           ( LATBCLR = (1 << 1) )
/**
  @Summary
    Toggles the GPIO pin, RB1, using LATBbits.LATB1.

  @Description
    Toggles the GPIO pin, RB1, using LATBbits.LATB1.

  @Preconditions
    The RB1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB1
    CURRENT_4_Toggle();
    </code>

*/
#define CURRENT_4_Toggle()           ( LATBINV = (1 << 1) )
/**
  @Summary
    Reads the value of the GPIO pin, RB1.

  @Description
    Reads the value of the GPIO pin, RB1.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB1
    postValue = CURRENT_4_GetValue();
    </code>

*/
#define CURRENT_4_GetValue()         PORTBbits.RB1
/**
  @Summary
    Configures the GPIO pin, RB1, as an input.

  @Description
    Configures the GPIO pin, RB1, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB1 as an input
    CURRENT_4_SetDigitalInput();
    </code>

*/
#define CURRENT_4_SetDigitalInput()   ( TRISBSET = (1 << 1) )
/**
  @Summary
    Configures the GPIO pin, RB1, as an output.

  @Description
    Configures the GPIO pin, RB1, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB1 as an output
    CURRENT_4_SetDigitalOutput();
    </code>

*/
#define CURRENT_4_SetDigitalOutput()   ( TRISBCLR = (1 << 1) )
/**
  @Summary
    Sets the GPIO pin, RB10, high using LATBbits.LATB10.

  @Description
    Sets the GPIO pin, RB10, high using LATBbits.LATB10.

  @Preconditions
    The RB10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB10 high (1)
    RELAY_7_SetHigh();
    </code>

*/
#define RELAY_7_SetHigh()          ( LATBSET = (1 << 10) )
/**
  @Summary
    Sets the GPIO pin, RB10, low using LATBbits.LATB10.

  @Description
    Sets the GPIO pin, RB10, low using LATBbits.LATB10.

  @Preconditions
    The RB10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB10 low (0)
    RELAY_7_SetLow();
    </code>

*/
#define RELAY_7_SetLow()           ( LATBCLR = (1 << 10) )
/**
  @Summary
    Toggles the GPIO pin, RB10, using LATBbits.LATB10.

  @Description
    Toggles the GPIO pin, RB10, using LATBbits.LATB10.

  @Preconditions
    The RB10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB10
    RELAY_7_Toggle();
    </code>

*/
#define RELAY_7_Toggle()           ( LATBINV = (1 << 10) )
/**
  @Summary
    Reads the value of the GPIO pin, RB10.

  @Description
    Reads the value of the GPIO pin, RB10.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB10
    postValue = RELAY_7_GetValue();
    </code>

*/
#define RELAY_7_GetValue()         PORTBbits.RB10
/**
  @Summary
    Configures the GPIO pin, RB10, as an input.

  @Description
    Configures the GPIO pin, RB10, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB10 as an input
    RELAY_7_SetDigitalInput();
    </code>

*/
#define RELAY_7_SetDigitalInput()   ( TRISBSET = (1 << 10) )
/**
  @Summary
    Configures the GPIO pin, RB10, as an output.

  @Description
    Configures the GPIO pin, RB10, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB10 as an output
    RELAY_7_SetDigitalOutput();
    </code>

*/
#define RELAY_7_SetDigitalOutput()   ( TRISBCLR = (1 << 10) )
/**
  @Summary
    Sets the GPIO pin, RB11, high using LATBbits.LATB11.

  @Description
    Sets the GPIO pin, RB11, high using LATBbits.LATB11.

  @Preconditions
    The RB11 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB11 high (1)
    RELAY_6_SetHigh();
    </code>

*/
#define RELAY_6_SetHigh()          ( LATBSET = (1 << 11) )
/**
  @Summary
    Sets the GPIO pin, RB11, low using LATBbits.LATB11.

  @Description
    Sets the GPIO pin, RB11, low using LATBbits.LATB11.

  @Preconditions
    The RB11 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB11 low (0)
    RELAY_6_SetLow();
    </code>

*/
#define RELAY_6_SetLow()           ( LATBCLR = (1 << 11) )
/**
  @Summary
    Toggles the GPIO pin, RB11, using LATBbits.LATB11.

  @Description
    Toggles the GPIO pin, RB11, using LATBbits.LATB11.

  @Preconditions
    The RB11 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB11
    RELAY_6_Toggle();
    </code>

*/
#define RELAY_6_Toggle()           ( LATBINV = (1 << 11) )
/**
  @Summary
    Reads the value of the GPIO pin, RB11.

  @Description
    Reads the value of the GPIO pin, RB11.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB11
    postValue = RELAY_6_GetValue();
    </code>

*/
#define RELAY_6_GetValue()         PORTBbits.RB11
/**
  @Summary
    Configures the GPIO pin, RB11, as an input.

  @Description
    Configures the GPIO pin, RB11, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB11 as an input
    RELAY_6_SetDigitalInput();
    </code>

*/
#define RELAY_6_SetDigitalInput()   ( TRISBSET = (1 << 11) )
/**
  @Summary
    Configures the GPIO pin, RB11, as an output.

  @Description
    Configures the GPIO pin, RB11, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB11 as an output
    RELAY_6_SetDigitalOutput();
    </code>

*/
#define RELAY_6_SetDigitalOutput()   ( TRISBCLR = (1 << 11) )
/**
  @Summary
    Sets the GPIO pin, RB13, high using LATBbits.LATB13.

  @Description
    Sets the GPIO pin, RB13, high using LATBbits.LATB13.

  @Preconditions
    The RB13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB13 high (1)
    RELAY_5_SetHigh();
    </code>

*/
#define RELAY_5_SetHigh()          ( LATBSET = (1 << 13) )
/**
  @Summary
    Sets the GPIO pin, RB13, low using LATBbits.LATB13.

  @Description
    Sets the GPIO pin, RB13, low using LATBbits.LATB13.

  @Preconditions
    The RB13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB13 low (0)
    RELAY_5_SetLow();
    </code>

*/
#define RELAY_5_SetLow()           ( LATBCLR = (1 << 13) )
/**
  @Summary
    Toggles the GPIO pin, RB13, using LATBbits.LATB13.

  @Description
    Toggles the GPIO pin, RB13, using LATBbits.LATB13.

  @Preconditions
    The RB13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB13
    RELAY_5_Toggle();
    </code>

*/
#define RELAY_5_Toggle()           ( LATBINV = (1 << 13) )
/**
  @Summary
    Reads the value of the GPIO pin, RB13.

  @Description
    Reads the value of the GPIO pin, RB13.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB13
    postValue = RELAY_5_GetValue();
    </code>

*/
#define RELAY_5_GetValue()         PORTBbits.RB13
/**
  @Summary
    Configures the GPIO pin, RB13, as an input.

  @Description
    Configures the GPIO pin, RB13, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB13 as an input
    RELAY_5_SetDigitalInput();
    </code>

*/
#define RELAY_5_SetDigitalInput()   ( TRISBSET = (1 << 13) )
/**
  @Summary
    Configures the GPIO pin, RB13, as an output.

  @Description
    Configures the GPIO pin, RB13, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB13 as an output
    RELAY_5_SetDigitalOutput();
    </code>

*/
#define RELAY_5_SetDigitalOutput()   ( TRISBCLR = (1 << 13) )
/**
  @Summary
    Sets the GPIO pin, RB14, high using LATBbits.LATB14.

  @Description
    Sets the GPIO pin, RB14, high using LATBbits.LATB14.

  @Preconditions
    The RB14 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB14 high (1)
    RELAY_2_SetHigh();
    </code>

*/
#define RELAY_2_SetHigh()          ( LATBSET = (1 << 14) )
/**
  @Summary
    Sets the GPIO pin, RB14, low using LATBbits.LATB14.

  @Description
    Sets the GPIO pin, RB14, low using LATBbits.LATB14.

  @Preconditions
    The RB14 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB14 low (0)
    RELAY_2_SetLow();
    </code>

*/
#define RELAY_2_SetLow()           ( LATBCLR = (1 << 14) )
/**
  @Summary
    Toggles the GPIO pin, RB14, using LATBbits.LATB14.

  @Description
    Toggles the GPIO pin, RB14, using LATBbits.LATB14.

  @Preconditions
    The RB14 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB14
    RELAY_2_Toggle();
    </code>

*/
#define RELAY_2_Toggle()           ( LATBINV = (1 << 14) )
/**
  @Summary
    Reads the value of the GPIO pin, RB14.

  @Description
    Reads the value of the GPIO pin, RB14.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB14
    postValue = RELAY_2_GetValue();
    </code>

*/
#define RELAY_2_GetValue()         PORTBbits.RB14
/**
  @Summary
    Configures the GPIO pin, RB14, as an input.

  @Description
    Configures the GPIO pin, RB14, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB14 as an input
    RELAY_2_SetDigitalInput();
    </code>

*/
#define RELAY_2_SetDigitalInput()   ( TRISBSET = (1 << 14) )
/**
  @Summary
    Configures the GPIO pin, RB14, as an output.

  @Description
    Configures the GPIO pin, RB14, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB14 as an output
    RELAY_2_SetDigitalOutput();
    </code>

*/
#define RELAY_2_SetDigitalOutput()   ( TRISBCLR = (1 << 14) )
/**
  @Summary
    Sets the GPIO pin, RB15, high using LATBbits.LATB15.

  @Description
    Sets the GPIO pin, RB15, high using LATBbits.LATB15.

  @Preconditions
    The RB15 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB15 high (1)
    RELAY_1_SetHigh();
    </code>

*/
#define RELAY_1_SetHigh()          ( LATBSET = (1 << 15) )
/**
  @Summary
    Sets the GPIO pin, RB15, low using LATBbits.LATB15.

  @Description
    Sets the GPIO pin, RB15, low using LATBbits.LATB15.

  @Preconditions
    The RB15 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB15 low (0)
    RELAY_1_SetLow();
    </code>

*/
#define RELAY_1_SetLow()           ( LATBCLR = (1 << 15) )
/**
  @Summary
    Toggles the GPIO pin, RB15, using LATBbits.LATB15.

  @Description
    Toggles the GPIO pin, RB15, using LATBbits.LATB15.

  @Preconditions
    The RB15 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB15
    RELAY_1_Toggle();
    </code>

*/
#define RELAY_1_Toggle()           ( LATBINV = (1 << 15) )
/**
  @Summary
    Reads the value of the GPIO pin, RB15.

  @Description
    Reads the value of the GPIO pin, RB15.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB15
    postValue = RELAY_1_GetValue();
    </code>

*/
#define RELAY_1_GetValue()         PORTBbits.RB15
/**
  @Summary
    Configures the GPIO pin, RB15, as an input.

  @Description
    Configures the GPIO pin, RB15, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB15 as an input
    RELAY_1_SetDigitalInput();
    </code>

*/
#define RELAY_1_SetDigitalInput()   ( TRISBSET = (1 << 15) )
/**
  @Summary
    Configures the GPIO pin, RB15, as an output.

  @Description
    Configures the GPIO pin, RB15, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB15 as an output
    RELAY_1_SetDigitalOutput();
    </code>

*/
#define RELAY_1_SetDigitalOutput()   ( TRISBCLR = (1 << 15) )
/**
  @Summary
    Sets the GPIO pin, RB2, high using LATBbits.LATB2.

  @Description
    Sets the GPIO pin, RB2, high using LATBbits.LATB2.

  @Preconditions
    The RB2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB2 high (1)
    CURRENT_5_SetHigh();
    </code>

*/
#define CURRENT_5_SetHigh()          ( LATBSET = (1 << 2) )
/**
  @Summary
    Sets the GPIO pin, RB2, low using LATBbits.LATB2.

  @Description
    Sets the GPIO pin, RB2, low using LATBbits.LATB2.

  @Preconditions
    The RB2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB2 low (0)
    CURRENT_5_SetLow();
    </code>

*/
#define CURRENT_5_SetLow()           ( LATBCLR = (1 << 2) )
/**
  @Summary
    Toggles the GPIO pin, RB2, using LATBbits.LATB2.

  @Description
    Toggles the GPIO pin, RB2, using LATBbits.LATB2.

  @Preconditions
    The RB2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB2
    CURRENT_5_Toggle();
    </code>

*/
#define CURRENT_5_Toggle()           ( LATBINV = (1 << 2) )
/**
  @Summary
    Reads the value of the GPIO pin, RB2.

  @Description
    Reads the value of the GPIO pin, RB2.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB2
    postValue = CURRENT_5_GetValue();
    </code>

*/
#define CURRENT_5_GetValue()         PORTBbits.RB2
/**
  @Summary
    Configures the GPIO pin, RB2, as an input.

  @Description
    Configures the GPIO pin, RB2, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB2 as an input
    CURRENT_5_SetDigitalInput();
    </code>

*/
#define CURRENT_5_SetDigitalInput()   ( TRISBSET = (1 << 2) )
/**
  @Summary
    Configures the GPIO pin, RB2, as an output.

  @Description
    Configures the GPIO pin, RB2, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB2 as an output
    CURRENT_5_SetDigitalOutput();
    </code>

*/
#define CURRENT_5_SetDigitalOutput()   ( TRISBCLR = (1 << 2) )
/**
  @Summary
    Sets the GPIO pin, RB3, high using LATBbits.LATB3.

  @Description
    Sets the GPIO pin, RB3, high using LATBbits.LATB3.

  @Preconditions
    The RB3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB3 high (1)
    CURRENT_6_SetHigh();
    </code>

*/
#define CURRENT_6_SetHigh()          ( LATBSET = (1 << 3) )
/**
  @Summary
    Sets the GPIO pin, RB3, low using LATBbits.LATB3.

  @Description
    Sets the GPIO pin, RB3, low using LATBbits.LATB3.

  @Preconditions
    The RB3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB3 low (0)
    CURRENT_6_SetLow();
    </code>

*/
#define CURRENT_6_SetLow()           ( LATBCLR = (1 << 3) )
/**
  @Summary
    Toggles the GPIO pin, RB3, using LATBbits.LATB3.

  @Description
    Toggles the GPIO pin, RB3, using LATBbits.LATB3.

  @Preconditions
    The RB3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB3
    CURRENT_6_Toggle();
    </code>

*/
#define CURRENT_6_Toggle()           ( LATBINV = (1 << 3) )
/**
  @Summary
    Reads the value of the GPIO pin, RB3.

  @Description
    Reads the value of the GPIO pin, RB3.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB3
    postValue = CURRENT_6_GetValue();
    </code>

*/
#define CURRENT_6_GetValue()         PORTBbits.RB3
/**
  @Summary
    Configures the GPIO pin, RB3, as an input.

  @Description
    Configures the GPIO pin, RB3, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB3 as an input
    CURRENT_6_SetDigitalInput();
    </code>

*/
#define CURRENT_6_SetDigitalInput()   ( TRISBSET = (1 << 3) )
/**
  @Summary
    Configures the GPIO pin, RB3, as an output.

  @Description
    Configures the GPIO pin, RB3, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB3 as an output
    CURRENT_6_SetDigitalOutput();
    </code>

*/
#define CURRENT_6_SetDigitalOutput()   ( TRISBCLR = (1 << 3) )
/**
  @Summary
    Sets the GPIO pin, RC0, high using LATCbits.LATC0.

  @Description
    Sets the GPIO pin, RC0, high using LATCbits.LATC0.

  @Preconditions
    The RC0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC0 high (1)
    CURRENT_7_SetHigh();
    </code>

*/
#define CURRENT_7_SetHigh()          ( LATCSET = (1 << 0) )
/**
  @Summary
    Sets the GPIO pin, RC0, low using LATCbits.LATC0.

  @Description
    Sets the GPIO pin, RC0, low using LATCbits.LATC0.

  @Preconditions
    The RC0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC0 low (0)
    CURRENT_7_SetLow();
    </code>

*/
#define CURRENT_7_SetLow()           ( LATCCLR = (1 << 0) )
/**
  @Summary
    Toggles the GPIO pin, RC0, using LATCbits.LATC0.

  @Description
    Toggles the GPIO pin, RC0, using LATCbits.LATC0.

  @Preconditions
    The RC0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC0
    CURRENT_7_Toggle();
    </code>

*/
#define CURRENT_7_Toggle()           ( LATCINV = (1 << 0) )
/**
  @Summary
    Reads the value of the GPIO pin, RC0.

  @Description
    Reads the value of the GPIO pin, RC0.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC0
    postValue = CURRENT_7_GetValue();
    </code>

*/
#define CURRENT_7_GetValue()         PORTCbits.RC0
/**
  @Summary
    Configures the GPIO pin, RC0, as an input.

  @Description
    Configures the GPIO pin, RC0, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC0 as an input
    CURRENT_7_SetDigitalInput();
    </code>

*/
#define CURRENT_7_SetDigitalInput()   ( TRISCSET = (1 << 0) )
/**
  @Summary
    Configures the GPIO pin, RC0, as an output.

  @Description
    Configures the GPIO pin, RC0, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC0 as an output
    CURRENT_7_SetDigitalOutput();
    </code>

*/
#define CURRENT_7_SetDigitalOutput()   ( TRISCCLR = (1 << 0) )
/**
  @Summary
    Sets the GPIO pin, RC1, high using LATCbits.LATC1.

  @Description
    Sets the GPIO pin, RC1, high using LATCbits.LATC1.

  @Preconditions
    The RC1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC1 high (1)
    CURRENT_8_SetHigh();
    </code>

*/
#define CURRENT_8_SetHigh()          ( LATCSET = (1 << 1) )
/**
  @Summary
    Sets the GPIO pin, RC1, low using LATCbits.LATC1.

  @Description
    Sets the GPIO pin, RC1, low using LATCbits.LATC1.

  @Preconditions
    The RC1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC1 low (0)
    CURRENT_8_SetLow();
    </code>

*/
#define CURRENT_8_SetLow()           ( LATCCLR = (1 << 1) )
/**
  @Summary
    Toggles the GPIO pin, RC1, using LATCbits.LATC1.

  @Description
    Toggles the GPIO pin, RC1, using LATCbits.LATC1.

  @Preconditions
    The RC1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC1
    CURRENT_8_Toggle();
    </code>

*/
#define CURRENT_8_Toggle()           ( LATCINV = (1 << 1) )
/**
  @Summary
    Reads the value of the GPIO pin, RC1.

  @Description
    Reads the value of the GPIO pin, RC1.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC1
    postValue = CURRENT_8_GetValue();
    </code>

*/
#define CURRENT_8_GetValue()         PORTCbits.RC1
/**
  @Summary
    Configures the GPIO pin, RC1, as an input.

  @Description
    Configures the GPIO pin, RC1, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC1 as an input
    CURRENT_8_SetDigitalInput();
    </code>

*/
#define CURRENT_8_SetDigitalInput()   ( TRISCSET = (1 << 1) )
/**
  @Summary
    Configures the GPIO pin, RC1, as an output.

  @Description
    Configures the GPIO pin, RC1, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC1 as an output
    CURRENT_8_SetDigitalOutput();
    </code>

*/
#define CURRENT_8_SetDigitalOutput()   ( TRISCCLR = (1 << 1) )
/**
  @Summary
    Sets the GPIO pin, RC3, high using LATCbits.LATC3.

  @Description
    Sets the GPIO pin, RC3, high using LATCbits.LATC3.

  @Preconditions
    The RC3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC3 high (1)
    N_R_LED_SetHigh();
    </code>

*/
#define N_R_LED_SetHigh()          ( LATCSET = (1 << 3) )
/**
  @Summary
    Sets the GPIO pin, RC3, low using LATCbits.LATC3.

  @Description
    Sets the GPIO pin, RC3, low using LATCbits.LATC3.

  @Preconditions
    The RC3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC3 low (0)
    N_R_LED_SetLow();
    </code>

*/
#define N_R_LED_SetLow()           ( LATCCLR = (1 << 3) )
/**
  @Summary
    Toggles the GPIO pin, RC3, using LATCbits.LATC3.

  @Description
    Toggles the GPIO pin, RC3, using LATCbits.LATC3.

  @Preconditions
    The RC3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC3
    N_R_LED_Toggle();
    </code>

*/
#define N_R_LED_Toggle()           ( LATCINV = (1 << 3) )
/**
  @Summary
    Reads the value of the GPIO pin, RC3.

  @Description
    Reads the value of the GPIO pin, RC3.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC3
    postValue = N_R_LED_GetValue();
    </code>

*/
#define N_R_LED_GetValue()         PORTCbits.RC3
/**
  @Summary
    Configures the GPIO pin, RC3, as an input.

  @Description
    Configures the GPIO pin, RC3, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC3 as an input
    N_R_LED_SetDigitalInput();
    </code>

*/
#define N_R_LED_SetDigitalInput()   ( TRISCSET = (1 << 3) )
/**
  @Summary
    Configures the GPIO pin, RC3, as an output.

  @Description
    Configures the GPIO pin, RC3, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC3 as an output
    N_R_LED_SetDigitalOutput();
    </code>

*/
#define N_R_LED_SetDigitalOutput()   ( TRISCCLR = (1 << 3) )
/**
  @Summary
    Sets the GPIO pin, RC4, high using LATCbits.LATC4.

  @Description
    Sets the GPIO pin, RC4, high using LATCbits.LATC4.

  @Preconditions
    The RC4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC4 high (1)
    N_G_LED_SetHigh();
    </code>

*/
#define N_G_LED_SetHigh()          ( LATCSET = (1 << 4) )
/**
  @Summary
    Sets the GPIO pin, RC4, low using LATCbits.LATC4.

  @Description
    Sets the GPIO pin, RC4, low using LATCbits.LATC4.

  @Preconditions
    The RC4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC4 low (0)
    N_G_LED_SetLow();
    </code>

*/
#define N_G_LED_SetLow()           ( LATCCLR = (1 << 4) )
/**
  @Summary
    Toggles the GPIO pin, RC4, using LATCbits.LATC4.

  @Description
    Toggles the GPIO pin, RC4, using LATCbits.LATC4.

  @Preconditions
    The RC4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC4
    N_G_LED_Toggle();
    </code>

*/
#define N_G_LED_Toggle()           ( LATCINV = (1 << 4) )
/**
  @Summary
    Reads the value of the GPIO pin, RC4.

  @Description
    Reads the value of the GPIO pin, RC4.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC4
    postValue = N_G_LED_GetValue();
    </code>

*/
#define N_G_LED_GetValue()         PORTCbits.RC4
/**
  @Summary
    Configures the GPIO pin, RC4, as an input.

  @Description
    Configures the GPIO pin, RC4, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC4 as an input
    N_G_LED_SetDigitalInput();
    </code>

*/
#define N_G_LED_SetDigitalInput()   ( TRISCSET = (1 << 4) )
/**
  @Summary
    Configures the GPIO pin, RC4, as an output.

  @Description
    Configures the GPIO pin, RC4, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC4 as an output
    N_G_LED_SetDigitalOutput();
    </code>

*/
#define N_G_LED_SetDigitalOutput()   ( TRISCCLR = (1 << 4) )
/**
  @Summary
    Sets the GPIO pin, RC5, high using LATCbits.LATC5.

  @Description
    Sets the GPIO pin, RC5, high using LATCbits.LATC5.

  @Preconditions
    The RC5 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC5 high (1)
    N_Y_LED_SetHigh();
    </code>

*/
#define N_Y_LED_SetHigh()          ( LATCSET = (1 << 5) )
/**
  @Summary
    Sets the GPIO pin, RC5, low using LATCbits.LATC5.

  @Description
    Sets the GPIO pin, RC5, low using LATCbits.LATC5.

  @Preconditions
    The RC5 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC5 low (0)
    N_Y_LED_SetLow();
    </code>

*/
#define N_Y_LED_SetLow()           ( LATCCLR = (1 << 5) )
/**
  @Summary
    Toggles the GPIO pin, RC5, using LATCbits.LATC5.

  @Description
    Toggles the GPIO pin, RC5, using LATCbits.LATC5.

  @Preconditions
    The RC5 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC5
    N_Y_LED_Toggle();
    </code>

*/
#define N_Y_LED_Toggle()           ( LATCINV = (1 << 5) )
/**
  @Summary
    Reads the value of the GPIO pin, RC5.

  @Description
    Reads the value of the GPIO pin, RC5.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC5
    postValue = N_Y_LED_GetValue();
    </code>

*/
#define N_Y_LED_GetValue()         PORTCbits.RC5
/**
  @Summary
    Configures the GPIO pin, RC5, as an input.

  @Description
    Configures the GPIO pin, RC5, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC5 as an input
    N_Y_LED_SetDigitalInput();
    </code>

*/
#define N_Y_LED_SetDigitalInput()   ( TRISCSET = (1 << 5) )
/**
  @Summary
    Configures the GPIO pin, RC5, as an output.

  @Description
    Configures the GPIO pin, RC5, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC5 as an output
    N_Y_LED_SetDigitalOutput();
    </code>

*/
#define N_Y_LED_SetDigitalOutput()   ( TRISCCLR = (1 << 5) )
/**
  @Summary
    Sets the GPIO pin, RC6, high using LATCbits.LATC6.

  @Description
    Sets the GPIO pin, RC6, high using LATCbits.LATC6.

  @Preconditions
    The RC6 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC6 high (1)
    AC_AVAIL_SetHigh();
    </code>

*/
#define AC_AVAIL_SetHigh()          ( LATCSET = (1 << 6) )
/**
  @Summary
    Sets the GPIO pin, RC6, low using LATCbits.LATC6.

  @Description
    Sets the GPIO pin, RC6, low using LATCbits.LATC6.

  @Preconditions
    The RC6 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC6 low (0)
    AC_AVAIL_SetLow();
    </code>

*/
#define AC_AVAIL_SetLow()           ( LATCCLR = (1 << 6) )
/**
  @Summary
    Toggles the GPIO pin, RC6, using LATCbits.LATC6.

  @Description
    Toggles the GPIO pin, RC6, using LATCbits.LATC6.

  @Preconditions
    The RC6 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC6
    AC_AVAIL_Toggle();
    </code>

*/
#define AC_AVAIL_Toggle()           ( LATCINV = (1 << 6) )
/**
  @Summary
    Reads the value of the GPIO pin, RC6.

  @Description
    Reads the value of the GPIO pin, RC6.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC6
    postValue = AC_AVAIL_GetValue();
    </code>

*/
#define AC_AVAIL_GetValue()         PORTCbits.RC6
/**
  @Summary
    Configures the GPIO pin, RC6, as an input.

  @Description
    Configures the GPIO pin, RC6, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC6 as an input
    AC_AVAIL_SetDigitalInput();
    </code>

*/
#define AC_AVAIL_SetDigitalInput()   ( TRISCSET = (1 << 6) )
/**
  @Summary
    Configures the GPIO pin, RC6, as an output.

  @Description
    Configures the GPIO pin, RC6, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC6 as an output
    AC_AVAIL_SetDigitalOutput();
    </code>

*/
#define AC_AVAIL_SetDigitalOutput()   ( TRISCCLR = (1 << 6) )
/**
  @Summary
    Sets the GPIO pin, RC7, high using LATCbits.LATC7.

  @Description
    Sets the GPIO pin, RC7, high using LATCbits.LATC7.

  @Preconditions
    The RC7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC7 high (1)
    ZERO_CROSS_SetHigh();
    </code>

*/
#define ZERO_CROSS_SetHigh()          ( LATCSET = (1 << 7) )
/**
  @Summary
    Sets the GPIO pin, RC7, low using LATCbits.LATC7.

  @Description
    Sets the GPIO pin, RC7, low using LATCbits.LATC7.

  @Preconditions
    The RC7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC7 low (0)
    ZERO_CROSS_SetLow();
    </code>

*/
#define ZERO_CROSS_SetLow()           ( LATCCLR = (1 << 7) )
/**
  @Summary
    Toggles the GPIO pin, RC7, using LATCbits.LATC7.

  @Description
    Toggles the GPIO pin, RC7, using LATCbits.LATC7.

  @Preconditions
    The RC7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC7
    ZERO_CROSS_Toggle();
    </code>

*/
#define ZERO_CROSS_Toggle()           ( LATCINV = (1 << 7) )
/**
  @Summary
    Reads the value of the GPIO pin, RC7.

  @Description
    Reads the value of the GPIO pin, RC7.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC7
    postValue = ZERO_CROSS_GetValue();
    </code>

*/
#define ZERO_CROSS_GetValue()         PORTCbits.RC7
/**
  @Summary
    Configures the GPIO pin, RC7, as an input.

  @Description
    Configures the GPIO pin, RC7, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC7 as an input
    ZERO_CROSS_SetDigitalInput();
    </code>

*/
#define ZERO_CROSS_SetDigitalInput()   ( TRISCSET = (1 << 7) )
/**
  @Summary
    Configures the GPIO pin, RC7, as an output.

  @Description
    Configures the GPIO pin, RC7, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC7 as an output
    ZERO_CROSS_SetDigitalOutput();
    </code>

*/
#define ZERO_CROSS_SetDigitalOutput()   ( TRISCCLR = (1 << 7) )

/**
    Section: Function Prototypes
*/
/**
  @Summary
    Configures the pin settings of the PIC32MM0064GPM048
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description
    This is the generated manager file for the MPLAB(c) Code Configurator device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        PIN_MANAGER_Initialize();
    }
    </code>

*/
void PIN_MANAGER_Initialize(void);

#endif
