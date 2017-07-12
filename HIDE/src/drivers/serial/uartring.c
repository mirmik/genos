#include <drivers/serial/uartring.h>
#include <kernel/csection.h>
#include <gxx/debug/dprint.h>
#include <genos/panic.h>

int uartring_write(chardev_t * dev, const char* data, size_t sz) {
	uartring_t * uring = (uartring_t *) dev;
	uartdev_t * udev = &uring->udev;
	const struct uart_operations * uops = udev->uops;
	
	int ret = 0;
	if (sz == 0) return 0;

	atomic_section_enter();

	if (udev->uops->room(udev) && ring_empty(&uring->txring)) {
		ret += udev->uops->putc(udev, *data++);
		udev->uops->irq_enable_tx(udev, true);
	};

	ret += ring_write(&uring->txring, uring->txbuf, data, sz - ret);

	//if (!txwq.empty() && txring.room() > txring.size() / 2)
	//	txwq.wakeUp();

	atomic_section_leave();

	return ret;
}

int uartring_read(chardev_t * dev, const char* str, size_t sz) {
	
}

int uartring_avail(chardev_t * dev) {
	
}

int uartring_room(chardev_t * dev) {
	
}

const struct chardev_operations uartring_chardev_ops = {
	.write = uartring_write,
	.read = uartring_read,
	.avail = uartring_avail,
	.room = uartring_room,
};

void uartring_irq_rx(uartdev_t * udev, char c) {
	
}

void uartring_irq_tx(uartdev_t * udev) {
	uartring_t * uring = (uartring_t *) udev;
	
	if (ring_empty(&uring->txring)) {
		udev->uops->irq_enable_tx(udev,false);
		return;
	}

	udev->uops->putc(udev, ring_getc(&uring->txring, uring->txbuf));
}

void uartring_irq_tc(uartdev_t * udev) {
	panic("tc");
}

const struct uart_irq_handlers uartring_uartdev_irqops = {
	.rx_handler = uartring_irq_rx,
	.tx_handler = uartring_irq_tx,
	.tc_handler = uartring_irq_tc,
};