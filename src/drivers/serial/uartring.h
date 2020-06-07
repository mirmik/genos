#ifndef GENOS_DATASTRUCT_UARTRING_H
#define GENOS_DATASTRUCT_UARTRING_H

#include <sys/cdefs.h>

#include <drivers/cdev/serdev.h>

#include <genos/cdev.h>
#include <drivers/serial/uart.h>

#include <igris/buffer.h>
#include <igris/datastruct/dlist.h>
#include <igris/datastruct/ring.h>

/*struct uartring;

__BEGIN_DECLS

int uartring_write(struct serial_device*, const char*, unsigned int len, int flags);
int uartring_read(struct serial_device*, char*, unsigned int len, int flags);
int uartring_room(struct serial_device*);
int uartring_avail(struct serial_device*);
//int uartring_waitread(struct serial_device*);

void uartring_begin(struct uartring * dev, struct uart_device * uart);

void uartring_emulate_read(struct uartring * dev,
                           const char*, unsigned int len);

__END_DECLS

extern const struct serial_device_operations uartring_dev_ops;*/

namespace genos
{

	struct uartring : public genos::char_device
	{
		genos::uart * udev;
		char* rxbuffer;
		char* txbuffer;
		struct ring_head rxring;
		struct ring_head txring;

		struct dlist_head txwait = DLIST_HEAD_INIT(txwait);
		struct dlist_head rxwait = DLIST_HEAD_INIT(rxwait);

		uint8_t debug_mode = 0;
		int refs = 0;

		uartring(const char* name, genos::uart * udev,
		         char* rxbuffer, char* txbuffer,
		         size_t rxsz, size_t txsz)
			:
			genos::char_device(name),
			udev(udev), rxbuffer(rxbuffer), txbuffer(txbuffer),
			rxring(RING_HEAD_INIT(rxsz)), txring(RING_HEAD_INIT(txsz))
		{}

		uartring(const char* name, genos::uart * udev,
		         igris::buffer rxbuf, igris::buffer txbuf)
			:
			genos::char_device(name),
			udev(udev), rxbuffer(rxbuf.data()), txbuffer(txbuf.data()),
			rxring(RING_HEAD_INIT(rxbuf.size())), txring(RING_HEAD_INIT(txbuf.size()))
		{}

		int read(void* data, size_t size, int flags) override;
		int write(const void* data, size_t size, int flags) override;
		int release() override;
		int open(genos::openres * filp) override;

		size_t room() override;
		size_t avail() override;

		void begin(genos::uart * udev);
		void begin() { begin(udev); };
	};
}

#define UARTRING_DECLARE(name, cname, uart, rxsz, txsz)				 			\
char name##_rxbuffer[rxsz];						 					 			\
char name##_txbuffer[txsz];						 					 			\
genos::uartring name { cname, (struct uart_device*)uart,					\
				name##_rxbuffer, name##_txbuffer,					 			\
				rxsz, txsz}

#endif