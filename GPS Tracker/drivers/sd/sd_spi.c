/*
 * sd_spi.c
 *
 *  This is a standard spi implementation used by the sd driver
 */

#include "msp430f2618.h"
#include "hw_conf.h"

void sd_spi_init(void)
{
  //
  // Init HW state machine
  //

  // hold USCI in reset
  UCB0CTL1 |= UCSWRST;

  UCB0CTL0 = UCMST | UCMSB | UCSYNC | UCCKPL ; // 3 Pin SPI, Master mode, MSB 1st
  UCB0CTL1 |= UCSSEL_3; // Clock source is SMCLK (8MHz)
  UCB0BR0 = 20; // Bit Rate = 8 MHz / 20 = 400 KHz
  UCB0BR1 = 0;

  //
  // Init ports
  //

  // Data lines
  SD_SPI_DIR |= (SD_SPI_SIMO|SD_SPI_CLK); // ouput bits
  SD_SPI_DIR &= ~SD_SPI_SOMI; // input bits
  SD_SPI_SEL |= (SD_SPI_SIMO|SD_SPI_CLK|SD_SPI_SOMI); // USCI selected

  // Start the SPI
  UCB0CTL1 &= ~UCSWRST; // clear reset
//  IE2 |= (UCB0TXIE|UCB0RXIE); // enable interrupts
}

void sd_spi_write(unsigned char * bytes, unsigned int len)
{
  unsigned int i = 0;
  for (i = 0; i < len; i++){
    while (sd_spi_tx_ready() == 0);   // wait while not ready for TX
    sd_spi_send(bytes[i]);
    while (sd_spi_rx_ready() == 0);   // wait for RX buffer
    bytes[i] = sd_spi_rx_buf;
  }
}

unsigned char sd_spi_write_byte(unsigned char byte)
{
  while (sd_spi_tx_ready() == 0);    // wait while not ready for TX
  sd_spi_send(byte);            // write
  while (sd_spi_rx_ready() == 0);    // wait for RX buffer
  return (sd_spi_rx_buf);
}

void sd_spi_read(unsigned char * buffer, unsigned int len)
{
  unsigned int i = 0;
  char dummy = 0xFF;
  for (i = 0; i < len; i++){
    while (sd_spi_tx_ready() == 0);   // wait while not ready for TX
    sd_spi_send(dummy);
    while (sd_spi_rx_ready() == 0);   // wait for RX buffer
    buffer[i] = sd_spi_rx_buf;
  }
}

