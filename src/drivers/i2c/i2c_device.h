#ifndef GENOS_DRIVERS_I2C_DEVICE_H
#define GENOS_DRIVERS_I2C_DEVICE_H

#include <drivers/device.h>

namespace genos 
{
	class i2c_device : public genos::device
	{
	public:
//		virtual void write(uint8_t addr, 
//			const uint8_t * data, int size) = 0;
	
//		virtual void read(uint8_t addr, 
//			uint8_t * data, int size) = 0;
	
		virtual int exchange(uint8_t addr, 
			const void * txdata, int txsize,
			void * rxdata, int rxsize) = 0;
	
		virtual void set_slave_address(uint8_t addr);

		virtual void lock_bus() = 0;
		virtual void unlock_bus() = 0;
	};
}

#endif