#ifndef GENOS_DRIVERS_SERIAL_ARDUINO_SERIAL_DEVICE_H
#define GENOS_DRIVERS_SERIAL_ARDUINO_SERIAL_DEVICE_H

#include <Arduino.h>
#include <genos/drivers/device_head.h>

struct arduino_serial_device
{
	struct device_head dev;
	struct dlist_head lnk_reader;
	struct dlist_head waitqueue;
	Serial * serial;
};

__BEGIN_DECLS

void arduino_serial_devices_step();

__END_DECLS

#endif