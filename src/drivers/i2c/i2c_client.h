#ifndef GENOS_I2C_CLIENT_H
#define GENOS_I2C_CLIENT_H

#include <drivers/i2c/i2c_master.h>

namespace genos 
{
	class i2c_client : public genos::device
	{
	public:
		i2c_master * parent() { return (i2c_master*) genos::device::parent; }
		uint8_t addr;

		i2c_client(i2c_master * parent, uint8_t addr) : 
			device(parent), addr(addr)
		{}

		int write(const void* out, uint16_t olen) 
		{
			auto * master = parent();

			master->set_slave_address();
		}

		int read(void* in, uint16_t ilen) 
		{
			auto * master = parent();
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