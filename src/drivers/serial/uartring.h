#ifndef GENOS_DATASTRUCT_UARTRING_H
#define GENOS_DATASTRUCT_UARTRING_H

#include <sys/cdefs.h>

#include <drivers/cdev/cdev.h>
#include <drivers/serial/uart.h>

#include <igris/datastruct/ring.h>


#ifdef __cplusplus
#define UARTRING_BUFFER_SIZE_RX 8
#define UARTRING_BUFFER_SIZE_TX 16

#include <mvfs/file.h>
//#include <nos/io/iostorage.h>

/*namespace genos
{
	namespace drivers
	{
		struct uartring_device : public char_device
		{
			struct uart * uart;

			char rxbuffer[UARTRING_BUFFER_SIZE_RX];
			char txbuffer[UARTRING_BUFFER_SIZE_TX];
			struct ring_head rxring;
			struct ring_head txring;

			struct dlist_head txwlst;
			struct dlist_head rxwlst;

			int init(struct uart * u, const char* name);

			ssize_t write(const char* data, unsigned int size) override;
			ssize_t read(char* data, unsigned int size) override;
			int waitread() override;

			//file operations
			//int write(struct file * f, const char* data, unsigned int size);
			//int read(struct file * f, char* data, unsigned int size);
			int open(struct file * f) override;
			int release (struct file * f) override;
		};


		struct uartring : public nos::iostorage
		{
			struct uart * uart;

			char* rxbuffer;
			char* txbuffer;
			struct ring_head rxring;
			struct ring_head txring;

			uartring(genos::drivers::uart * u, char* rxbuf, int rxsz, char* txbuf, int txsz)
			{
				init(u, rxbuf, rxsz, txbuf, txsz);
			}

			int init(genos::drivers::uart * u, char* rxbuf, int rxsz, char* txbuf, int txsz);

			ssize_t write(const void* data, size_t size) override;
			ssize_t read(void* data, size_t size) override;

			int room() override;
			int avail() override;
		};
	};
}*/
#endif

struct uartring_device;

__BEGIN_DECLS

int uartring_device_write(struct char_device*, const char*, unsigned int len, int flags);
int uartring_device_read(struct char_device*, char*, unsigned int len, int flags);
int uartring_device_room(struct char_device*);
int uartring_device_avail(struct char_device*);
int uartring_device_waitread(struct char_device*);

void uartring_begin(struct uartring_device * dev, struct uart_device * uart);

__END_DECLS

const struct char_device_operations uartring_dev_ops = 
{
	.write = 		uartring_device_write,
	.read = 		uartring_device_read,
	.room = 		uartring_device_room,
	.avail = 		uartring_device_avail,
	.waitread = 	uartring_device_waitread
};

struct uartring_device
{
	struct char_device cdev;
	struct uart_device * uart;
	char* rxbuffer;
	char* txbuffer;
	struct ring_head rxring;
	struct ring_head txring;
};

#define UARTRING_DECLARE(name, uart, rxsz, txsz) 					 			\
char name##_rxbuffer[rxsz];						 					 			\
char name##_txbuffer[txsz];						 					 			\
struct uartring_device name = { CHAR_DEVICE_INIT(name.cdev, &uartring_dev_ops), \
				(struct uart_device*)uart, 							 			\
				name##_rxbuffer, name##_txbuffer,					 			\
				RING_HEAD_INIT(rxsz), RING_HEAD_INIT(txsz)}			



#endif