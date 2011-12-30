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

// SD
//    Data lines
#define SD_SPI_SEL  P3SEL
#define SD_SPI_DIR  P3DIR
//#define SD_SPI_STE  BIT0
#define SD_SPI_SIMO BIT1
#define SD_SPI_SOMI BIT2
#define SD_SPI_CLK  BIT3
//    Card detect
#define SD_CD_DIR P3DIR
#define SD_CD_IN  P3IN
#define SD_CD_BIT BIT4
#define sd_card_detect() (!(SD_CD_IN & SD_CD_BIT))
//    Chip Select
#define SD_CS_DIR P3DIR
#define SD_CS_OUT P3OUT
#define SD_CS_BIT BIT0
#define sd_set_cs() SD_CS_OUT &= ~SD_CS_BIT
#define sd_clr_cs() SD_CS_OUT |= SD_CS_BIT
#define sd_init_cs() SD_CS_DIR |= SD_CS_BIT
//    IFG macros
 #define sd_spi_rx_buf  UCA0RXBUF
 #define sd_spi_send(x) UCA0TXBUF=(x)
 #define sd_spi_tx_ready()  (IFG2&UCA0TXIFG)
 #define sd_spi_tx_done  (UCA0STAT&UCBUSY)
 #define sd_spi_rx_ready() (IFG2&UCA0RXIFG)
 #define sd_spi_rx_FG_clr() IFG2 &= ~UCA0RXIFG

#endif /* HW_CONF_H_ */