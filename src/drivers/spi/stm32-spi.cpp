#include <drivers/spi/stm32-spi.h>
#include <asm/spi.h>
#include <asm/rcc.h>

#include <systime/systime.h>
#include <errno.h>

int drivers::stm32::spi_driver::enable(bool en)
{
	rcc_enable_spi(regs);
	//stm32l4_spi_enable(regs, en);
delayMicroseconds(100000);

//	regs->CR2 = 0;
	//regs->CR2 = SPI_CR2_FRXTH;
//	regs->CR1 = SPI_CR1_SPE | SPI_CR1_MSTR | SPI_CR1_BR_2 | SPI_CR1_SSI | SPI_CR1_SSM;
regs->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_1 
	| SPI_CR1_BR_2 | SPI_CR1_BR_0 | SPI_CR1_SSM | SPI_CR1_SSI;
//regs->CR2 = SPI_CR2_SSOE;// |SPI_CR2_RXNEIE;
regs->CR2 = SPI_CR2_FRXTH;
regs->CR1 |= SPI_CR1_SPE;	
delayMicroseconds(100000);

	//delayMicroseconds(100000);

	return 0;
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
	stm32l4_spi_exchange(regs,
	                   (const uint8_t*)txbuf, (uint8_t*)rxbuf,
	                   len, dummy);

	return 0;
}

int drivers::stm32::spi_driver::setfrequency(uint32_t freq)
{
	return -ENOTSUP;
}

int drivers::stm32::spi_driver::set_divider(int div) 
{
	return stm32l4_spi_set_divider(regs, div);
}
