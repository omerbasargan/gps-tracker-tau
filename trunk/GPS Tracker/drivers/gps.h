/*
 * gps.h
 *
 *  Created on: Dec 3, 2011
 *      Author: aviv
 */

#ifndef GPS_H_
#define GPS_H_


typedef enum { GPS_VALID, GPS_INVALID } StatusType;
typedef double CoordType;

typedef struct {
  unsigned char hour;
  unsigned char minutes;
  unsigned char seconds;
} TimeType;

typedef struct {
  unsigned char day;
  unsigned char month;
  unsigned int year;
} DateType;

typedef struct {
  StatusType status;
  CoordType latitude;
  CoordType longitude;
  TimeType time;
  DateType date;
  char mode;
} GPSDataType ;


void gps_init(void);
StatusType parse_line(char* line, GPSDataType* data);

#endif /* GPS_H_ */
