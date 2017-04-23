/*
 * Common
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

#ifndef COMMON_H
#define COMMON_H

/* some commonly used macros */
#define range(i, min, max)          (i >= min) && (i <= max) ? 1 : 0
#define rangeDeadband(i, x, db)     (i >= (x-db)) && (i <= (x+db)) ? 1 : 0
#define rangeInc(i, max)            ((i+1) <= max) ? i+1 : i
#define rangeDcr(i, min)            ((i-1) >= min) ? i-1 : i

#define increment(i, max)           i = (++i) % max

#define float2int(f)                (int)((f < 0.0f) ? f - 0.5f : f + 0.5f)
#define float2int16_t(f)            (int16_t)((f < 0.0f) ? f - 0.5f : f + 0.5f)

#define int2bcd(i)	(uint8_t)(((i)/10<<4) + (i) % 10)
#define bcd2int(i)	(uint8_t)((((i)>>4) * 10) + ((i) & 0x0F))

#define maskFlagSet(word, mask, b)  b ? (word |= mask) : (word &= ~mask)
#define maskFlagOff(word, mask)     word &= ~mask
#define maskFlagOn(word, mask)      word |= mask
#define maskFlagTest(word, mask)    (word & mask) ? 1 : 0

#define bitFlagSet(word, bit, b)    b ? (word |= (0x01 << bit)) : (word &= ~(0x01 << bit))
#define bitFlagOff(word, bit)       word &= ~(0x01 << bit)
#define bitFlagOn(word, bit)        word |= (0x01 << bit)
#define bitFlagTest(word, bit)      (word & (0x01 << bit)) ? 1 : 0

#endif  /* COMMON_H */
