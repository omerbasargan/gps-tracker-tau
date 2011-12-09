/*
 * timer.c
 *
 *  Created on: Dec 3, 2011
 */

void wait(int mili)
{
  // TODO real wait with timer
  volatile unsigned long dummy = mili*12000;
  do { dummy--; } while( dummy > 0 );
}
