#ifndef GENOS_I2C_CLIENT_H
#define GENOS_I2C_CLIENT_H

#include <drivers/i2c/i2c_master.h>

namespace genos 
{
	class i2c_client : public genos::device
	{
	public:
		i2c_device * dev;
		uint8_t addr;

		i2c_client(i2c_master * parent, uint8_t addr) : 
			device(parent), addr(addr)
		{}

		int write(const void* out, uint16_t olen) 
		{
			dev->lock_bus();
			dev->set_slave_address(addr);
			dev->exchange(out, olen, nullptr, 0);
			dev->unlock_bus();
		}

		int exchange(
			const void* out, uint16_t olen, 
			void* in, uint16_t ilen) 
		{
			dev->lock_bus();
				
			dev->unlock_bus();
		}

		int lock_bus() 
		{
			parent()->lock_bus();
		}

		int unlock_bus() 
		{
			parent()->unlock_bus();
		}
	};
}

#endif