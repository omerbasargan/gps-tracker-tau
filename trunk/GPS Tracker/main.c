/*
 * main.c
 */
#include <msp430.h>
#include <msp430f2618.h>

#include "hw_conf.h"
#include "drivers/uart.h"
#include "drivers/gps.h"

void hw_setup(void);

// TODO remove this!
char lines[5][128];

void main(void)
{
  char buf[1];
  int nextIn = 0;
  int curLine = 0;
  volatile unsigned long dummy;

  WDTCTL = WDTPW | WDTHOLD; // stop watch-dog timer

  hw_setup();
  uart_init();
  gps_init();

	while ( 1 ) 
	{
    LED_ON(); // Use break point here

    while (curLine < 5)
    {
      int len = uart_read(1, buf);
      if (len > 0)
      {
        lines[curLine][nextIn++] = buf[0];
        if (buf[0] == '\n')
        {
          nextIn = 0;
          curLine++;
        }
      }
    }

    LED_OFF();
  }
  __disable_interrupt();
  while (1);
}

void led_setup(void);
void clocks_setup(void);
void timers_setup(void);

void hw_setup(void)
{
  led_setup();

  LED_ON(); // for hw setup duration

  clocks_setup();
  timers_setup();

  __enable_interrupt();
  LED_OFF();
}

void led_setup(void)
{
  LED_DIR |= LED_BIT;
  LED_OFF();
}

void clocks_setup(void)
{
  int i, j;

  DCOCTL = 0xA4; // DCO Setup (about 12MHz)
  BCSCTL1 = 0x0E; // ACLK  = Xtal1 (=32,768 Hz)
  BCSCTL2 = SELS; // MCLK  = DCO (about 12MHz)
  BCSCTL3 = XT2S_2 | XCAP_3; // SMCLK = Xtal2 (=8MHz)

  for (i = 0; i < 200; i++)
  {
    for (j = 0; j < 300; j++)
      __no_operation();
  }
}

void timers_setup(void)
{
  // TODO
}
