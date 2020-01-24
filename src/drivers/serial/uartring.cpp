#include <drivers/serial/uartring.h>

#include <igris/defs/schedee.h>
#include <igris/sync/syslock.h>
#include <igris/util/bug.h>

#include <genos/sched.h>
#include <genos/wait.h>
#include <util/cpu_delay.h>

int genos::uartring::write(const void* data,
                           unsigned int size,
                           int flags)
{
	int curwrited;
	size_t writed = 0;

	if (size == 0) 
	{
		return 0;
	}

	system_lock();

	if (udev->cantx() && ring_empty(&txring))
	{
		writed++;
		udev->sendbyte(*(char*)data);
	}

	while (size != writed)
	{
		curwrited = ring_write(&txring,
		                       txbuffer,
		                       (char*)data + writed, size - writed);

		if (curwrited)
			udev->ctrirqs(UART_CTRIRQS_TXON);

		writed += curwrited;

		if (writed == size)
			break;

		if (flags & IO_HOTLOOP) 
		{
			system_unlock();
			cpu_delay(1000);
			system_lock();
			continue;
		}

		if ((flags & IO_NOBLOCK) || current_schedee() == NULL)
		{
			break;
		}

		system_unlock();
		wait_current_schedee(&txwait, WAIT_PRIORITY, nullptr);
		system_lock();
	}

	system_unlock();

	return writed;

}

#warning "wait_current_schedee and system_lock restoring"
int genos::uartring::read(void* data,
                          unsigned int size, int flags)
{
	int ret;

	system_lock();

	while (ring_empty(&rxring))
	{

		if (flags & IO_NOBLOCK) 
		{
			system_unlock();
			return 0;
		}

		system_unlock();
		if (wait_current_schedee(&rxwait, 0, nullptr) == SCHEDEE_DISPLACE_VIRTUAL)
		{
			system_unlock();
			return 0;
		}
	}

	system_unlock();

	if (flags & IO_ONLYWAIT)
		return 0;

	system_lock();
	ret = ring_read(&rxring, rxbuffer, (char*)data, size);
	system_unlock();

	return ret;
}

int genos::uartring::release()
{
	--refs;

	if (refs == 0)
	{
		udev->ctrirqs(UART_CTRIRQS_TXOFF);
		ring_clean(&rxring);
		ring_clean(&txring);
	}

	return 0;
}

int genos::uartring::open(genos::openres * ores)
{
	if (refs == 0)
	{
		udev->ctrirqs(UART_CTRIRQS_TXOFF);
		ring_clean(&rxring);
		ring_clean(&txring);
	}

	refs++;

	return 0;
}

int genos::uartring::room()
{
	return ring_room(&txring);
}

int genos::uartring::avail()
{
	return ring_avail(&rxring);
}

static void uartring_irq_handler(void* priv, int code)
{
	struct genos::uartring* uring = (struct genos::uartring*) priv;

	switch (code)
	{
		case UART_IRQCODE_TX:
			{
				if ( ring_empty(&uring->txring) )
				{
					uring->udev->ctrirqs(UART_CTRIRQS_TXOFF);
					unwait_one(&uring->txwait, nullptr);
					return;
				}

				char c = ring_getc(&uring->txring, uring->txbuffer);
				uring->udev->sendbyte(c);
				return;
			}

		case UART_IRQCODE_RX:
			{
				char c;

				c = uring->udev->recvbyte();

				if (uring->debug_mode)
				{
					dpr("i recv: "); dprhexln(c);
				}

				ring_putc(&uring->rxring, uring->rxbuffer, c);
				unwait_one(&uring->rxwait, nullptr);
				return;
			}

		case UART_IRQCODE_TC: //fallthrow
			BUG();

		default:
			dprln("code:", code);
			BUG();
	}
}

void uartring_emulate_read(struct genos::uartring * dev,
                           const char* data, unsigned int len)
{
	system_lock();

	ring_write(&dev->rxring, dev->rxbuffer, data, len);
	unwait_one(&dev->rxwait, nullptr);

	system_unlock();
}

void genos::uartring::begin(struct uart_device * uart)
{
	udev = uart;
	uart -> handler = uartring_irq_handler;
	uart -> handarg = (void*)this;

	udev->enable(1);
	udev->ctrirqs(UART_CTRIRQS_RXON);
}
