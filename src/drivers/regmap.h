#ifndef GENOS_REGMAP_H
#define GENOS_REGMAP_H

#include <genos/device.h>

/*
	Register map acess API
*/

namespace genos
{
	class i2c_client;
	class spi_client;

	enum class regmap_bustype : uint8_t 
	{
		SPI,
		I2C
	};

	struct regmap_operations 
	{
		int (*writereg)(genos::device *, unsigned int reg, unsigned int val);
		int (*readreg) (genos::device *, unsigned int reg, unsigned int * val);
	};

	class regmap
	{
		genos::device * dev; // Устройство, через которое осуществляется доступ
		regmap_bustype bustype;
		const regmap_operations * r_ops;

		void init_spi(genos::spi_client * spi);
		void init_i2c(genos::i2c_client * i2c);

		int writereg(unsigned int reg, unsigned int val) 
		{
			return r_ops->writereg(dev, reg, val);
		}

		int readreg (unsigned int reg, unsigned int * val) 
		{
			return r_ops->readreg(dev, reg, val);
		}
	};
}

#endif