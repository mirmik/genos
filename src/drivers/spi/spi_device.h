#ifndef GENOS_SPI_DEVICE_H
#define GENOS_SPI_DEVICE_H

#include <drivers/device.h>

namespace genos 
{
	class spi_device : public genos::device
	{
	public:
		virtual int exchange (
			const void *txbuf, void *rxbuf, int len) = 0;

		virtual int setfrequency(uint32_t freq) = 0;

		virtual void lock_bus() = 0;
		virtual void unlock_bus() = 0;
	};
}

#endif