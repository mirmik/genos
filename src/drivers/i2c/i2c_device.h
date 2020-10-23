#ifndef GENOS_DRIVERS_I2C_DEVICE_H
#define GENOS_DRIVERS_I2C_DEVICE_H

#include <stdint.h>
#include <drivers/device.h>

namespace genos 
{
	class i2c_device : public genos::device
	{
	public:
		virtual int write_memory(
			uint8_t devaddr, 
			uint8_t memaddr,
			const void * buffer, int buflen) = 0;

		//virtual int exchange(uint8_t addr, 
		//	const void * txdata, int txsize,
		//	void * rxdata, int rxsize) = 0;
	
		//virtual void set_slave_address(uint8_t addr) = 0;

		virtual void lock_bus() = 0;
		virtual void unlock_bus() = 0;
	};
}

#endif