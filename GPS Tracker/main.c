/*
 * main.c
 */
#include <msp430.h>
#include <msp430f2618.h>

#include "hw_conf.h"
#include "drivers/timer.h"
#include "drivers/uart.h"
#include "drivers/gps.h"
#include "drivers/sd/sd.h"

void hw_setup(void);

// TODO remove this!
unsigned char lines[5][128];
unsigned char lengths[5];

#define HEADER_LEN 14
#define FOOTER_LEN 12
const char header_str[HEADER_LEN] = "Start of file\n";
const char footer_str[FOOTER_LEN] = "End of file\n";

void main(void)
{
  char buf[1];
  int nextIn = 0;
  int curLine = 0;
  int i;
  volatile unsigned long dummy;

  WDTCTL = WDTPW | WDTHOLD; // stop watch-dog timer

  hw_setup();
  uart_init();
  
  GPS_OFF();
  
  sd_init();
  
  GPS_ON();
  
  gps_init();

//	while ( 1 )
//	{
    LED_ON(); // Use break point here

    while (curLine < 5)
    {
      int len = uart_read(1, buf);
      if (len > 0)
      {
        lines[curLine][nextIn++] = buf[0];
        if (buf[0] == '\n')
        {
          lengths[ curLine++ ] = nextIn;
          nextIn = 0;
        }
      }
    }

    sd_open((unsigned char*)"file.txt"); // sets SD driver to write to "file.txt"
    if ( ! sd_error() )
    {
      sd_write(HEADER_LEN, (unsigned char*)header_str);
      for ( i = 0; i < 5; i++ )
      {
        sd_write( lengths[i], lines[i] );
      }
      sd_write(FOOTER_LEN, (unsigned char*)footer_str);
    }
    sd_close();

    if ( sd_error() )
    { // if there was an error, twinkle once
      LED_OFF();
      for ( i = 0; i < 10; i++ )
        wait(50);
      LED_ON();
      for ( i = 0; i < 10; i++ )
        wait(50);
    }
    LED_OFF();
//  }
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
