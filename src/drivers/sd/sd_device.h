#ifndef GENOS_DRIVERS_SD_DEVICE_H
#define GENOS_DRIVERS_SD_DEVICE_H

#include <drivers/spi_client.h>

namespace genos 
{
	class sd_device 
	{
		spi_client * client;
	
	public:
		void begin(spi_client * client) 
		{
			this->client = client; 
		}


	};
}

#endif