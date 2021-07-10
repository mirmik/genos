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
	virtual int mode(uint32_t _mode, int arg) = 0;
};

#endif