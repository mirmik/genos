#ifndef GENOS_DRIVERS_GPIO_PIN_3
#define GENOS_DRIVERS_GPIO_PIN_3

#include <drivers/gpio/gpio_device.h>

class gpio_device_pin 
{
	gpio_device * _dev;
	uint32_t      _mask;

public:
	gpio_device_pin(gpio_device * dev, uint32_t mask) :
		dev(dev), mask(mask)
	{}

	int set(unsigned char level);
	int get();
	int toggle();
	int mode(uint32_t mode, int arg);
};

#endif