#ifndef DRIVERS_STM32_I2C_H
#define DRIVERS_STM32_I2C_H

#include <drivers/i2c/i2c_device.h>
#include <periph/map.h>

#include <igris/sync/semaphore.h>

namespace genos {
	class stm32_i2c_device : public genos::i2c_device
	{
		I2C_TypeDef * regs;
		int irqno_event;
		int irqno_error;

		struct semaphore __lock = SEMAPHORE_INIT(__lock, 1);

	public:
		stm32_i2c_device(
			I2C_TypeDef * regs, 
			int irqno_event,
			int irqno_error) 
				: 
		regs(regs), 
		irqno_event(irqno_event),
		irqno_error(irqno_error) {}

		device * parent_device() override { return nullptr; }

		int exchange(uint8_t addr, 
			const void * txdata, int txsize,
			void * rxdata, int rxsize) override;
	
		void set_slave_address(uint8_t addr) override;

		void lock_bus() override { semaphore_down(&__lock); }
		void unlock_bus() override { semaphore_up(&__lock); }
	};
}

#endif