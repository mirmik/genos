#ifndef GENOS_DRIVERS_SERIAL_ARDUINO_SERIAL_DEVICE_H
#define GENOS_DRIVERS_SERIAL_ARDUINO_SERIAL_DEVICE_H

#include <Arduino.h>
#include <genos/drivers/device_head.h>

struct arduino_serial_device
{
	struct stream_adapter adapter;
};

__BEGIN_DECLS

void arduino_serial_device_init(
	struct arduino_serial_device * dev,
	Stream * serial
);

__END_DECLS

#endif