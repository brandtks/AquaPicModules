/*
 * Analog To Digital Converter
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
#include "adc.h" 

void initAdc (uint8_t adcon0, uint8_t adcon1) {
    ADCON0 = adcon0;
    ADCON1 = adcon1;
}

uint16_t getAdc (void) {
    uint16_t counts;

    counts = ADRESH << 8; //grab highest 2 bits shift right 8
    counts |= ADRESL;

    return counts;
}
