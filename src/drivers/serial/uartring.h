#ifndef GENOS_DATASTRUCT_UARTRING_H
#define GENOS_DATASTRUCT_UARTRING_H

#include <sys/cdefs.h>

#include <drivers/cdev/serdev.h>

#include <genos/cdev.h>
#include <drivers/serial/uart.h>

#include <igris/datastruct/dlist.h>
#include <igris/datastruct/ring.h>

struct uartring_device;

__BEGIN_DECLS

int uartring_device_write(struct serial_device*, const char*, unsigned int len, int flags);
int uartring_device_read(struct serial_device*, char*, unsigned int len, int flags);
int uartring_device_room(struct serial_device*);
int uartring_device_avail(struct serial_device*);
//int uartring_device_waitread(struct serial_device*);

void uartring_begin(struct uartring_device * dev, struct uart_device * uart);

void uartring_emulate_read(struct uartring_device * dev,
                           const char*, unsigned int len);

__END_DECLS

extern const struct serial_device_operations uartring_dev_ops;

struct uartring_device : public genos::char_device
{
	//struct serial_device cdev;
	struct uart_device * udev;
	char* rxbuffer;
	char* txbuffer;
	struct ring_head rxring;
	struct ring_head txring;

	struct dlist_head txwait = DLIST_HEAD_INIT(txwait);
	struct dlist_head rxwait = DLIST_HEAD_INIT(rxwait);

	uint8_t debug_mode = 0;
	int refs = 0;

	uartring_device(const char* name, uart_device * udev, 
			char* rxbuffer, char* txbuffer,
			size_t rxsz, size_t txsz) 
	:
		genos::char_device(name),
		udev(udev), rxbuffer(rxbuffer), txbuffer(txbuffer),
		rxring(RING_HEAD_INIT(rxsz)), txring(RING_HEAD_INIT(txsz)) 
	{}

	int read(void* data, size_t size, int flags) override;
	int write(const void* data, size_t size, int flags) override;
	int release() override;
	int open(genos::openres * filp) override;
};

#define UARTRING_DECLARE(name, cname, uart, rxsz, txsz)				 			\
char name##_rxbuffer[rxsz];						 					 			\
char name##_txbuffer[txsz];						 					 			\
struct uartring_device name { cname, (struct uart_device*)uart,					\
				name##_rxbuffer, name##_txbuffer,					 			\
				rxsz, txsz}

#endif