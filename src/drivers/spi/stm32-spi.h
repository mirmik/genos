#ifndef GENOS_DRIVERS_STM32_SPI_H
#define GENOS_DRIVERS_STM32_SPI_H

#include <stdint.h>

#include <asm/stm32_spi.h>
#include <drivers/spi/spi.h>
#include <periph/map.h>

namespace genos
{
	class stm32_spi : public genos::spi
	{
		SPI_TypeDef * regs;
		int irqno;

	public:
		stm32_spi(SPI_TypeDef * regs, int irqno) : regs(regs), irqno(irqno) {}

		int begin();
		int enable 			(bool en = true) override;
		//int select   		(void *slct, int en) override;
		int exbyte 			(int byte) override;
		int exchange 		(const void *txbuf, void *rxbuf, int len) override;
		int setfrequency	(uint32_t freq) override;

		int set_divider(int divider);


		void irq_handler();
		void init_irq();
	};
}

#endif