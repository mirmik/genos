#include <drivers/bdev/atmel_dataflash.h>
#include <gxx/debug/dprint.h>

uint8_t atmel_dataflash_device::get_status() 
{
	uint8_t status;

	lock();

	status = spi->exbyte(StatusReg);
	status = spi->exbyte(0x00);
	
	unlock();

	return status;
}