#ifndef GENOS_DRIVER_I2C_DRIVER_H
#define GENOS_DRIVER_I2C_DRIVER_H

namespace genos 
{
	class i2c_driver 
	{
	public:
		struct semaphore lock = SEMAPHORE_INIT(lock, 1);

		virtual void sendbyte(uint8_t byte) = 0;
		virtual uint8_t recvbyte() = 0;
	}
}

#endif