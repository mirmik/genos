#ifndef DRIVERS_GPIO_DEVICE
#define DRIVERS_GPIO_DEVICE

#include <stdint.h>

class gpio_device 
{
public:
	virtual int setup(unsigned int mask, uint32_t mode) = 0;
	
	virtual void write(unsigned int mask, uint8_t level) = 0;
	virtual void toggle(unsigned int mask) = 0;
	virtual unsigned int read(unsigned int mask) = 0;

	virtual void pull_up(unsigned int mask, uint8_t level) = 0;	

	virtual int alternate(unsigned int mask, uint8_t code) 
	{
		return -1;
	}
};

#endif