#include <hal/irqtable.h>

#include <drivers/spi/stm32-spi.h>
#include <asm/stm32_spi.h>
#include <asm/stm32_rcc.h>
//#include <asm/chip.h>

#include <systime/systime.h>
#include <errno.h>

/*int genos::stm32_spi_device::enable(bool en)
{
#ifdef CHIP_STM32L432XX
	stm32l4_spi_begin(regs);
#else
	BUG();
#endif
}
*/
int genos::stm32_spi_device::begin()
{
	//return enable(true);
	return 0;
}

/*int genos::stm32::spi::select(void *slct, int en)
{
	//return 0;
}*/

/*int genos::stm32_spi_device::exbyte(int byte)
{
//	dprln("exbyte");
	uint8_t b = byte;
	uint8_t rb = 0;
	//dprln("send", b);
	exchange(&b, &rb, 1);
	//dprln("recv", b);
	return rb;
}
*/

// TODO: reimplement with interrupt
int genos::stm32_spi_device::exchange(
    const void *txbuf_v, void *rxbuf_v, int len)
{
	char c;
	uint8_t * txbuf = (uint8_t*) txbuf_v;
	uint8_t * rxbuf = (uint8_t*) rxbuf_v;

	while (len--)
	{
		while ((regs->SR & SPI_SR_RXNE)) {};
		while (!(regs->SR & SPI_SR_TXE)) {};

		stm32_spi_send_byte(regs, *txbuf++);

		while (!(regs->SR & SPI_SR_RXNE)) {};

		c = stm32_spi_recv_byte(regs);
		if (rxbuf)
		{
			*rxbuf++ = c;
		}
	}

	while (regs->SR & SPI_SR_BSY); // Wait until the transmission of the last byte is complete

	return 0;
}

int genos::stm32_spi_device::setfrequency(uint32_t freq)
{
	return -ENOTSUP;
}

int genos::stm32_spi_device::set_divider(int div)
{
	return stm32_spi_set_divider(regs, div);
}

void genos::stm32_spi_device::_irq_handler(void *privarg)
{
//	genos::stm32_spi_device * dev = (genos::stm32_spi_device *) privarg;

	//dev->regs->DR = ;
}

void genos::stm32_spi_device::init_irq_handler()
{
	irqtable_set_handler(
	    irqno,
	    &genos::stm32_spi_device::_irq_handler,
	    (void*) this);
}