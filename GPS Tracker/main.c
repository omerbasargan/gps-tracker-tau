/*
 * main.c
 */
#include <msp430.h>
#include <msp430f2618.h>

#include <stdio.h>

#include "hw_conf.h"
#include "drivers/timer.h"
#include "drivers/uart.h"
#include "drivers/gps.h"
#include "drivers/fs/ff.h"

void hw_setup(void);

// TODO remove this!
char line[128];

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

UINT spool_coords( CoordType latitude, CoordType longitude, char* str );

void main(void)
{
  char buf[1];
  int nextIn = 0;
  UINT len, bw;
  char bFileCreated = 0;
  
  WDTCTL = WDTPW | WDTHOLD; // stop watch-dog timer

  hw_setup();
  uart_init();
  gps_init();

  // Mount filesystem and register work area
  result = f_mount(0, &Fatfs);		/* Register volume work area (never fails) */
	if ( result != FR_OK ) die();

  LED_ON(); 

  while(1)
  {
    len = uart_read(1, buf);
    if (len > 0)
    {
      line[nextIn++] = buf[0];
      if (buf[0] == '\n')
      {
        GPSDataType data;
        
        if ( parse_line(line, &data) == GPS_VALID )
        {
          LED_ON();
            
          len = spool_coords( data.latitude, data.longitude, line );
          if ( ! bFileCreated )
          {
            result = f_open( &Fil, "GPS.TXT", FA_WRITE | FA_CREATE_ALWAYS );
            if ( result != FR_OK ) die();
            bFileCreated = 1;
          }
          
          result = f_write( &Fil, line, len, &bw );
          if ( ( result != FR_OK ) || ( bw < len ) ) die();
          
          LED_OFF();
        }
        else if ( bFileCreated )
        {
          GPS_OFF();
          result = f_close(&Fil);
          if ( result != FR_OK ) die();
          break;
        }
        nextIn = 0;
      }
    }
    else
    { // nothing to read
      // sleep a while
    }
  }
  
  __disable_interrupt();
  LPM4;
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

  DCOCTL  = 0xA4; // DCO Setup (about 12MHz)
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

UINT spool_coords( CoordType latitude, CoordType longitude, char* str )
{
  return sprintf(str, "%lf, %lf, 0\n", latitude,longitude);
}