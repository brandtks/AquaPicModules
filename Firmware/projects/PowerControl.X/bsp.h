/*
 * Board Support Package
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

#ifndef BSP_H
#define	BSP_H

/******************************************************************************/
/* CONFIGUTION WORDS                                                          */
/******************************************************************************/
/*CONFIG1*/
#pragma config FOSC = HS        //Oscillator Selection (HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins)
#pragma config WDTE = OFF       //Watchdog Timer Enable (WDT disnabled)
#pragma config PWRTE = OFF      //Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       //MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         //Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        //Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       //Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   //Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        //Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       //Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

/*CONFIG2*/
#pragma config WRT = OFF        //Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = RA5     //Voltage Regulator Capacitor Enable (VCAP functionality is enabled on RA5)
#pragma config PLLEN = ON       //PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      //Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        //Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        //Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#define XTAL_FREQ       32000000UL  //Used by the __delay_ms(xx) and __delay_us(xx) Methods, 32MHz

#define RED_LED_PORT    &LATC
#define GREEN_LED_PORT  &LATC
#define YELLOW_LED_PORT &LATC

#define RED_LED_PIN     0
#define GREEN_LED_PIN   1
#define YELLOW_LED_PIN  2

#define OUTLET1_RELAY_PORT  &LATD
#define OUTLET2_RELAY_PORT  &LATD
#define OUTLET3_RELAY_PORT  &LATD
#define OUTLET4_RELAY_PORT  &LATD
#define OUTLET5_RELAY_PORT  &LATD
#define OUTLET6_RELAY_PORT  &LATD
#define OUTLET7_RELAY_PORT  &LATD
#define OUTLET8_RELAY_PORT  &LATD

#define OUTLET1_RELAY_PIN   0
#define OUTLET2_RELAY_PIN   1
#define OUTLET3_RELAY_PIN   2
#define OUTLET4_RELAY_PIN   3
#define OUTLET5_RELAY_PIN   4
#define OUTLET6_RELAY_PIN   5
#define OUTLET7_RELAY_PIN   6
#define OUTLET8_RELAY_PIN   7

#define POWER_AVAIL_PORT    &PORTC
#define POWER_AVAIL_PIN     3

#define TX_ENABLE_PORT  &LATC
#define TX_ENABLE_PIN   5
#define APB_ADDRESS     0x10
#define COMM_ERROR_SP   400 /* 25mSec timer interrupt, 10 sec alarm */
                            /* 10,000mSec / 25mSec = 200 */

#endif	/* BSP_H */

