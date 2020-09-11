#include <drivers/regmap.h>

uint32_t genos::regmap::init_spi(genos::spi_client * client)
{
	bustype = regmap_bustype::SPI;
	dev = client;
}

uint32_t genos::regmap::init_i2c(genos::i2c_client * client)
{
	bustype = regmap_bustype::I2C;
	dev = client;
}
