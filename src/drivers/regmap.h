#ifndef GENOS_REGMAP_H
#define GENOS_REGMAP_H

#include <drivers/device.h>
#include <stdint.h>

/*
	Register map acess API
*/

namespace genos
{
	class i2c_client;
	class spi_client;
	class sysfs_i2c_client;
	class sysfs_spi_client;

	enum class regmap_bustype : uint8_t 
	{
		SPI,
		I2C,
		SYSFS_SPI,
		SYSFS_I2C
	};

	struct regmap_operations
	{
		int (*writereg)(genos::device *, unsigned int reg, unsigned int val);
		int (*readreg) (genos::device *, unsigned int reg, unsigned int * val);
		int (*write)(genos::device *, unsigned int start_reg, uint8_t * val, int len);
		int (*read) (genos::device *, unsigned int start_reg, uint8_t * val, int len);
		int (*lock)(genos::device *);
		int (*unlock)(genos::device *);
	};

	class regmap
	{
		genos::device * dev; // Устройство, через которое осуществляется доступ
		regmap_bustype bustype;
		const regmap_operations * r_ops;

	public:
		regmap() {}
		regmap(genos::spi_client * spi) { init_spi(spi); }
		regmap(genos::i2c_client * i2c) { init_i2c(i2c); }

		void init_spi(genos::spi_client * spi);
		void init_i2c(genos::i2c_client * i2c);
		void init_sysfs_i2c(genos::sysfs_i2c_client * client);
		void init_sysfs_spi(genos::sysfs_spi_client * client);

		int writereg(unsigned int reg, unsigned int val) 
		{
			return r_ops->writereg(dev, reg, val);
		}

		int readreg (unsigned int reg, unsigned int * val) 
		{
			return r_ops->readreg(dev, reg, val);
		}

		int writereg_group(unsigned int start_reg, uint8_t * val, int len) 
		{
			return r_ops->write(dev, start_reg, val, len);
		}

		int readreg_group (unsigned int start_reg, uint8_t * val, int len) 
		{
			dprln("READREG GROUP");
			return r_ops->read(dev, start_reg, val, len);
		}

		int lock() { return r_ops->lock(dev); }
		int unlock() { return r_ops->unlock(dev); }
	};

	//extern const regmap_operations regmap_ops_spi;
	//extern const regmap_operations regmap_ops_i2c;
	//extern const regmap_operations regmap_ops_sysfs_spi;
	//extern const regmap_operations regmap_ops_sysfs_i2c;
}

#endif