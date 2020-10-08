#ifndef GENOS_DRIVERS_STM32_SPI_H
#define GENOS_DRIVERS_STM32_SPI_H

#include <stdint.h>

#include <asm/stm32_spi.h>
#include <drivers/spi/spi_device.h>
#include <periph/map.h>

#include <igris/sync/semaphore.h>

namespace genos
{
	class stm32_spi_device : public genos::spi_device
	{
		SPI_TypeDef * regs;
		int irqno;

		struct semaphore __lock = SEMAPHORE_INIT(__lock, 1);

	public:
		stm32_spi_device(SPI_TypeDef * regs, int irqno) 
			: regs(regs), irqno(irqno) {}

		device * parent_device() override { return nullptr; }

		int begin();
		//int enable 			(bool en = true) override;
		//int select   		(void *slct, int en) override;
		//int exbyte 			(int byte) override;
		int exchange (
		    const void *txbuf, void *rxbuf, int len) override;
		int setfrequency	(uint32_t freq) override;

		int set_divider(int divider);

		static void _irq_handler(void * arg);
		void init_irq_handler();

		void lock_bus() override { semaphore_down(&__lock); }
		void unlock_bus() override { semaphore_up(&__lock); }
	};
}

#endif