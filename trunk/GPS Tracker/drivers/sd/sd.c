/*
 * sd.c
 *
 *  Driver for usage of an SD card.
 *  HW definitions are assumed to be in ../../hw_conf.h
 */

#include <msp430f2618.h>

#include "../../hw_conf.h"
#include "sd.h"
#include "sd_spi.c"
#include "mmc.c"

/*
 * For now ignore FS related stuff such as filenames etc. and only
 * write raw data directly to the sd card.
 */
// Buffer for complete block write
// TEMPORARY!!
#define SD_BLOCK_LEN 64
static unsigned char SD_Buff[SD_BLOCK_LEN];
static unsigned int SD_NextIn;
static unsigned int SD_NextAddress;
static void SpoolBlock(void);

static SDErrorType sd_error_;



void sd_init(void)
{
  // Chip Select / Slave select
  // - not using HW STE since it's timing does not fit sd uses
  SD_CS_DIR |= SD_CS_BIT; // set as output
  sd_clr_cs();

  // Card detect
  SD_CD_DIR &= ~SD_CD_BIT;

  sd_spi_init();

  if ( ! sd_card_detect() )
  {
    sd_error_ = SD_NO_CARD;
  }
  else
  {
    sd_error_ = SD_OK;
  }

  if ( ! sd_error() )
    mmcInit();

}

void sd_open(sd_str_t filename)
{
  // TODO

  // For now...
  SD_NextIn = 0; // TODO get rid of this
  SD_NextAddress = 0; // TODO get rid of this

}

void sd_close(void)
{
  // TODO

  // For now...
  if ( SD_NextIn > 0 )
  {
    while ( SD_NextIn < SD_BLOCK_LEN )
      SD_Buff[SD_NextIn++] = 0;
    SpoolBlock();
  }
}

char sd_error(void)
{
  return sd_error_;
}

void sd_write(int len, sd_byte_arr_t bytes)
{
  // TODO

  // for now...
  while ( len-- )
  {
    SD_Buff[SD_NextIn++] = *(bytes++);
    if ( SD_NextIn >= SD_BLOCK_LEN )
    {
      SpoolBlock();
      SD_NextIn = 0;
    }
  }
}

static void SpoolBlock(void)
{
  char res = mmcWriteBlock(SD_NextAddress, SD_BLOCK_LEN, SD_Buff);
  if ( res == MMC_SUCCESS )
  {
    SD_NextAddress++;
    sd_error_ = SD_OK;
  }
  else
  {
    sd_error_ = SD_WRITE_FAILED;
  }
}
