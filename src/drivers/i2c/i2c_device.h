#ifndef GENOS_DRIVERS_I2C_DEVICE_H
#define GENOS_DRIVERS_I2C_DEVICE_H

#include <stdint.h>
#include <drivers/device.h>

class i2c_device : public genos::device
{
public:
	virtual int write_memory(
	    uint8_t devaddr,
	    uint8_t memaddr,
	    const void * buffer, int buflen) = 0;


	virtual int write(
	    uint8_t devaddr,
	    const void * buffer, int buflen) = 0;

	//virtual int write_start(uint8_t devaddr);
	//virtual int write_finish(uint8_t devaddr);


	virtual int read(
	    uint8_t devaddr,
	    const void * buffer, int buflen) = 0;

	virtual void lock_bus() = 0;
	virtual void unlock_bus() = 0;

	int writeread(int devaddr, 
		const void * outdata, int outsize,
		void * indata, int insize) 
	{
		int w = write(devaddr, outdata, outsize);
		int r = read(devaddr, indata, insize);

		return r;
	}
};

#endif