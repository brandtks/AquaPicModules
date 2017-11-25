/*******************************************************************************
 * mcp3428.h
 *
 * Goodtime Development 
 * 
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  MCP3428 Analog to digital converter device driver
 ******************************************************************************/

/******************************************************************************/
/* License                                                                    */
/******************************************************************************/
/*******************************************************************************
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
 ******************************************************************************/

#ifndef LTC_2483_H
#define LTC_2483_H

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
typedef enum mcp3428StatusEnum {
    WAIT_FOR_CONVERSION,
    WAIT_FOR_STATUS
}ltc2483Status_t;

struct ltc2483ObjStruct {
    uint8_t address;
    int16_t result;
    uint8_t response[3];
    ltc2483Status_t ltc2483Status;
    uint8_t timeOutCount;
    uint8_t retryCount;
    I2C1_MESSAGE_STATUS i2cStatus;
};

typedef struct ltc2483ObjStruct* ltc2483Obj;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
int8_t ltc2483_init(ltc2483Obj ltc2483Inst, uint8_t addressVar);
/* Call at 1ms */
int16_t ltc2483_polling(ltc2483Obj ltc2483Inst);
int16_t ltc2483_getResult(ltc2483Obj ltc2483Inst);

#endif /* LTC_2483_H */