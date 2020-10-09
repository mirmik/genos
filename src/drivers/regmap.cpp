#include <drivers/regmap.h>
#include <drivers/spi/spi_client.h>
#include <drivers/i2c/i2c_client.h>
#include <drivers/spi/sysfs_spi_client.h>
#include <drivers/i2c/sysfs_i2c_client.h>

#include <igris/util/bug.h>

int spi_writereg_group(genos::device * dev,
                 unsigned int reg,
                 uint8_t * val, 
                 int len)
{
	genos::spi_client * client = (genos::spi_client *) dev;

	uint8_t tx_data[len + 1];
	uint8_t rx_data[len + 1];

	tx_data[0] = reg;
	memcpy(tx_data + 1, val, len);

	client -> exchange(tx_data, rx_data, len + 1);
	return 0;
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

int spi_lock(genos::device * dev)
{
	((genos::spi_client *) dev)->lock_bus();
	return 0;
}

int i2c_lock(genos::device * dev)
{
	BUG();
	return 0;
}

int sysfs_spi_lock(genos::device * dev)
{
	BUG();
	return 0;
}

int sysfs_i2c_lock(genos::device * dev)
{
	BUG();
	return 0;
}

int spi_unlock(genos::device * dev)
{
	((genos::spi_client *) dev)->unlock_bus();
	return 0;
}

int i2c_unlock(genos::device * dev)
{
	BUG();
	return 0;
}

int sysfs_spi_unlock(genos::device * dev)
{
	BUG();
	return 0;
}

int sysfs_i2c_unlock(genos::device * dev)
{
	BUG();
	return 0;
}


const genos::regmap_operations regmap_ops_spi =
{
	.writereg = spi_writereg,
	.readreg  = spi_readreg,
	.write = spi_writereg_group,
	.read = spi_writereg_group,
	.lock = spi_lock,
	.unlock = spi_unlock
};

const genos::regmap_operations regmap_ops_i2c =
{
	.writereg = i2c_writereg,
	.readreg  = i2c_readreg,
	.write = nullptr,
	.read = nullptr,
	.lock = i2c_lock,
	.unlock = i2c_unlock
};

const genos::regmap_operations regmap_ops_sysfs_spi =
{
	.writereg = sysfs_spi_writereg,
	.readreg  = sysfs_spi_readreg,
	.write = nullptr,
	.read = nullptr,
	.lock = sysfs_spi_lock,
	.unlock = sysfs_spi_unlock
};

const genos::regmap_operations regmap_ops_sysfs_i2c =
{
	.writereg = sysfs_i2c_writereg,
	.readreg  = sysfs_i2c_readreg,
	.write = nullptr,
	.read = nullptr,
	.lock = sysfs_i2c_lock,
	.unlock = sysfs_i2c_unlock
};

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
