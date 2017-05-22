#include <drivers/serial/UartRingDriver.h>

UartRingDriver::UartRingDriver(Uart* uart, char* txbuffer, size_t txsize, char* rxbuffer, size_t rxsize) : SerialDriver() {
	txring.init(txbuffer, txsize);
	rxring.init(rxbuffer, rxsize);
	m_uart = uart;
	m_uart->setDriver(this);
}

int UartRingDriver::write(const char* data, size_t size) {
	int ret = 0;
	if (size == 0) return 0;

	global_irqs_disable();

	if (m_uart->room() && txring.empty()) {
		ret += m_uart->putc(*data++);
		m_uart->irqEnableTX(true);
	};

	ret += txring.write(data, size - ret);

	//if (!txwq.empty() && txring.room() > txring.size() / 2)
	//	txwq.wakeUp();

	global_irqs_enable();

	return ret;
}

int UartRingDriver::read(char* data, size_t size) {
	return rxring.read(data, size);
}

int UartRingDriver::flush() {

}

int UartRingDriver::avail() {
	return rxring.avail();
}

int UartRingDriver::room() {
	return txring.avail();
}


void UartRingDriver::txHandler() {
	if (txring.empty()) {
		m_uart->irqEnableTX(false);
		return;
	}

	m_uart->putc(txring.getc());
}

bool UartRingDriver::waitread(WaiterBasic* waiter) {
	rxwq.addWaiter(waiter);
	return true;
}

bool UartRingDriver::waitwrite(WaiterBasic* waiter) {
	txwq.addWaiter(waiter);
	return true;
}

void UartRingDriver::tcHandler() {
	panic("tcHandler");
}

void UartRingDriver::rxHandler(char c) {
	rxring.putc(c);
	rxwq.wakeUp();
}

void UartRingDriver::begin(UartParams* params) {
	m_uart->setup(params);
	m_uart->enable(true);
	m_uart->enableRX(true);
	m_uart->enableTX(true);
	m_uart->irqEnableRX(true);
}