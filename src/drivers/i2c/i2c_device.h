#ifndef GENOS_DRIVERS_I2C_DEVICE_H
#define GENOS_DRIVERS_I2C_DEVICE_H

namespace genos 
{
	class i2c_device 
	{
		virtual void write(uint8_t addr, 
			const uint8_t * data, int size) = 0;
	
		virtual void read(uint8_t addr, 
			uint8_t * data, int size) = 0;
	
		virtual void exchange(uint8_t addr, 
			uint8_t * rxdata, int rxsize,
			const uint8_t * txdata, int txsize) = 0;
	
		virtual void lock_bus() = 0;
		virtual void unlock_bus() = 0;
	}
}

#endif