/*
 * UART
 * 
 * Copyright (c) 2017 Skyler Brandt
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

#include <xc.h>
#include <stdint.h>     /* uint8_t, int8_t */
#include "uart.h"

void inline initUart(uint8_t txsta, 
        uint8_t rcsta, 
        uint8_t baudcon, 
        uint8_t spbrgh, 
        uint8_t spbrg1) 
{
    TX_STA = txsta;
    RC_STA = rcsta;
    BAUD_CON = baudcon;
    SP_BRGH = spbrgh;
    SP_BRGL = spbrg1;
}

void inline enableUart() {
    TXSTAbits.TXEN = 1; /* Transmit Enable, Transmit enabled            */
    RCSTAbits.CREN = 1; /* Continuous Receive Enable, Enables receiver  */
}

void inline disableUart() {
    TXSTAbits.TXEN = 0; /* Transmit Enable, Transmit disable            */
    RCSTAbits.CREN = 0; /* Continuous Receive Enable, disable receiver  */
}

void putsch(uint8_t* data, uint8_t length) {
    while(length--) {
        TXREG = *data++;
        while (!TXIF) //TXIF is set when the TXREG is empty
            continue;
    }

    while (!TRMT) //wait for transmit shift register to be empty
        continue;
}