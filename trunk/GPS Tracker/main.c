/*
 * main.c
 */
#include <msp430.h>
#include <msp430f2618.h>
#include <string.h>
#include <stdio.h>

#include "hw_conf.h"
#include "drivers/timer.h"
#include "drivers/uart.h"
#include "drivers/gps.h"
#include "drivers/fs/ff.h"

void hw_setup(void);

// TODO remove this!
char line[128];

#define MAX_SAMPLES ((unsigned int)-1)
#define die(x) __no_operation()

const char* KML_Header = "<?xml version=\"1.0\"?><kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\"><Document><name>Project.kml</name><open>1</open><Placemark><name>course</name><Style><LineStyle><color>ff0000ff</color><width>3</width></LineStyle></Style><LineString><coordinates>";
const char* KML_Footer = "</coordinates></LineString></Placemark></Document></kml>";

unsigned long cardSize = 0;
FIL Fil;			/* File object */
BYTE Buff[128];		/* File read buffer */
FATFS Fatfs;		/* File system object */
FRESULT result;

UINT spool_coords( CoordType latitude, CoordType longitude, char* str );
void update_fattime(int year, int month, int day, int hour, int minutes, int seconds);
void record_button_init(void);

void main(void)
{
  char buf[1];
  int nextIn = 0;
  UINT len, bw;
  char bFileCreated = 0;
  unsigned int sampleCounter = 0;
  
  WDTCTL = WDTPW | WDTHOLD; // stop watch-dog timer

  hw_setup();
  uart_init();
  record_button_init();

  GPS_OFF();
  
  // Mount filesystem and register work area
  result = f_mount(0, &Fatfs);		/* Register volume work area (never fails) */
	if ( result != FR_OK ) die();
  
  

  while(1)
  {
    // if waking up in record on state do not go to sleep
    if ( ! is_record_on() )
      LPM0;
    LED_ON();
    gps_init();
    nextIn = 0;
    while ( is_record_on() )
    {
      len = uart_read(1, buf);
      if (len > 0)
      {
        line[nextIn++] = buf[0];
        if (buf[0] == '\n')
        {
          GPSDataType data;
          
          if ( ( parse_line(line, &data) == GPS_VALID ) && ( sampleCounter <= MAX_SAMPLES ) )
          {
            LED_ON();
  
            // update FS time
            update_fattime(data.date.year, data.date.month, data.date.day, 
                           data.time.hour, data.time.minutes, data.time.seconds);
  
            if ( ! bFileCreated )
            {
              sprintf(line, "%02d%02d%02d%02d.kml", data.date.month, data.date.day,
                      data.time.hour, data.time.minutes);
              result = f_open( &Fil, line, FA_WRITE | FA_CREATE_ALWAYS );
              if ( result != FR_OK ) 
                die();
              result = f_write( &Fil, KML_Header, strlen(KML_Header), &bw );
              if ( ( result != FR_OK ) || ( bw < len ) ) die();
              bFileCreated = 1;
            }
            
            len = spool_coords( data.latitude, data.longitude, line );
            result = f_write( &Fil, line, len, &bw );
            if ( ( result != FR_OK ) || ( bw < len ) ) die();
            
            sampleCounter++;
            LED_OFF();
          }
          nextIn = 0;
        }
      }
    }
    GPS_OFF();
    LED_OFF();
    if ( bFileCreated )
    {
      result = f_write( &Fil, KML_Footer, strlen(KML_Footer), &bw );
      if ( ( result != FR_OK ) || ( bw < len ) ) 
        die();
      result = f_close(&Fil);
      if ( result != FR_OK ) 
        die();
    }
    bFileCreated = 0;
  }
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
  return sprintf(str, "%.14lf,%.14lf,0 \r\n",longitude ,latitude);
}

void record_button_init(void)
{
  BUTTON_REN |= BUTTON_BIT;
  BUTTON_OUT |= BUTTON_BIT;
  BUTTON_IE  |= BUTTON_BIT;
  BUTTON_IES |= BUTTON_BIT;
}

#pragma vector=PORT1_VECTOR
__interrupt void ButtonISR(void)
{
  BUTTON_IFG &= ~BUTTON_BIT;
  LPM0_EXIT;
}
