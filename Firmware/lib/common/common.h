/*
 * Common
 * 
 */

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

#define assignFlagStateMask(word, mask, b)  b ? (word |= mask) : (word &= ~mask)
#define setFlagMask(word, mask)     word |= mask
#define resetFlagMask(word, mask)   word &= ~mask
#define testFlagMask(word, mask)    (word & mask) ? 1 : 0

#define assignFlagStateBit(word, bit, b)    b ? (word |= (0x01 << bit)) : (word &= ~(0x01 << bit))
#define setFlagBit(word, bit)       word |= (0x01 << bit)
#define resetFlagBit(word, bit)     word &= ~(0x01 << bit)
#define testFlagBit(word, bit)      (word & (0x01 << bit)) ? 1 : 0

#endif  /* COMMON_H */
