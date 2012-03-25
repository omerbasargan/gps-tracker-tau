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
    "$PUBX,40,RMC,0,0,0,0,0,0*47\r\n",
    "$PUBX,40,VTG,0,0,0,0,0,0*5E\r\n",
    "$PUBX,40,GGA,0,0,0,0,0,0*5A\r\n",
    "$PUBX,40,GSA,0,0,0,0,0,0*4E\r\n",
    "$PUBX,40,GSV,0,0,0,0,0,0*59\r\n",
    "$PUBX,40,GLL,0,5,0,0,0,0*59\r\n",
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

char IsGPSFix(char* line)
{
  char* nextcomma = line;
  int commacounter = 0;

  // Parse GLL Message
  while ((*nextcomma != 0) && (*nextcomma != '\n'))
  {
    if (*nextcomma == ',')
      commacounter++;
    
    // check for 7th GLL field
    if (commacounter == 6)
    {
      if (nextcomma[1] == 'A') // We found GPS fix marker
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
      
    nextcomma++;
  }

  return 0;
}