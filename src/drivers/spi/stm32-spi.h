#ifndef GENOS_DRIVERS_STM32_SPI_H
#define GENOS_DRIVERS_STM32_SPI_H

#include <stdint.h>

#include <asm/spi.h>
#include <drivers/spi/spi.h>
#include <periph/map.h>

namespace drivers
{
	namespace stm32
	{
		class spi_driver : public drivers::spi_driver
		{
			spi_regs_t * regs;
			int irqno;

		public:
			spi_driver(spi_regs_t * regs, int irqno) : regs(regs), irqno(irqno) {}

			int begin();
			int enable 			(bool en = true) override;
			//int select   		(void *slct, int en) override;
			int exbyte 			(int byte) override;
			int exchange 		(const void *txbuf, void *rxbuf, int len, char dummy = '\0') override;
			int setfrequency	(uint32_t freq) override;

			int set_divider(int divider);
		};
	}
}

#endif