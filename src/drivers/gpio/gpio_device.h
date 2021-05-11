#ifndef DRIVERS_GPIO_DEVICE
#define DRIVERS_GPIO_DEVICE

#include <stdint.h>

class gpio_device 
{
public:
	virtual int mode(uint32_t mask, uint32_t mode, int arg=0) = 0;
	virtual int set(uint32_t mask, uint32_t level) = 0;
	virtual int toggle(uint32_t mask) = 0;
	virtual int get(uint32_t mask, uint32_t * result) = 0;
};

#endif