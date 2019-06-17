#include <drivers/spi/stm32-spi.h>
#include <asm/spi.h>
#include <asm/rcc.h>

#include <errno.h>

int stm32::spi::enable(bool en) 
{
	rcc_enable_spi(regs);
	stm32_spi_enable(regs, en);
	return 0;
}

int stm32::spi::select(void *slct, int en) 
{
	return 0;
}

int stm32::spi::exbyte(int byte) 
{
	return 0;
}

int stm32::spi::exchange(const void *txbuf, void *rxbuf, int len) 
{
	return 0;
}

int stm32::spi::setfrequency(uint32_t freq) 
{
	return -ENOTSUP;
}
