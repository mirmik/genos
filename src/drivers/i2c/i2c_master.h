#ifndef GENOS_I2C_H
#define GENOS_I2C_H

#include <igris/osinter/sem.h>

namespace genos
{
	// linux compatible
	class i2c_master : genos::device
	{
		struct semaphore semlock = SEMAPHORE_INIT(semlock);

	public:
		virtual int set_slave_address(uint8_t addr) = 0;

		virtual int write(const void* out, uint16_t olen) = 0;

		// combined transaction. it mixing reading and writing.
		virtual int read(void* in, uint16_t ilen) = 0;

		virtual int set_baudrate(int rate) = 0;

		void lock_bus() { sema_down(&semlock); }
		void unlock_bus() { sema_up(&semlock); }
	};
}

#endif
