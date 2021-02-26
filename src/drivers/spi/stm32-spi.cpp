#include <hal/irqtable.h>

#include <drivers/spi/stm32-spi.h>
#include <asm/stm32_spi.h>
#include <asm/stm32_rcc.h>
//#include <asm/chip.h>

#include <util/cpu_delay.h>

#include <systime/systime.h>
#include <errno.h>

int genos::stm32_spi_device::begin_master()
{
	stm32_spi_rcc_enable(regs, true);
	cpu_delay(1000);

	stm32_spi_set_master_mode(regs, true);
	stm32_spi_set_divider(regs, 16);

	stm32_spi_disable_nss(regs, true);
	stm32_spi_set_mode(regs, false, false);

	stm32_spi_enable(regs, true);

	cpu_delay(1000);

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

#if SPI_DEBUG >= 1
	int l = len;
	uint8_t * txbuf2 = (uint8_t*) txbuf_v;
	uint8_t * rxbuf2 = (uint8_t*) rxbuf_v;
#endif

	while (len--)
	{
		stm32_spi_wait_for_tx_empty(regs);
		stm32_spi_send_byte(regs, *txbuf++);
		stm32_spi_wait_for_rx_not_empty(regs);

		c = stm32_spi_recv_byte(regs);
		if (rxbuf)
		{
			*rxbuf++ = c;
		}
	}

#if SPI_DEBUG>=1
	dpr("send: ");
	for (int i = 0; i < l; ++i)
	{
		dprhex(txbuf2[i]);
	}

	dpr(" recv: ");
	for (int i = 0; i < l; ++i)
	{
		dprhex(rxbuf2[i]);
	}
	dln();
#endif

	// Wait until the transmission of the last byte is complete
	stm32_spi_wait_until_busy(regs);

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