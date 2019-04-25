#include <drivers/serial/uartring.h>
#include <sched/wait.h>
#include <igris/sync/syslock.h>

#include <sched/sched.h>

#include <igris/util/bug.h>

int uartring_device_write(struct char_device* dev,
                          const char* data,
                          unsigned int size,
                          int flags)
{
	RETYPE(struct uartring_device *, udev, dev);

	int curwrited;
	size_t writed = 0;

	if (uart_device_cantx(udev->uart) && ring_empty(&udev->txring))
	{
		writed++;
		uart_device_sendbyte(udev->uart, *data);
	}

	while (size != writed)
	{
		system_lock();
		curwrited = ring_write(&udev->txring,
		                       udev->txbuffer,
		                       data + writed, size - writed);

		if (curwrited)
			uart_device_ctrirqs(udev->uart, UART_CTRIRQS_TXON);

		system_unlock();
		writed += curwrited;

		if (writed == size)
			break;

		if (flags && IO_HOTLOOP)
			continue;

		if (flags && IO_NOBLOCK || current_schedee() == NULL)
			break;

		else
		{
			wait_current_schedee(&udev->txwait, WAIT_PRIORITY);
		}
	}

	return writed;

}

int uartring_device_read(struct char_device* dev, char* data,
                         unsigned int size, int flags)
{
	RETYPE(struct uartring_device *, udev, dev);

	while (ring_empty(&udev->rxring)) 
	{
		if (flags & IO_NOBLOCK) 
			return 0;

		if (wait_current_schedee(&udev->rxwait, 0) == DISPLACE_VIRTUAL) 
			return 0; 
	}

	if (flags & IO_ONLYWAIT) 
		return 0;

	return ring_read(&udev->rxring, udev->rxbuffer, data, size);
}

int uartring_device_room(struct char_device* dev)
{
	RETYPE(struct uartring_device *, udev, dev);
	return ring_room(&udev->txring);
}

int uartring_device_avail(struct char_device* dev)
{
	RETYPE(struct uartring_device *, udev, dev);
	return ring_avail(&udev->rxring);
}

/*int uartring_device_waitread(struct char_device* dev)
{
	RETYPE(struct uartring_device *, udev, dev);
	system_lock();

	if (ring_avail(&udev->rxring))
	{
		system_unlock();
		return 1;
	}

	wait_current_schedee(&dev->node.rxwlst, 0);
	system_unlock();
	return 0;
}*/

void uartring_ddevice_irq_handler(void* priv, int code)
{
	struct uartring_device* uring = (struct uartring_device*) priv;

	switch (code)
	{
		case UART_IRQCODE_TX:
		{
			if ( ring_empty(&uring->txring) )
			{
				uart_device_ctrirqs(uring->uart, UART_CTRIRQS_TXOFF);
				unwait_one(&uring->txwait);
				return;
			}

			char c = ring_getc(&uring->txring, uring->txbuffer);
			uart_device_sendbyte(uring->uart, c);
			return;
		}

		case UART_IRQCODE_RX:
		{
			ring_putc(&uring->rxring, uring->rxbuffer, uart_device_recvbyte(uring->uart));
			unwait_one(&uring->rxwait);
			return;
		}

		case UART_IRQCODE_TC: //fallthrow
			BUG();

		default:
			dprln("code:", code);
			BUG();
	}
}

void uartring_emulate_read(struct uartring_device * dev,
                           const char* data, unsigned int len) 
{
	system_lock();

	ring_write(&dev->rxring, dev->rxbuffer, data, len);
	unwait_one(&dev->rxwait);

	system_unlock();
}

void uartring_begin(struct uartring_device * dev, struct uart_device * uart)
{
	dev -> uart = uart;
	uart -> handler = uartring_ddevice_irq_handler;
	uart -> handarg = (void*)dev;
	uart_device_ctrirqs(dev->uart, UART_CTRIRQS_RXON);
}

const struct char_device_operations uartring_dev_ops = 
{
	.write = 		uartring_device_write,
	.read = 		uartring_device_read,
	.room = 		uartring_device_room,
	.avail = 		uartring_device_avail,
	//.waitread = 	uartring_device_waitread
};
