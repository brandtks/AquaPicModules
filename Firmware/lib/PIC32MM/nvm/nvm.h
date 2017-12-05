/*******************************************************************************
 * nvm.h
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

#ifndef NVM_H
#define NVM_H

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define NUMBER_64_BIT_MEMORY_LOCATIONS     4

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
uint64_t nvm_read(uint8_t index);
uint32_t nvm_write(uint8_t index, uint64_t value);

#endif /* AQUAPIC_BUS_H */