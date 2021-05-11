#ifndef DRIVERS_GPIO_DEVICE
#define DRIVERS_GPIO_DEVICE

#include <stdint.h>

class gpio_device 
{
public:
	virtual int mode(unsigned int mask, uint32_t mode, int arg=0) = 0;
	virtual void set(unsigned int mask, uint8_t level) = 0;
	virtual void toggle(unsigned int mask) = 0;
	virtual unsigned int get(unsigned int mask) = 0;
};

#endif