#include <drivers/serial/uartring.h>

#include <gxx/syslock.h>
#include <gxx/panic.h>

#include <sched/api.h>
#include <sched/wait.h>

int uartring_device::write(const char* data, unsigned int size) 
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
			uart->txirq(true);
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

int uartring_device::read(char* data, unsigned int size) 
{
	if (ring_empty(&rxring)) 
		return 0;

	return ring_read(&rxring, rxbuffer, data, size);
}

int uartring_device::open(struct file * f) 
{ 
	return 0; 
}

int uartring_device::release (struct file * f) 
{ 
	return 0; 
}

void uartring_irq_handler(void* priv, int code) 
{
	struct uartring_device* uring = (struct uartring_device*) priv;
	switch(code) {
		case UART_IRQCODE_TX: {
			if ( ring_empty(&uring->txring) ) 
			{
				uring->uart->txirq(false);
				unwait_one(&uring->txwlst);
				return;
			}

			char c = ring_getc(&uring->txring, uring->txbuffer);
			uring->uart->sendbyte(c);
			return;
		}

		case UART_IRQCODE_RX: {
			ring_putc(&uring->rxring, uring->rxbuffer, uring->uart->recvbyte());
			unwait_one(&uring->rxwlst);
			return;
		}

		case UART_IRQCODE_TC: //fallthrow
			panic("2");
		default: dprln(code); panic("unrecognized uart irq code");
	}
}

int uartring_device::init(struct uart * u, const char* name) 
{
	uart = u;

	ring_init(&rxring, UARTRING_BUFFER_SIZE_RX);
	ring_init(&txring, UARTRING_BUFFER_SIZE_TX);

	dlist_init(&txwlst);
	dlist_init(&rxwlst);

	//uart->enable(false);

	uart->handler = uartring_irq_handler;
	uart->handarg = (void*) this;
	uart->txirq(false);
	
	//uart->enable(true);

	vfs_link_cdev(this, "/dev", name);

	return 0;
}

int uartring_device::waitread() {
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