#include <communication/serial/tty.h>
#include <communication/serial/serial_driver.h>
#include <drivers/serial/uart_device.h>

#include <datastruct/ring_head.h>
#include <gxx/debug/dprint.h>

#include <assert.h>
#include <mem/alloc.h>

#include <util/retype.h>
#include <kernel/csection.h>

//int debug_serial_device_write(struct serial_device* dev, const void* data, size_t size);

int serial_putchar(struct tty_s* tty, char c) {
	RETYPE (struct ttydrv_serial*, drv, tty->drv);
	struct uart* dev = drv->dev;

	atomic_section_enter();

	if (dev->uops->room(dev) && ring_empty(&drv->txring)) {
		dev->uops->putc(dev, c);
		
		atomic_section_leave();
		return 1;
	};

	ring_putc(&drv->txring, drv->txbuf, c);
	dev->uops->txbegin(dev);

	atomic_section_leave();
	return 1;
}

int serial_write(struct tty_s* tty, const void *data, size_t size) {
	RETYPE(const char*, __data, data);
	size_t ret = 0;
	for (int i = 0; i < size; ++i) {
		ret += serial_putchar(tty, *__data++);
	}
	return ret;
}

struct tty_drv_ops ttydrv_serial_ops = {
	.putchar = serial_putchar,
	.write = serial_write,
};

int ttydrv_serial_init(struct ttydrv_serial* drv, struct uart* dev) {
	assert(dev);

	drv->dev = (struct serial_device*)dev;
	
	ring_init(&drv->txring, sizeof(drv->txring));
	
	return (struct tty_driver*) drv;
}

int tty_serial_init(struct tty_s* tty, struct ttydrv_serial* drv, struct uart* dev) {
	ttydrv_serial_init(drv, dev);
	return tty_init(tty, drv, &ttydrv_serial_ops);
}

