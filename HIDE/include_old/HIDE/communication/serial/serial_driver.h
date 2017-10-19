#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H

#include <sys/cdefs.h>

#include <communication/serial/tty.h>
#include <drivers/serial/serial_device.h>

#include <datastruct/ring_head.h>

//struct serial_driver_uops {
//	int (*putchar) (struct serial_device *dev, char c);
//	int (*room) (struct serial_device *dev);
//};
struct uart;
struct tty_drv_ops;

struct ttydrv_serial {
	/*parent*/ 
	//struct tty_drv parent;

	/*hardware driver*/
	struct uart* dev;

	/*transmitter ring*/
	struct ring_head txring;
	char txbuf[128];
};

//extern const struct serial_device_ops usart_serial_device_operations;

__BEGIN_DECLS

int ttydrv_serial_init(struct ttydrv_serial* drv, struct uart* dev);
int tty_serial_init(struct tty_s* tty,  struct ttydrv_serial* drv, struct uart* dev);

extern struct tty_drv_ops ttydrv_serial_ops;

__END_DECLS

#define TTYDRV_SERIAL_DEF(name,__uart)		\
struct ttydrv_serial name = {				\
	.dev = (struct uart*) __uart,	\
}; 

#define TTY_SERIAL_DEF(name,uart)						\
TTYDRV_SERIAL_DEF(MACRO_CONCAT(name,_private),uart);	\
TTY_STD_DEF(name, &MACRO_CONCAT(name,_private), &ttydrv_serial_ops);

#endif