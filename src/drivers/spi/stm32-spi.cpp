#include <drivers/spi/stm32-spi.h>
#include <asm/spi.h>
#include <asm/rcc.h>
//#include <asm/chip.h>

#include <systime/systime.h>
#include <errno.h>

int drivers::stm32::spi_driver::enable(bool en)
{
	#ifdef CHIP_STM32L432XX
	stm32l4_spi_begin(regs);
	#else
	BUG();
	#endif
}

int drivers::stm32::spi_driver::begin()
{
	//return enable(true);
}

/*int genos::stm32::spi::select(void *slct, int en)
{
	//return 0;
}*/

int drivers::stm32::spi_driver::exbyte(int byte)
{
//	dprln("exbyte");
	uint8_t b = byte;
	uint8_t rb = 0;
	//dprln("send", b);
	exchange(&b, &rb, 1);
	//dprln("recv", b);
	return rb;
}

// rxbuf can be nullptr
int drivers::stm32::spi_driver::exchange(
    const void *txbuf, void *rxbuf, int len, char dummy)
{
//	dprln("exchange");
	#ifdef CHIP_STM32L432XX
	stm32l4_spi_exchange(regs,
	                   (const uint8_t*)txbuf, (uint8_t*)rxbuf,
	                   len, dummy);
	#else
	BUG();
	#endif
	return 0;
}

int drivers::stm32::spi_driver::setfrequency(uint32_t freq)
{
	return -ENOTSUP;
}

int drivers::stm32::spi_driver::set_divider(int div) 
{
	#ifdef CHIP_STM32L4XXX
	return stm32l4_spi_set_divider(regs, div);
	#else
	BUG();
	#endif
}
