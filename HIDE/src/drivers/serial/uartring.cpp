#include <drivers/serial/uartring.h>

#include <igris/defs/schedee.h>
#include <igris/sync/syslock.h>
#include <igris/util/bug.h>

#include <genos/schedee.h>
#include <genos/wait.h>
#include <util/cpu_delay.h>

#include <hal/irq.h>

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

	if (flags & IO_WAITSEND)
	{
		// Ждём, пока уйдет последний символ.
		while (1)
		{
			system_lock();
			if (ring_empty(&txring) && udev->cantx())
			{
				system_unlock();
				break;
			}
			system_unlock();
			cpu_delay(1000);
		}
	}

	return writed;

}

int genos::uartring::read(void* data,
                          unsigned int size, int flags)
{
	int ret;
	irqstate_t save;

	save = irqs_save();
	while (ring_empty(&rxring))
	{

		if (flags & IO_NOBLOCK)
		{
			irqs_restore(save);
			return 0;
		}

		if (wait_current_schedee(&rxwait, 0, nullptr) == SCHEDEE_DISPLACE_VIRTUAL)
		{
			irqs_restore(save);
			return 0;
		}
	}

	irqs_restore(save);

	if (flags & IO_ONLYWAIT)
		return 0;

	save = irqs_save();
	ret = ring_read(&rxring, rxbuffer, (char*)data, size);
	irqs_restore(save);

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

size_t genos::uartring::room()
{
	return ring_room(&txring);
}

size_t genos::uartring::avail()
{
	igris::syslock_guard lock;
	int avail = ring_avail(&rxring);
	return avail;
}

void genos::uartring::clear()
{
	igris::syslock_guard lock;
	ring_clean(&rxring);
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

		//case UART_IRQCODE_TC: //fallthrow


		default:
			dprln("uring:", code);
			return;
			//BUG();
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

void genos::uartring::begin(genos::uart_device * uart)
{
	udev = uart;
	uart -> handler = uartring_irq_handler;
	uart -> handarg = (void*)this;

	udev->enable(1);
	udev->ctrirqs(UART_CTRIRQS_RXON);
}
