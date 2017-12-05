/*
 * Pins
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

#ifndef PINS_H
#define	PINS_H

#define HIGH    1
#define LOW     0

#define WRITE_PIN(port, pin, value) value ? *port |= 0x01 << pin : *port &= ~(0x01 << pin)
#define READ_PIN(port, pin)         *port & (0x01 << pin)

#endif	/* PINS_H */ 
