/*
 * gps.c
 *
 *  GPS driver handling the GPS module
 *  Created on: Dec 3, 2011
 */

#include <msp430f2618.h>

#include "../hw_conf.h"

#include "gps.h"
#include "uart.h"
#include "timer.h"

#define SETUP_STR_LEN 29
#define NUM_SETUP_STRS 6

const char gpsInitStrs[NUM_SETUP_STRS][SETUP_STR_LEN + 1] = { // +1 is for the NULL at the end
    "$PUBX,40,RMC,0,5,0,0,0,0*42\r\n",
    "$PUBX,40,VTG,0,0,0,0,0,0*5E\r\n",
    "$PUBX,40,GGA,0,0,0,0,0,0*5A\r\n",
    "$PUBX,40,GSA,0,0,0,0,0,0*4E\r\n",
    "$PUBX,40,GSV,0,0,0,0,0,0*59\r\n",
    "$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n",
};

void gps_init(void)
{
  int i;

  // H/W setup
  GPS_POWER_DIR |= GPS_POWER_BIT;
  GPS_ON();
//  GPS_OFF();

  for ( i = 0; i < 20; i++ ) // wait 1 sec
    wait(50);

  for ( i = 0; i < NUM_SETUP_STRS; i++ )
  {
    uart_send( SETUP_STR_LEN, (char*)(gpsInitStrs[i]) );
    wait(50);
  }

  for ( i = 0; i < 20; i++ ) // wait 1 sec
    wait(50);

  while ( ! uart_is_tx_clear() );    // wait until setup finish sending
  while ( uart_read(1, (char*)i)  ); // clear read buffer

}

// utils for parse_line
char parse_time ( char *, TimeType*   );
char parse_coord( char *, CoordType*  );
char parse_date ( char *, DateType*   );

#define is_digit(c) (((c)<='9')&&((c)>='0'))

// definitions and constants
#define GPS_LINE_HEADER_LEN 7
const char * gps_line_header = "$GPRMC,";

StatusType parse_line(char* line, GPSDataType* data)
{
  int i;
  StatusType read_status;
  
//  line = "$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57";
  
  // init value
  data->status = GPS_INVALID;
  
  // header
  for ( i = 0; i < GPS_LINE_HEADER_LEN; i++ )
    if ( *(line++) != gps_line_header[i] )
      return GPS_INVALID;
  
  // time
  if ( ! parse_time( line, &(data->time) ) )
      return GPS_INVALID;
  // skip to after the next ','
  while ( (*line != '\0') && (*(line++) != ',') );
  if ( *line == '\0' ) 
    return GPS_INVALID;
  
  // status
  if ( *(line++) == 'A' )
    read_status = GPS_VALID;
  else
    return GPS_INVALID;
  // skip to after the next ','
  if ( *(line++) != ',' )
    return GPS_INVALID;
  
  // latitude
  if ( ! parse_coord(line, &(data->latitude) ) )
    return GPS_INVALID;
  // skip to after the next ','
  while ( (*line != '\0') && (*(line++) != ',') );
  if ( *line == '\0' ) 
    return GPS_INVALID;
  if ( *(line++) != 'N' )
    return GPS_INVALID;
  // skip to after the next ','
  if ( *(line++) != ',' )
    return GPS_INVALID;
  
  // longitude
  if ( ! parse_coord(line, &(data->longitude) ) )
    return GPS_INVALID;
  // skip to after the next ','
  while ( (*line != '\0') && (*(line++) != ',') );
  if ( *line == '\0' ) 
    return GPS_INVALID;
  if ( *(line++) != 'E' )
    return GPS_INVALID;
  // skip to after the next ','
  if ( *(line++) != ',' )
    return GPS_INVALID;

  // skip to after the next two ',' (ignore speed and course)
  for ( i = 0; i < 2; i++ )
  {
    while ( (*line != '\0') && (*(line++) != ',') );
    if ( *line == '\0' ) 
      return GPS_INVALID;
  }
  
  // date
  if ( ! parse_date( line, &(data->date) ) )
    return GPS_INVALID;
  
  
  // possible: check checksum and/or use 'mode'
  
  
  // final value
  data->status = read_status;
  return read_status;
}

char parse_time( char *str, TimeType* time )
{
  time->hour = (*(str++) - '0')*10;
  if ( time->hour > 20 ) 
    return 0;
  time->hour += (*(str++) - '0');
  if ( time->hour > 23 ) 
    return 0;
  
  time->minutes = (*(str++) - '0')*10;
  if ( time->minutes > 50 ) 
    return 0;
  time->minutes += (*(str++) - '0');
  if ( time->minutes > 59 ) 
    return 0;
  
  time->seconds = (*(str++) - '0')*10;
  if ( time->seconds > 50 ) 
    return 0;
  time->seconds += (*(str++) - '0');
  if ( time->seconds > 59 ) 
    return 0;

  return 1;
}

char parse_coord( char *str, CoordType* coord )
{
  // coordinates are in format ddmm.mmmm or dddmm.mmmm
  
  char* p_dot = str+2;
  double minutes = 0;
  double minute_frac = 0, minute_den = 1.0;
  *coord = 0;
  
  // parse full degrees ( str is on "dd" or "ddd" )
  while ( ( *p_dot     != '\0' ) && 
          ( *str       != '\0' ) &&
          ( *(p_dot++) !=  '.' )    )
  {
    char c = *(str++);
    if ( ! is_digit(c) )
      return 0;
    *coord *= 10.0;
    *coord += (c - '0');
  }
  if ( ( *p_dot == '\0' ) || ( *str == '\0' ) )
    return 0;
  
  // parse full minutes ( str is on "mm" )
  while ( is_digit(*str) && ( str < p_dot ) )
  {
    char c = *(str++);
    minutes *= 10;
    minutes += (c - '0');
  }
  if ( *str == '\0' )
    return 0;
  
  // skip dot
  str++;
  
  // parse minute fracture ( str is on "mmmm" )
  while ( ( *str != '\0' ) && ( *str != ',' ) )
  {
    char c = *(str++);
    if ( ! is_digit(c) )
      return 0;
    minute_frac *= 10.0;
    minute_frac += (c - '0');
    minute_den *= 10.0;
  }
  if ( *str == '\0' )
    return 0;

  minutes += (minute_frac / minute_den);
  *coord += (minutes / 60.0);
  
  return 1;  
}

char parse_date ( char *str, DateType *date )
{
  date->day = (*(str++) - '0')*10;
  if ( date->day > 30 ) 
    return 0;
  date->day += (*(str++) - '0');
  if ( date->day > 31 ) 
    return 0;

  date->month = (*(str++) - '0')*10;
  if ( date->month > 10 ) 
    return 0;
  date->month += (*(str++) - '0');
  if ( date->month > 12 ) 
    return 0;

  date->year = (*(str++) - '0')*10;
  if ( date->year > 90 ) 
    return 0;
  date->year += (*(str++) - '0');
  if ( date->year > 99 ) 
    return 0;
  date->year += 2000;
  
  return 1;
}