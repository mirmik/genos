#include <drivers/serial/serial_device.h>
#include <stdio.h>

#include <gxx/debug/dprint.h>

int debug_serial_device_putchar(struct serial_device* dev, char c) {
	debug_putchar(c);
	return 1;
}

int debug_serial_device_room(struct serial_device* dev) {
	return 1;
}

const struct serial_device_ops debug_serial_device_operations = {
	.putchar = &debug_serial_device_putchar,
	.write = &debug_serial_device_write,
	.room = debug_serial_device_room
};

const struct serial_device debug_serial_device = {
	.ops = (struct serial_device_ops*) &debug_serial_device_operations,
};
