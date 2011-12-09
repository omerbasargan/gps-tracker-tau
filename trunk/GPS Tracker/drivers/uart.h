/*
 * uart.h
 *  USCI A1 driver for communication with the GPS module
 *  Created on: Dec 3, 2011
 */

#ifndef UART_H_
#define UART_H_


#define UART_TX_BUF_LEN 128
#define UART_RX_BUF_LEN 256

void uart_init(void);
void uart_send(int len, char* vec);
int  uart_read(int max, char* vec);

void uart_enable(void);
void uart_disable(void);

int uart_send_available(void);
char uart_is_tx_clear(void);

#endif /* UART_H_ */
