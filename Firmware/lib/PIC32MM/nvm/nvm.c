/*******************************************************************************
 * nvm.c
 *
 * Goodtime Development 
 * 
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  Non-volatile memory emulation in flash memory
 * 
 * Device:
 *  PIC32MM
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

#include <stdint.h>
#include <xc.h>
#include "nvm.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define MEMORY_SIZE     sizeof(uint64_t) * NUMBER_64_BIT_MEMORY_LOCATIONS
#define NVM_TIMEOUT     16000

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/

/******************************************************************************/
/* Variables                                                                  */
/******************************************************************************/
/* Place memory at the end of flash */
uint64_t __attribute__((aligned(MEMORY_SIZE),
                        address(__KSEG0_PROGRAM_MEM_BASE + __KSEG0_PROGRAM_MEM_LENGTH - MEMORY_SIZE),
                        persistent)) nvm[NUMBER_64_BIT_MEMORY_LOCATIONS];

uint64_t nvm_read(uint8_t index) {
    if (index < 0)
    if (index >= NUMBER_64_BIT_MEMORY_LOCATIONS) {
        return 0;
    }
    return nvm[index];
}

uint32_t nvm_write(uint8_t index, uint64_t value) {
    uint32_t status, timeout = NVM_TIMEOUT;
    
    if (index < 0)
    if (index >= NUMBER_64_BIT_MEMORY_LOCATIONS) {
        return 1;
    }
    
    /* Lower 32 bits */
    NVMDATA0 = value & 0x00000000FFFFFFFF;
    /* Upper 32 bits */
    NVMDATA1 = value >> 32;
    /* Convert from virtual address to physical address */
    NVMADDR = (unsigned int)&(nvm[index]) & 0x1FFFFFFF;
    
    /* Suspend or Disable all Interrupts */
    asm volatile ("di %0" : "=r" (status));
    
    /* Enable Flash Write/Erase Operations and select double word write */
    NVMCON = 0x00004002;
    
    /* Unlock flash memory */
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;
    
    /* Start the operation using the Set Register */
    NVMCONSET = 0x00008000;
    
    /* Wait for operation to complete*/
    while (NVMCON & 0x00008000) {
        timeout--;
        if(timeout == 0) {        
            break;
        }
    }
    
    /* Disable Flash Write/Erase Operations */
    NVMCONCLR = 0x00004000;
    
    /* Restore Interrupts */
    if (status & 0x00000001) {
        asm volatile ("ei");
    } else {
        asm volatile ("di");
    }
    
    /* Return WRERR and LVDERR Error Status Bits and if timeout occurred */
    return (NVMCON & 0x3000) | (timeout == 0);
}