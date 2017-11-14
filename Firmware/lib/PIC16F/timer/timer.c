/*
 * Timer
 * 
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
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>
#include <stdint.h>     /* uint8_t, int8_t */
#include "timer.h"

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
#ifdef _16F18855
void inline timer_init8bit(uint8_t timerName, 
        uint8_t clkcon, 
        uint8_t con, 
        uint8_t hlt, 
        uint8_t rst, 
        uint8_t pr) 
{
    switch (timerName) {
        case T2:
            T2CLKCON = clkcon;
            T2CON = con;
            T2HLT = hlt;
            T2RST = rst;
            T2PR = pr;
            break;
        default:
            break;
    }
}
#endif