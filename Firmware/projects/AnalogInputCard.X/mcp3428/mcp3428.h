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

#ifndef MCP_3428_H
#define MCP_3428_H

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
int8_t mcp3428_init(uint8_t addressVar);
int8_t mcp3428_setChannelAndStartConversion(uint8_t channel);
/* Call at 1ms */
int16_t mcp3428_polling();
int8_t mcp3428_getChannel();
int16_t mcp3428_getResult();

#endif /* MCP_3428_H */