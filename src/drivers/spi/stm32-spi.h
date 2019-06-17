#ifndef GENOS_DRIVERS_STM32_SPI_H
#define GENOS_DRIVERS_STM32_SPI_H

#include <stdint.h>

#include <drivers/spi/spi.h>
#include <periph/map.h>

namespace stm32 
{
	class spi : public genos::spi_driver
	{
		spi_regs * regs;
		int irqno;

	public:
		spi(spi_regs * regs, int irqno) : regs(regs), irqno(irqno) {}

		int enable 			(bool en = true) override;
		int select   		(void *slct, int en) override;
		int exbyte 			(int byte) override;
		int exchange 		(const void *txbuf, void *rxbuf, int len) override;
		int setfrequency	(uint32_t freq) override;

	};
}

#endif