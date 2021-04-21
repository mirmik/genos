#ifndef GENOS_DRIVERS_AVR_I2C_H
#define GENOS_DRIVERS_AVR_I2C_H

#include <drivers/i2c/i2c_device.h>

class avr_i2c_device : public i2c_device
{
	device * parent_device() override;
	int      read(uint8_t, const void*, int) override;
	int      write(uint8_t, const void*, int) override;
	void     lock_bus() override;
	void     unlock_bus() override;

	int write_memory(uint8_t, uint8_t, const void*, int) override;
};

#endif