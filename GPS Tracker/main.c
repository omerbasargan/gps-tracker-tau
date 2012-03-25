/*
 * main.c
 */
#include <msp430.h>
#include <msp430f2618.h>

#include "hw_conf.h"
#include "drivers/timer.h"
#include "drivers/uart.h"
#include "drivers/gps.h"
#include "drivers/fs/ff.h"

void hw_setup(void);

// TODO remove this!
unsigned char lines[5][128];
unsigned char lengths[5];

#define HEADER_LEN 14
#define FOOTER_LEN 12
#define die(x) ((void)0)
const char header_str[HEADER_LEN] = "Start of file\n";
const char footer_str[FOOTER_LEN] = "End of file\n";

unsigned long cardSize = 0;
FIL Fil;			/* File object */
BYTE Buff[128];		/* File read buffer */
FATFS Fatfs;		/* File system object */
FRESULT result;

void main(void)
{
  char buf[1];
  int nextIn = 0;
  int curLine = 0;
  int i;
  
	FRESULT rc;				/* Result code */
	DIR dir;				/* Directory object */
	FILINFO fno;			/* File information object */
	UINT bw, br;

  volatile unsigned long dummy;

  WDTCTL = WDTPW | WDTHOLD; // stop watch-dog timer

  hw_setup();
  uart_init();

  GPS_ON();
  
  gps_init();

  LED_ON(); // Use break point here

  while (curLine < 5)
  {
    int len = uart_read(1, buf);
    if (len > 0)
    {
      lines[curLine][nextIn++] = buf[0];
      if (buf[0] == '\n')
      {
        // Search for GPS Fix indicator
        if (IsGPSFix(lines[ curLine ] ))
        {
          lengths[ curLine++ ] = nextIn;
        }
        nextIn = 0;
      }
    }
  }
  
  LED_OFF();

  
  // Mount filesystem and register work area
  result = f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

	f_mount(0, &Fatfs);		/* Register volume work area (never fails) */
  
/*
	rc = f_open(&Fil, "TEST.TXT", FA_READ);
	if (rc) die(rc);

	for (;;) {
		rc = f_read(&Fil, Buff, sizeof Buff, &br);	// Read a chunk of file
		if (rc || !br) break;			// Error or end of file
	}
	if (rc) die(rc);

	rc = f_close(&Fil);
	if (rc) die(rc);
*/
  
	rc = f_open(&Fil, "GPS.TXT", FA_WRITE | FA_CREATE_ALWAYS);
	if (rc) die(rc);

  for (i = 0; i < 5; i++)
  {
  	rc = f_write(&Fil, lines[i], lengths[i], &bw);
  	if (rc) die(rc);
  }

	rc = f_close(&Fil);
	if (rc) die(rc);

  
/*
    // Init/Zero buffer
    for (i = 0; i < 512; i++) buffer[i] = 0xaa;
    
    sd_open((unsigned char*)"file.txt"); // sets SD driver to write to "file.txt"
    if ( ! sd_error() )
    {
      sd_write(HEADER_LEN, (unsigned char*)header_str);
      for ( i = 0; i < 100; i++ )
      {
        sd_write(512, buffer);
      }
      /*
      for ( i = 0; i < 5; i++ )
      {
        sd_write( lengths[i], lines[i] );
      }
      sd_write(FOOTER_LEN, (unsigned char*)footer_str);
     
    }
    sd_close();
*/
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
