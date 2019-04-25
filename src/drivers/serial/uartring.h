#ifndef GENOS_DATASTRUCT_UARTRING_H
#define GENOS_DATASTRUCT_UARTRING_H

#include <sys/cdefs.h>

#include <drivers/cdev/cdev.h>
#include <drivers/serial/uart.h>

#include <igris/datastruct/dlist.h>
#include <igris/datastruct/ring.h>

struct uartring_device;

__BEGIN_DECLS

int uartring_device_write(struct char_device*, const char*, unsigned int len, int flags);
int uartring_device_read(struct char_device*, char*, unsigned int len, int flags);
int uartring_device_room(struct char_device*);
int uartring_device_avail(struct char_device*);
//int uartring_device_waitread(struct char_device*);

void uartring_begin(struct uartring_device * dev, struct uart_device * uart);

void uartring_emulate_read(struct uartring_device * dev,
                           const char*, unsigned int len);

__END_DECLS

extern const struct char_device_operations uartring_dev_ops;

struct uartring_device
{
	struct char_device cdev;
	struct uart_device * uart;
	char* rxbuffer;
	char* txbuffer;
	struct ring_head rxring;
	struct ring_head txring;

	struct dlist_head txwait;
	struct dlist_head rxwait;
};

#define UARTRING_DECLARE(name, uart, rxsz, txsz) 					 			\
char name##_rxbuffer[rxsz];						 					 			\
char name##_txbuffer[txsz];						 					 			\
struct uartring_device name = { CHAR_DEVICE_INIT(name.cdev, &uartring_dev_ops), \
				(struct uart_device*)uart, 							 			\
				name##_rxbuffer, name##_txbuffer,					 			\
				RING_HEAD_INIT(rxsz), RING_HEAD_INIT(txsz), 					\
				DLIST_HEAD_INIT(name.txwait), DLIST_HEAD_INIT(name.rxwait)}



#endif