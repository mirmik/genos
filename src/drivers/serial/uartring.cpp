#include <drivers/serial/uartring.h>

#include <igris/sync/syslock.h>
#include <igris/util/bug.h>

#include <genos/sched.h>
#include <genos/wait.h>

int uartring_device::write(const void* data,
                          unsigned int size,
                          int flags)
{
	int curwrited;
	size_t writed = 0;

	system_lock();
	if (uart_device_cantx(udev) && ring_empty(&txring))
	{
		writed++;
		uart_device_sendbyte(udev, *(char*)data);
	}
	
	while (size != writed)
	{
		curwrited = ring_write(&txring,
		                       txbuffer,
		                       (char*)data + writed, size - writed);

		if (curwrited)
			uart_device_ctrirqs(udev, UART_CTRIRQS_TXON);

		writed += curwrited;

		if (writed == size)
			break;

		if (flags & IO_HOTLOOP)
			continue;

		if ((flags & IO_NOBLOCK) || current_schedee() == NULL) {
			break;
		}

		system_unlock();
		wait_current_schedee(&txwait, WAIT_PRIORITY);
		system_lock();
	}

	system_unlock();

	return writed;

}

int uartring_device::read(void* data,
                         unsigned int size, int flags)
{
	int ret;

	system_lock();
	
	while (ring_empty(&rxring)) 
	{

		if (flags & IO_NOBLOCK) 
			return 0;

		//dprln("WAIT");
		if (wait_current_schedee(&rxwait, 0) == DISPLACE_VIRTUAL) {
			//dprln("AFTERWAIT");
			return 0; 
		}
	}

	system_unlock();
	
	if (flags & IO_ONLYWAIT) 
		return 0;

		//dprln("READ");

	system_lock();
	ret = ring_read(&rxring, rxbuffer, (char*)data, size);
	system_unlock();

	return ret;
}

int uartring_device::release() 
{
	uart_device_ctrirqs(udev, UART_CTRIRQS_TXOFF);
	ring_clean(&rxring);
	ring_clean(&txring);
	return 0;
}

int uartring_device::open(genos::file * ores) 
{
	ores->node = this;

	uart_device_ctrirqs(udev, UART_CTRIRQS_TXOFF);
	ring_clean(&rxring);
	ring_clean(&txring);
	return 0;
}

int uartring_device_room(struct serial_device* dev)
{
	RETYPE(struct uartring_device *, udev, dev);
	return ring_room(&udev->txring);
}

int uartring_device_avail(struct serial_device* dev)
{
	RETYPE(struct uartring_device *, udev, dev);
	return ring_avail(&udev->rxring);
}

/*int uartring_device_waitread(struct serial_device* dev)
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
				uart_device_ctrirqs(uring->udev, UART_CTRIRQS_TXOFF);
				unwait_one(&uring->txwait);
				return;
			}

			char c = ring_getc(&uring->txring, uring->txbuffer);
			uart_device_sendbyte(uring->udev, c);
			return;
		}

		case UART_IRQCODE_RX:
		{
			char c;

			c = uart_device_recvbyte(uring->udev);
			if (uring->debug_mode) 
			{
				dpr("i recv: "); dprhexln(c); 
			}

			ring_putc(&uring->rxring, uring->rxbuffer, c);
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
	dev -> udev = uart;
	uart -> handler = uartring_ddevice_irq_handler;
	uart -> handarg = (void*)dev;
	uart_device_ctrirqs(dev->udev, UART_CTRIRQS_RXON);
}
