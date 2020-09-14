#ifndef GENOS_SPI_DEVICE_H
#define GENOS_SPI_DEVICE_H

namespace genos 
{
	class spi_device : public genos::device
	{
		virtual int exchange (
			const void *txbuf, void *rxbuf, int len) = 0;
	};
}

#endif