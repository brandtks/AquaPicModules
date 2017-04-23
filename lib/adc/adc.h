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

#ifndef ADC_H
#define	ADC_H

/* WIP adding config bits as needed */
#define CHS_AN0             0b00000     /* CHS: Analog Channel Select bits, AN0 */

#define AD0_CHS_AN0         0b00000000  /* CHS: Analog Channel Select bits, AN0 */
#define AD0_ADON_ENABLE     0b00000001  /* ADON: ADC Enable bit, ADC is enabled */
#define AD0_ADON_DISABLE    0b00000000  /* ADON: ADC Enable bit, ADC is disabled */

#define AD1_ADFM_RIGHT      0b10000000  /* ADFM: A/D Result Format Select, Right Justified */
#define AD1_ADCS_FOSC_16    0b01010000  /* ADCS: A/D Conversion Clock Select, FOSC/16 */
#define AD1_ADNREF_VSS      0b00000000  /* ADNREF: A/D Negative Voltage Reference Configuration, VREF- is connected to Vss */
#define AD1_ADPREF_VDD      0b00000000  /* ADPREF: A/D Positive Voltage Reference Configuration, VREF+ is connected to Vdd */

#define START_ADC()          GO = 1
#define SET_CHANNEL(channel) ADCON0bits.CHS = channel

void initAdc (uint8_t adcon0, uint8_t adcon1);
uint16_t getAdc (void);

#endif	/* ADC_H */
