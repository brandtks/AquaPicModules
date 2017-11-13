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
        Device            :  PIC32MM0064GPM028
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
    IO_RB0_SetHigh();
    </code>

*/
#define IO_RB0_SetHigh()          ( LATBSET = (1 << 0) )
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
    IO_RB0_SetLow();
    </code>

*/
#define IO_RB0_SetLow()           ( LATBCLR = (1 << 0) )
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
    IO_RB0_Toggle();
    </code>

*/
#define IO_RB0_Toggle()           ( LATBINV = (1 << 0) )
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
    postValue = IO_RB0_GetValue();
    </code>

*/
#define IO_RB0_GetValue()         PORTBbits.RB0
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
    IO_RB0_SetDigitalInput();
    </code>

*/
#define IO_RB0_SetDigitalInput()   ( TRISBSET = (1 << 0) )
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
    IO_RB0_SetDigitalOutput();
    </code>

*/
#define IO_RB0_SetDigitalOutput()   ( TRISBCLR = (1 << 0) )
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
    IO_RB1_SetHigh();
    </code>

*/
#define IO_RB1_SetHigh()          ( LATBSET = (1 << 1) )
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
    IO_RB1_SetLow();
    </code>

*/
#define IO_RB1_SetLow()           ( LATBCLR = (1 << 1) )
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
    IO_RB1_Toggle();
    </code>

*/
#define IO_RB1_Toggle()           ( LATBINV = (1 << 1) )
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
    postValue = IO_RB1_GetValue();
    </code>

*/
#define IO_RB1_GetValue()         PORTBbits.RB1
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
    IO_RB1_SetDigitalInput();
    </code>

*/
#define IO_RB1_SetDigitalInput()   ( TRISBSET = (1 << 1) )
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
    IO_RB1_SetDigitalOutput();
    </code>

*/
#define IO_RB1_SetDigitalOutput()   ( TRISBCLR = (1 << 1) )
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
    IO_RB2_SetHigh();
    </code>

*/
#define IO_RB2_SetHigh()          ( LATBSET = (1 << 2) )
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
    IO_RB2_SetLow();
    </code>

*/
#define IO_RB2_SetLow()           ( LATBCLR = (1 << 2) )
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
    IO_RB2_Toggle();
    </code>

*/
#define IO_RB2_Toggle()           ( LATBINV = (1 << 2) )
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
    postValue = IO_RB2_GetValue();
    </code>

*/
#define IO_RB2_GetValue()         PORTBbits.RB2
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
    IO_RB2_SetDigitalInput();
    </code>

*/
#define IO_RB2_SetDigitalInput()   ( TRISBSET = (1 << 2) )
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
    IO_RB2_SetDigitalOutput();
    </code>

*/
#define IO_RB2_SetDigitalOutput()   ( TRISBCLR = (1 << 2) )

/**
    Section: Function Prototypes
*/
/**
  @Summary
    Configures the pin settings of the PIC32MM0064GPM028
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
