#include "util/bits.h"
#include <drivers/serial/avr/UsartStream.h>

void interruptHandler_UsartRX(UsartStream* usart);
void interruptHandler_UsartTX(UsartStream* usart);
void interruptHandler_UsartTC(UsartStream* usart);

UsartStream::UsartStream(const usart_data& udata, gxx::buffer txbuf, gxx::buffer rxbuf)
 : UsartDriver(udata.regs, udata.irqbase), m_txring(txbuf), m_rxring(rxbuf) {}


int UsartStream::begin(int32_t baud, 
		Uart::Parity parity, 
		Uart::StopBits stopBits, 
		Uart::DataBits dataBits) {
	enable(true);	
	
	setup(baud, parity, stopBits, dataBits);

	setIRQHandlers(
		(IRQHandler) interruptHandler_UsartRX, this,
		(IRQHandler) interruptHandler_UsartTX, this,
		(IRQHandler) interruptHandler_UsartTC, this
	);

	enableRX(true);
	enableTX(true);
	
	irqEnableRX(true);
}

int UsartStream::write(const char* data, size_t size) {
	if (size == 0) return 0;

	int ret = 0;
	auto save = global_irq_save();

	if (cansend() && m_txring.empty()) {
		ret += sendbyte(*data++);
		irqEnableTX(true);
	};
	ret += m_txring.write(data, size - ret);

	txevent.emit_one(this);
	global_irq_restore(save);

	return ret;
}

int UsartStream::read(char* data, size_t size) {
	return m_rxring.read(data, size);
}

int UsartStream::getc() {
	return m_rxring.getc();
}


int UsartStream::flush() {
	while(!m_txring.empty());
}

int UsartStream::avail() {
	return m_rxring.avail();
}

int UsartStream::room() {
	return m_txring.avail();
}


void interruptHandler_UsartRX(UsartStream* usart) {
	if (usart->m_rxring.putc(usart->recvbyte()) == 0) panic("USART OVERPUT");
	usart->rxevent.emit_one(usart);
}

void interruptHandler_UsartTX(UsartStream* usart) {
	if (usart->m_txring.empty()) {
		usart->irqEnableTX(false);
		return;
	}

	usart->sendbyte(usart->m_txring.getc());
}

void interruptHandler_UsartTC(UsartStream* usart) {
	panic("tcHandler");
}