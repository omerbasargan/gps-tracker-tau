/*
 * sd.h
 *
 *  Created on: Dec 30, 2011
 *      Author: aviv
 */

#ifndef SD_H_
#define SD_H_

typedef unsigned char * sd_str_t; // NULL terminated string
typedef unsigned char * sd_byte_arr_t; // bytes buffer

typedef enum { SD_OK = 0,
               SD_NO_CARD,
               SD_WRITE_FAILED,
               SDNumOfErrorTypes } SDErrorType;


void sd_init(void);

// Sets the current file to filename (overwrites if necessary)
void sd_open(sd_str_t filename);

// Close the currently open file
// This is a blocking function - it returns only when all changes
// were written to the card
void sd_close(void);

// Returns an error code if an error occurred during sd operations
char sd_error(void);

// Writes a byte array to the card
void sd_write(int len, sd_byte_arr_t bytes);

#endif /* SD_H_ */
