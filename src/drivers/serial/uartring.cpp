#include <drivers/serial/uartring.h>

#include <igris/sync/syslock.h>
#include <igris/util/bug.h>

//#include <gxx/panic.h>

#include <sched/api.h>
#include <sched/wait.h>

int genos::drivers::uartring_device::write(const char* data, unsigned int size)
{
	int curwrited;
	size_t writed = 0;

	if (uart->cantx() && ring_empty(&txring))
	{
		writed++;
		uart->sendbyte(*data);
	}

	while (size != writed)
	{
		system_lock();
		curwrited = ring_write(&txring, txbuffer, data + writed, size - writed);

		if (curwrited)
			uart->ctrirqs(UART_CTRIRQS_TXON);

		system_unlock();
		writed += curwrited;

		if (writed == size)
			break;

		if (flag.noblock)
			break;

		if (flag.nosched)
			continue;

		if (current_schedee() == NULL)
			break;

		else
		{
			wait_current_schedee(&txwlst, WAIT_PRIORITY);
		}

		//if (schedee_can_displace_asserted())
		//{
		//}
	}

	return writed;
}

int genos::drivers::uartring_device::read(char* data, unsigned int size)
{
	if (ring_empty(&rxring))
		return 0;

	return ring_read(&rxring, rxbuffer, data, size);
}

int genos::drivers::uartring_device::open(struct file * f)
{
	return 0;
}

int genos::drivers::uartring_device::release (struct file * f)
{
	return 0;
}

void uartring_device_irq_handler(void* priv, int code)
{
	struct genos::drivers::uartring_device* uring = (struct genos::drivers::uartring_device*) priv;

	switch (code)
	{
		case UART_IRQCODE_TX:
			{
				if ( ring_empty(&uring->txring) )
				{
					uring->uart->ctrirqs(UART_CTRIRQS_TXOFF);
					unwait_one(&uring->txwlst);
					return;
				}

				char c = ring_getc(&uring->txring, uring->txbuffer);
				uring->uart->sendbyte(c);
				return;
			}

		case UART_IRQCODE_RX:
			{
				ring_putc(&uring->rxring, uring->rxbuffer, uring->uart->recvbyte());
				unwait_one(&uring->rxwlst);
				return;
			}

		case UART_IRQCODE_TC: //fallthrow
			BUG();

		default:
			BUG();
	}
}

int genos::drivers::uartring_device::init(struct uart * u, const char* name)
{
	uart = u;

	ring_init(&rxring, UARTRING_BUFFER_SIZE_RX);
	ring_init(&txring, UARTRING_BUFFER_SIZE_TX);

	dlist_init(&txwlst);
	dlist_init(&rxwlst);

	//uart->enable(false);

	//uart->handler = uartring_device_irq_handler;
	//uart->handarg = (void*) this;
	uart->set_handler(uartring_device_irq_handler, (void*)this);
	uart->ctrirqs(UART_CTRIRQS_TXOFF);

	//uart->enable(true);

	//vfs_link_cdev(this, "/dev", name);

	return 0;
}

int genos::drivers::uartring_device::waitread()
{
	system_lock();

	if (ring_avail(&rxring))
	{
		system_unlock();
		return 1;
	}

	wait_current_schedee(&rxwlst, 0);
	system_unlock();
	return 0;
}









ssize_t genos::drivers::uartring::write(const void* _data, size_t size)
{
	int curwrited;
	size_t writed = 0;
	const char* data = (const char*) _data;

	if (uart->cantx() && ring_empty(&txring))
	{
		writed++;
		uart->sendbyte(*data);
	}

	if (size != writed)
	{
		system_lock();
		writed += ring_write(&txring, txbuffer, data + writed, size - writed);

		if (writed != 1)
			uart->ctrirqs(UART_CTRIRQS_TXON);

		system_unlock();
	}

	return writed;
}

ssize_t genos::drivers::uartring::read(void* _data, size_t size)
{
	char* data = (char*) _data;

	if (ring_empty(&rxring))
		return 0;

	return ring_read(&rxring, rxbuffer, data, size);
}

void uartring_irq_handler(void* priv, int code)
{
	struct genos::drivers::uartring* uring = (struct genos::drivers::uartring*) priv;

	switch (code)
	{
		case UART_IRQCODE_TX:
			{
				if ( ring_empty(&uring->txring) )
				{
					uring->uart->ctrirqs(UART_CTRIRQS_TXOFF);
					//	unwait_one(&uring->txwlst);
					return;
				}

				char c = ring_getc(&uring->txring, uring->txbuffer);
				uring->uart->sendbyte(c);
				return;
			}

		case UART_IRQCODE_RX:
			{
				ring_putc(&uring->rxring, uring->rxbuffer, uring->uart->recvbyte());
				//unwait_one(&uring->rxwlst);
				return;
			}

		case UART_IRQCODE_TC: //fallthrow
			BUG();

		default:
			BUG();
	}
}

int genos::drivers::uartring::init(genos::drivers::uart * u, char* rxbuf, int rxsz, char* txbuf, int txsz)
{
	uart = u;

	ring_init(&rxring, rxsz);
	ring_init(&txring, txsz);

	rxbuffer = rxbuf;
	txbuffer = txbuf;

	//dlist_init(&txwlst);
	//dlist_init(&rxwlst);

	//uart->enable(false);

//	uart->handler = uartring_irq_handler;
//	uart->handarg = (void*) this;
	uart->set_handler(uartring_irq_handler, (void*)this);

	uart->ctrirqs(UART_CTRIRQS_TXOFF);

	//uart->enable(true);

	//vfs_link_cdev(this, "/dev", name);

	return 0;
}

int genos::drivers::uartring::avail()
{
	return ring_avail(&rxring);
}

int genos::drivers::uartring::room()
{
	return ring_room(&txring);
}