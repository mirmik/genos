#ifndef DRIVERS_GPIO_PIN2_H
#define DRIVERS_GPIO_PIN2_H

#include <stdint.h>
#include <defs/gpiodefs.h>

class gpio_pin
{
public:
	virtual int set(uint8_t lvl) = 0;
	virtual int get() = 0;
	virtual int toggle() = 0;

	virtual int setup(uint32_t _mode) = 0;
	virtual int alternate(uint8_t fn) { return -1; }
};

#endif