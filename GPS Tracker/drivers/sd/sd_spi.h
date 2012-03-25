/*
 * sd_spi.h
 *
 *  This is a standard spi implementation used by the sd driver
 */

void sd_spi_init(void);
void sd_spi_write(unsigned char * bytes, unsigned int len);
unsigned char sd_spi_write_byte(unsigned char byte);
void sd_spi_read(unsigned char * buffer, unsigned int len);

