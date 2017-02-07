#include <drivers/serial/uartring.h>

int uartring_write(uartring_t * dev, const char* str, size_t sz) {
	const struct uart_operations * uops = dev->udev.uops;

	int ret = 0;
	if (size == 0) return 0;

	atomic_section_enter();

	if (m_uart->room() && txring.empty()) {
		ret += m_uart->putc(*data++);
		m_uart->irqEnableTX(true);
	};

	ret += txring.write(data, size - ret);

	//if (!txwq.empty() && txring.room() > txring.size() / 2)
	//	txwq.wakeUp();

	atomic_section_leave();

	return ret;
}

int uartring_riad(uartring_t * dev, const char* str, size_t sz) {
	
}

int uartring_avail(uartring_t * dev, const char* str, size_t sz) {
	
}

int uartring_room(uartring_t * dev, const char* str, size_t sz) {
	
}