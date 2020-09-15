#ifndef GENOS_I2C_CLIENT_H
#define GENOS_I2C_CLIENT_H

#include <drivers/i2c/i2c_device.h>
#include <drivers/i2c/i2c_master.h>

namespace genos 
{
	class i2c_client : public genos::device
	{
	public:
		i2c_device * dev;
		uint8_t addr;

		i2c_client(i2c_device * parent, uint8_t addr) : 
			addr(addr)
		{}

		genos::device * parent_device() override { return dev; }

		int write(const void* out, uint16_t olen) 
		{
			int ret;
			
			dev->set_slave_address(addr);
			ret = dev->exchange(addr, out, olen, nullptr, 0);
			
			return ret;
		}

		int exchange(
			const void* out, uint16_t olen, 
			void* in, uint16_t ilen) 
		{
			dev->lock_bus();
				
			dev->unlock_bus();

			return 0;
		}

		void lock_bus() 
		{
			dev->lock_bus();
		}

		void unlock_bus() 
		{
			dev->unlock_bus();
		}
	};
}

#endif