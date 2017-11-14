/*
 * Analog Input Card - main.c
 *
 * Created 2017 Skyler Brandt
 *
 * Description:
 *  Firmware for the analog input card
 * 
 * Device:
 *  PIC32MM0064GPM028
 */

/*
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

#include "mcc_generated_files/mcc.h"

void main(void) {
    /* initialize the device */
    SYSTEM_Initialize();


    /* Enable the Global Interrupts */
    INTERRUPT_GlobalEnable();

    while (1) {
        
    }
}