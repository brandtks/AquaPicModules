/*************************************************************************
 *  © 2016 Microchip Technology Inc.
 *
 *  Project Name:   Data EEPROM Emulation
 *  FileName:       dee.c
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *  Description: This C source file contains functions implementations
 *               to emulate EEPROM.
 *
 *************************************************************************/
/**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and
 * any derivatives created by any person or entity by or on your behalf,
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your reference in accompanying documentation. Microchip and its licensors
 * retain all ownership and intellectual property rights in the
 * accompanying software and in all derivatives hereto.
 *
 * This software and any accompanying information is for suggestion only.
 * It does not modify Microchip's standard warranty for its products. You
 * agree that you are solely responsible for testing the software and
 * determining its suitability. Microchip has no obligation to modify,
 * test, certify, or support the software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY
 * APPLICATION.
 *
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY,
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT,
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE,
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE,
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW,
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
 * THESE TERMS.
 *************************************************************************
 *
 * written by Anton Alkhimenok 06/01/2016
 *
 *************************************************************************/
 
#include "dee.h"

#define DEE_TIMEOUT              16000

typedef struct{
    unsigned long key;
    unsigned long value;
} DEE_DATA;

#define DEE_NUMBER_OF_DATA   (DEE_PAGE_SIZE_BYTES/sizeof(DEE_DATA))

// two pages/banks for data
DEE_DATA __attribute__((aligned(DEE_PAGE_SIZE_BYTES),
                        address(__KSEG0_PROGRAM_MEM_BASE+__KSEG0_PROGRAM_MEM_LENGTH-2*DEE_PAGE_SIZE_BYTES),
                        persistent)) dee[2][DEE_NUMBER_OF_DATA];

void DEE_Init()
{
    // unlock main flash
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;    
    NVMPWP = 0x80000000;   
}

unsigned long DEE_Program()
{   
unsigned long timeout = DEE_TIMEOUT;
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;
    // set WR bit
    NVMCONSET = 0x00008000;
    while(NVMCONbits.WR == 1)
    {
        timeout--;
        if(timeout == 0)
        {        
            return 1;
        }
    }
    return 0;
}


unsigned long DEE_BankErase(unsigned long bank)
{
unsigned long address = (unsigned long)&dee[bank][0];

    NVMCON = 0;
    NVMCON = 0x4004;    
    NVMADDR = address&0x1FFFFFFF;    
    return DEE_Program();
}

unsigned long DEE_DataProgram(unsigned long bank, unsigned long offset, unsigned long key, unsigned long value)
{
unsigned long address = (unsigned long)&dee[bank][offset];

    NVMCON = 0;
    NVMCON = 0x4002;
    NVMADDR = address&0x1FFFFFFF;
	NVMDATA0 = key;
	NVMDATA1 = value;
    return DEE_Program();
}

unsigned long DEE_GetBank()
{
    if(dee[0][0].key == 0xFFFFFFFF)
    {
        if(dee[1][0].key == 0xFFFFFFFF){
            return 0;
        }else{
            return 1;
        }    
    }
    return 0;
}

unsigned long DEE_AddData(unsigned long bank, unsigned long key, unsigned long value, unsigned long skip)
{
long offset;

    offset = 0;
    
    while(offset < DEE_NUMBER_OF_DATA)
    {
        if(skip)        
        if(dee[bank][offset].key == key)
        {
            return 0;
        }
        if(dee[bank][offset].key == 0xFFFFFFFF)
        {
            return DEE_DataProgram(bank, offset, key, value);
        }
        offset++;    
    }
    return 1;
}

unsigned long DEE_SwitchBank()
{
unsigned long current_bank;
unsigned long new_bank;
long offset;
unsigned long key;
unsigned long value;
    
    current_bank = DEE_GetBank();
    if(current_bank){
        new_bank = 0;
    }else{
        new_bank = 1;
    }   
    
    offset = DEE_NUMBER_OF_DATA-1;
    
    do{
        key = dee[current_bank][offset].key;
        if(key != 0xFFFFFFFF)
        {
            value = dee[current_bank][offset].value;
            if(DEE_AddData(new_bank, key, value, 1))
            {            
                return 1;
            }
        }
        offset--;
    }while(offset >= 0);

    return DEE_BankErase(current_bank);
}

unsigned long DEE_Write(unsigned long key, unsigned long value)
{
unsigned long bank;
    
    bank = DEE_GetBank();
    if(DEE_AddData(bank, key, value, 0))
    {
        if(DEE_SwitchBank())
        {
            return 1;
        }    
        bank = DEE_GetBank();
        if(DEE_AddData(bank, key, value, 0))
        {
            return 1;
        }
    }
        
    return 0;
}

unsigned long DEE_Read(unsigned long key)
{
unsigned long bank;
long offset;
    
    bank = DEE_GetBank();
    offset = DEE_NUMBER_OF_DATA-1;
    
    do{
        if(dee[bank][offset].key == key)
        {
            return dee[bank][offset].value;
        }
        offset--;
    }while(offset >= 0);
       
    return 0xFFFFFFFF;
}
