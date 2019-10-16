#include <drivers/spi/stm32-spi.h>
#include <asm/spi.h>
#include <asm/rcc.h>

#include <errno.h>

int drivers::stm32::spi_driver::enable(bool en)
{
	rcc_enable_spi(regs);
	stm32_spi_enable(regs, en);
	return 0;
}

/*int genos::stm32::spi::select(void *slct, int en)
{
	//return 0;
}*/

int drivers::stm32::spi_driver::exbyte(int byte)
{
	return 0;
}

// rxbuf can be nullptr
int drivers::stm32::spi_driver::exchange(
    const void *txbuf, void *rxbuf, int len, char dummy)
{
	stm32_spi_exchange(regs,
	                   (const uint8_t*)txbuf, (uint8_t*)rxbuf,
	                   len, dummy);
}

int drivers::stm32::spi_driver::setfrequency(uint32_t freq)
{
	return -ENOTSUP;
}

int drivers::stm32::spi_driver::set_divider(int div) 
{
	return stm32_spi_set_divider(regs, div);
}
