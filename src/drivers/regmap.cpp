#include <drivers/regmap.h>
#include <drivers/spi/spi_client.h>
#include <drivers/i2c/i2c_client.h>
#include <drivers/spi/sysfs_spi_client.h>
#include <drivers/i2c/sysfs_i2c_client.h>

#include <igris/util/bug.h>

void genos::regmap::init_spi(genos::spi_client * client)
{
	bustype = regmap_bustype::SPI;
	dev = client;
	r_ops = &regmap_ops_spi;
}

void genos::regmap::init_i2c(genos::i2c_client * client)
{
	bustype = regmap_bustype::I2C;
	dev = client;
	r_ops = &regmap_ops_i2c;
}

void genos::regmap::init_sysfs_spi(genos::sysfs_spi_client * client)
{
	bustype = regmap_bustype::SYSFS_SPI;
	dev = client;
	r_ops = &regmap_ops_sysfs_spi;
}

void genos::regmap::init_sysfs_i2c(genos::sysfs_i2c_client * client)
{
	bustype = regmap_bustype::SYSFS_I2C;
	dev = client;
	r_ops = &regmap_ops_sysfs_i2c;
}

int spi_writereg(genos::device * dev,
                 unsigned int reg,
                 unsigned int val)
{
	BUG();
	return 0;
}

int i2c_writereg(genos::device * dev,
                 unsigned int reg,
                 unsigned int val)
{
	BUG();
	return 0;
}

int sysfs_spi_writereg(genos::device * dev,
                 unsigned int reg,
                 unsigned int val)
{
	BUG();
	return 0;
}

int sysfs_i2c_writereg(genos::device * dev,
                 unsigned int reg,
                 unsigned int val)
{
	BUG();
	return 0;
}


int spi_readreg(genos::device * dev,
                 unsigned int reg,
                 unsigned int * val)
{
	BUG();
	return 0;
}

int i2c_readreg(genos::device * dev,
                 unsigned int reg,
                 unsigned int * val)
{
	BUG();
	return 0;
}

int sysfs_spi_readreg(genos::device * dev,
                 unsigned int reg,
                 unsigned int * val)
{
	BUG();
	return 0;
}

int sysfs_i2c_readreg(genos::device * dev,
                 unsigned int reg,
                 unsigned int * val)
{
	BUG();
	return 0;
}

const genos::regmap_operations regmap_ops_spi =
{
	.writereg = spi_writereg,
	.readreg  = spi_readreg
	.write = nullptr,
	.read = nullptr
};

const genos::regmap_operations regmap_ops_i2c =
{
	.writereg = i2c_writereg,
	.readreg  = i2c_readreg
	.write = nullptr,
	.read = nullptr
};

const genos::regmap_operations regmap_ops_sysfs_spi =
{
	.writereg = sysfs_spi_writereg,
	.readreg  = sysfs_spi_readreg
	.write = nullptr,
	.read = nullptr
};

const genos::regmap_operations regmap_ops_sysfs_i2c =
{
	.writereg = sysfs_i2c_writereg,
	.readreg  = sysfs_i2c_readreg,
	.write = nullptr,
	.read = nullptr
};