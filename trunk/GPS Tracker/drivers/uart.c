/*
 * uart.c
 *
 *  USCI A1 driver for communication with the GPS module
 *  Created on: Dec 3, 2011
 */

#include <msp430f2618.h>
#include "..\hw_conf.h"
#include "uart.h"

// data structure
char uartTxBuf[UART_TX_BUF_LEN];
char uartRxBuf[UART_RX_BUF_LEN];
//
unsigned int uartRxNextIn;
unsigned int uartRxNextOut;
unsigned char bUartRxValid;
//
unsigned int uartTxNextIn;
unsigned int uartTxNextOut;
unsigned char bUartTxClear;
//

//
// routines
//
// Enable Tx/Rx interrupts
void uart_enable(void)
{
  UC1IE |= (UCA1TXIE|UCA1RXIE);
}

// Disable Tx/Rx interrupts
void uart_disable(void)
{
  UC1IE &= ~(UCA1TXIE|UCA1RXIE);
}

/*
 * Initiate USCI_A1 to 9,600 BR, 8 bit, 1 stop, no parity, LSB 1st
 * and enables interrupts.
 */
void uart_init(void)
{
  // USCI_A1 init according to the 2xx user guide section 15.3.1
  UCA1CTL1 |= UCSWRST; // set USCI reset
  UCA1CTL0 = UCMODE_0; // set UART mode, 8 bit, 1 stop, no parity, lsb 1st
  UCA1CTL1 = UCSSEL_2|UCSWRST; // Source clock is SMCLK (=8MHz)

  // Baud rate
  UCA1BR0 = 0x41 ; // 9,600
  UCA1BR1 = 0x03 ;
  UCA1MCTL = UCBRS_2 ; // modulation to adjust to the clock freq

  // ports
  UART_DIR |= UART_TX_BIT;  // Set TX bit as output
  UART_DIR &= ~UART_RX_BIT; // Set RX bit as input
  UART_SEL |= (UART_TX_BIT|UART_RX_BIT); // Set Tx/Rx bits as primary function

  UCA1CTL1 &= ~UCSWRST; // clear reset bit

  uartRxNextIn = 0;
  uartRxNextOut = 0;
  bUartRxValid = 1;

  uartTxNextIn = 0;
  uartTxNextOut = 0;
  bUartTxClear = 1;

  uart_enable();
}

// Puts a vector in the output buffer and starts the sending process
void uart_send(int len, char* vec)
{
  if ( len < uart_send_available() )
  { // there's enough room to send the message
    while ( (len--) > 0 )
    {
      uartTxBuf[uartTxNextIn++] = *(vec++);
      if ( uartTxNextIn >= UART_TX_BUF_LEN )
        uartTxNextIn -= UART_TX_BUF_LEN;
    }
    if (bUartTxClear) // Tx is clear
    {
      bUartTxClear = 0;
      UCA1TXBUF = uartTxBuf[uartTxNextOut++];
      if (uartTxNextOut >= UART_TX_BUF_LEN)
        uartTxNextOut -= UART_TX_BUF_LEN;
    }
  }
}

// Reads up to max bytes from the input buffer to vec and removes them from the buffer
int  uart_read(int max, char* vec)
{
  int len = 0;

  if ( max == 0 )
    return(0);

  if ( ! bUartRxValid )
  { // Rx invalid. flush buffer and bail.
    uartRxNextOut = uartRxNextIn;
    bUartRxValid = 1;
    return(0);
  }

  while ( len < max )
  {
    if ( uartRxNextOut == uartRxNextIn )
      return(len);
    vec[len++] = uartRxBuf[uartRxNextOut++];
    if ( uartRxNextOut >= UART_RX_BUF_LEN )
      uartRxNextOut -= UART_RX_BUF_LEN;
  }
  return(len);
}

// Returns the room left on the Tx buffer
int uart_send_available(void)
{
  int room = uartTxNextOut - uartTxNextIn;
  if ( room <= 0 )
    room += UART_TX_BUF_LEN;
  return(room);
}

char uart_is_tx_clear(void)
{
  return bUartTxClear;
}

// TX ISR
#pragma vector=USCIAB1TX_VECTOR
__interrupt void uartTxISR(void)
{
  if ( uartTxNextOut != uartTxNextIn )
  { // more bytes to send
    UCA1TXBUF = uartTxBuf[uartTxNextOut++];
    if (uartTxNextOut >= UART_TX_BUF_LEN)
      uartTxNextOut -= UART_TX_BUF_LEN;
  }
  else
  {
    bUartTxClear = 1;
    UC1IFG &= ~UCA1TXIFG;
  }
}

// RX ISR
#pragma vector=USCIAB1RX_VECTOR
__interrupt void uartRxISR(void)
{
  if ( UCA1STAT & UCRXERR )
  { // Error in rx process
    volatile char c;
    bUartRxValid = 0;
    c = UCA1RXBUF; // clear current char
  }
  else
  {
    int nextPos = uartRxNextIn+1; //  to avoid unintentional clearing of the buffer
    if ( nextPos >= UART_RX_BUF_LEN)
      nextPos -= UART_RX_BUF_LEN;
    if ( nextPos == uartRxNextOut )
    { // Next in is on the last byte in the buffer and has no where to increase
      volatile char c;
      bUartRxValid = 0; // skipped a character - rx invalid
      c = UCA1RXBUF; // clear current char;
    }
    else
    {
      uartRxBuf[uartRxNextIn] = UCA1RXBUF; // rad char to buffer
      uartRxNextIn = nextPos;
    }
  }
}
