/*
 * hw_conf.h
 *
 *  Created on: Dec 3, 2011
 *      Author: aviv
 */

#ifndef HW_CONF_H_
#define HW_CONF_H_

// USCI A1 for communication with the GPS module
#define UART_DIR    P3DIR
#define UART_SEL    P3SEL
#define UART_RX_BIT BIT7
#define UART_TX_BIT BIT6

// GPS module controls
#define GPS_POWER_DIR P4DIR
#define GPS_POWER_OUT P4OUT
#define GPS_POWER_BIT BIT3
#define GPS_ON() GPS_POWER_OUT |= GPS_POWER_BIT
#define GPS_OFF() GPS_POWER_OUT &= ~GPS_POWER_BIT

// LED
#define LED_DIR P4DIR
#define LED_OUT P4OUT
#define LED_BIT BIT0
#define LED_ON() LED_OUT &= ~LED_BIT
#define LED_OFF() LED_OUT |= LED_BIT

#endif /* HW_CONF_H_ */
