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

#ifndef UART_H
#define	UART_H

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
#ifdef _16F18855
    #define TX_STA      TX1STA
    #define RC_STA      RC1STA
    #define BAUD_CON    BAUD1CON
    #define SP_BRGL     SP1BRGL
    #define SP_BRGH     SP1BRGH
#elif _16F1936
    #define TX_STA      TXSTA
    #define RC_STA      RCSTA
    #define BAUD_CON    BAUDCON
    #define SP_BRGL     SPBRGL
    #define SP_BRGH     SPBRGH
#endif

#define TX_CSRC_MASTER  0b10000000  /* CSRC: Clock Source Select bit, Master mode (clock generated internally from BRG) */
#define TX_CSRC_SLAVE   0b00000000  /* CSRC: Clock Source Select bit, Slave mode (clock from external source) */
#define TX_TX9_9BIT     0b01000000  /* TX9: 9-bit Transmit Enable bit, Selects 9-bit transmission */
#define TX_TX9_8BIT     0b00000000  /* TX9: 9-bit Transmit Enable bit, Selects 8-bit transmission */
#define TX_TXEX_ENABLE  0b00100000  /* TXEN: Transmit Enable bit, Transmit enabled */
#define TX_TXEX_DISABLE 0b00000000  /* TXEN: Transmit Enable bit, Transmit disabled */
#define TX_SYNC_SYNC    0b00010000  /* SYNC: EUSART Mode Select bit, Synchronous mode */
#define TX_SYNC_ASYNC   0b00000000  /* SYNC: EUSART Mode Select bit, Asynchronous mode */
#define TX_SENDB_START  0b00001000  /* SENDB: Send Break Character bit, Send SYNCH BREAK on next transmission */
#define TX_SENDB_DONE   0b00000000  /* ENDB: Send Break Character bit, SYNCH BREAK transmission disabled or completed */
#define TX_BRGH_HIGH    0b00000100  /* BRGH: High Baud Rate Select bit, High speed */
#define TX_BRGH_LOW     0b00000000  /* BRGH: High Baud Rate Select bit, Low speed */

#define RC_SPEN_ENABLE  0b10000000  /* SPEN: Serial Port Enable bit,  Serial port enabled */
#define RC_SPEN_DISABLE 0b00000000  /* SPEN: Serial Port Enable bit,  Serial port disabled */
#define RC_RC9_9BIT     0b01000000  /* RC9: 9-bit Receive Enable bit, Selects 9-bit reception */
#define RC_RC9_8BIT     0b00000000  /* RC9: 9-bit Receive Enable bit, Selects 8-bit reception */
#define RC_SREN_ENABLE  0b00100000  /* SREN: Single Receive Enable bit, Enables single receive */
#define RC_SREN_DISABLE 0b00000000  /* SREN: Single Receive Enable bit, Disables single receive */
#define RC_CREN_ENABLE  0b00010000  /* CREN: Continuous Receive Enable bit, Enables continuous receive until enable bit CREN is cleared */
#define RC_CREN_DISABLE 0b00000000  /* CREN: Continuous Receive Enable bit, Disables continuous receive */
#define RC_ADDEN_ENABLE 0b00001000  /* ADDEN: Address Detect Enable bit,  Enables address detection */
#define RC_ADDEN_DISABLE 0b00000000 /* ADDEN: Address Detect Enable bit,  Disables address detection */

#define BAUD_SCKP_ASYNC_LOW     0b00010000  /* SCKP: Clock/Transmit Polarity Select bit, Idle state for transmit (TX) is a low level */
#define BAUD_SCKP_ASYNC_HIGH    0b00000000  /* SCKP: Clock/Transmit Polarity Select bit, Idle state for transmit (TX) is a high level */
#define BAUD_BRG16_16BIT        0b00001000  /* BRG16: 16-bit Baud Rate Generator bit, 16-bit Baud Rate Generator is used */
#define BAUD_BRG16_8BIT         0b00000000  /* BRG16: 16-bit Baud Rate Generator bit, 8-bit Baud Rate Generator is used */
#define BAUD_WUE_ENABLE         0b00001000  /* WUE: Wake-up Enable bit, USART will continue to sample the Rx pin */
#define BAUD_WUE_DISABLE        0b00000000  /* WUE: Wake-up Enable bit, RX pin not monitored nor rising edge detected */

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
void inline uart_init(uint8_t txsta, 
        uint8_t rcsta, 
        uint8_t baudcon, 
        uint8_t spbrgl,
        uint8_t spbrgh);
void inline uart_enable();
void inline uart_disable();

void putsch(uint8_t* data, uint8_t length);

#endif	/* UART_H */

