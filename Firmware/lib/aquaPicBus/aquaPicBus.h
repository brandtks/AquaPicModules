/*******************************************************************************
 * aquaPicBus.h
 *
 * Goodtime Development 
 * 
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  AquaPic Bus Slave driver
 * 
 * Device:
 *  Any
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

#ifndef AQUAPIC_BUS_H
#define AQUAPIC_BUS_H

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/*****Initialize***************************************************************/
int8_t apb_init(void (*messageHandlerVar)(uint8_t, uint8_t*, uint8_t),
        void (*putschVar)(uint8_t*, uint8_t),
        uint8_t addressVar,
        uint8_t framingTimerTime,               /* In milliseconds */
        uint16_t errorTime);                    /* In seconds */

/*****Run Time*****************************************************************/
void apb_run(const uint8_t byteRecieved);
void apb_framing();
int8_t apb_isErrored();

/* This clears the message buffer so make sure you have all the data stored from the command */
void apb_sendDefualtResponse();

/* This clears the message buffer so make sure you have all the data stored from the command */
void apb_initResponse();
void apb_appendToResponse(uint8_t data);
void apb_addToResponse(void* data, size_t length);
void apb_sendResponse();

#endif /* AQUAPIC_BUS_H */