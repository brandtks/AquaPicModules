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

#define CHS_AN0             0b00000     /* CHS: Analog Channel Select bits, AN0 */
#define CHS_AN1             0b00001     /* CHS: Analog Channel Select bits, AN1 */
#define CHS_AN2             0b00010     /* CHS: Analog Channel Select bits, AN2 */
#define CHS_AN3             0b00011     /* CHS: Analog Channel Select bits, AN3 */
#define CHS_AN4             0b00100     /* CHS: Analog Channel Select bits, AN4 */
#define CHS_AN5             0b00101     /* CHS: Analog Channel Select bits, AN5 */
#define CHS_AN6             0b00110     /* CHS: Analog Channel Select bits, AN6 */
#define CHS_AN7             0b00111     /* CHS: Analog Channel Select bits, AN7 */
#define CHS_AN8             0b01000     /* CHS: Analog Channel Select bits, AN8 */
#define CHS_AN9             0b01001     /* CHS: Analog Channel Select bits, AN9 */
#define CHS_AN10            0b01010     /* CHS: Analog Channel Select bits, AN10 */
#define CHS_AN11            0b01011     /* CHS: Analog Channel Select bits, AN11 */
#define CHS_TEMP_IND        0b11101     /* CHS: Analog Channel Select bits, Temperatire Indicator */
#define CHS_DAC             0b11110     /* CHS: Analog Channel Select bits, DAC Output */
#define CHS_FVR             0b11111     /* CHS: Analog Channel Select bits, Fixed Voltage Referece Buffer 1 Output */

#define AD0_CHS_AN0         0b00000000  /* CHS: Analog Channel Select bits, AN0 */
#define AD0_CHS_AN1         0b00000100  /* CHS: Analog Channel Select bits, AN1 */
#define AD0_CHS_AN2         0b00001000  /* CHS: Analog Channel Select bits, AN2 */
#define AD0_CHS_AN3         0b00001100  /* CHS: Analog Channel Select bits, AN3 */
#define AD0_CHS_AN4         0b00010000  /* CHS: Analog Channel Select bits, AN4 */
#define AD0_CHS_AN5         0b00010100  /* CHS: Analog Channel Select bits, AN5 */
#define AD0_CHS_AN6         0b00011000  /* CHS: Analog Channel Select bits, AN6 */
#define AD0_CHS_AN7         0b00011100  /* CHS: Analog Channel Select bits, AN7 */
#define AD0_CHS_AN8         0b00100000  /* CHS: Analog Channel Select bits, AN8 */
#define AD0_CHS_AN9         0b00100100  /* CHS: Analog Channel Select bits, AN9 */
#define AD0_CHS_AN10        0b00101000  /* CHS: Analog Channel Select bits, AN10 */
#define AD0_CHS_AN11        0b00101100  /* CHS: Analog Channel Select bits, AN11 */
#define AD0_CHS_TEMP_IND    0b01110100  /* CHS: Analog Channel Select bits, Temperatire Indicator */
#define AD0_CHS_DAC         0b01111000  /* CHS: Analog Channel Select bits, DAC Output */
#define AD0_CHS_FVR         0b01111100  /* CHS: Analog Channel Select bits, Fixed Voltage Referece Buffer 1 Output */
#define AD0_GO              0b00000010  /* GO/DONE: A/D Conversion Status bit, Setting starts ADC, Cleared by hardware */ 
#define AD0_ADON_ENABLE     0b00000001  /* ADON: ADC Enable bit, ADC is enabled */
#define AD0_ADON_DISABLE    0b00000000  /* ADON: ADC Enable bit, ADC is disabled */

#define AD1_ADFM_RIGHT      0b10000000  /* ADFM: A/D Result Format Select, Right Justified */
#define AD1_ADFM_LEFT       0b00000000  /* ADFM: A/D Result Format Select, Left Justified */
#define AD1_ADCS_FOSC_2     0b00000000  /* ADCS: A/D Conversion Clock Select, FOSC/2 */
#define AD1_ADCS_FOSC_8     0b00010000  /* ADCS: A/D Conversion Clock Select, FOSC/8 */
#define AD1_ADCS_FOSC_32    0b00100000  /* ADCS: A/D Conversion Clock Select, FOSC/32 */
#define AD1_ADCS_FOSC_4     0b01000000  /* ADCS: A/D Conversion Clock Select, FOSC/4 */
#define AD1_ADCS_FOSC_16    0b01010000  /* ADCS: A/D Conversion Clock Select, FOSC/16 */
#define AD1_ADCS_FOSC_64    0b01100000  /* ADCS: A/D Conversion Clock Select, FOSC/64 */
#define AD1_ADCS_FRC        0b01110000  /* ADCS: A/D Conversion Clock Select, FRC. Dedicated RC oscillator */
#define AD1_ADNREF_VSS      0b00000000  /* ADNREF: A/D Negative Voltage Reference Configuration, VREF- is connected to Vss */
#define AD1_ADNFEF_VREFN    0b00000100  /* ADNREF: A/D Negative Voltage Reference Configuration, VREF- is connected to external Vref- */
#define AD1_ADPREF_VDD      0b00000000  /* ADPREF: A/D Positive Voltage Reference Configuration, VREF+ is connected to Vdd */
#define AD1_ADPREF_VREFP    0b00000010  /* ADPREF: A/D Positive Voltage Reference Configuration, VREF+ is connected to external Vref+ */
#define AD1_ADPREF_FVR      0b00000011  /* ADPREF: A/D Positive Voltage Reference Configuration, VREF+ is connected to internal Fixed Voltage Reference */

#define START_ADC()          GO = 1
#define SET_CHANNEL(channel) ADCON0bits.CHS = channel

void inline initAdc(uint8_t adcon0, uint8_t adcon1);
uint16_t getAdc(void);

#endif	/* ADC_H */
