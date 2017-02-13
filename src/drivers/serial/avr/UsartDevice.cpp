#ifndef GENOS_AVR_USART_DEVICE_H
#define GENOS_AVR_USART_DEVICE_H

#include "util/bits.h"
#include <drivers/serial/avr/UsartDevice.h>

void interruptHandler_UsartRX(UsartDevice* usart);
void interruptHandler_UsartTX(UsartDevice* usart);
void interruptHandler_UsartTC(UsartDevice* usart);

UsartDevice::UsartDevice(const usart_data& udata, gxx::buffer txbuf, gxx::buffer rxbuf)
 : UsartDriver(udata.regs, udata.irqbase), m_txring(txbuf), m_rxring(rxbuf) {}


int UsartDevice::begin() {
	enable(true);	
	
	setIRQHandlers(
		(IRQHandler) interruptHandler_UsartRX, this,
		(IRQHandler) interruptHandler_UsartTX, this,
		(IRQHandler) interruptHandler_UsartTC, this
	);
	//setIRQHandler(m_irqbase + 1, (IRQHandler) __interruptHandlerTX, this);
	//setIRQHandler(m_irqbase + 2, (IRQHandler) __interruptHandlerTC, this);
	enableRX(true);
	enableTX(true);
	
	irqEnableRX(true);
	//irqEnableTX(true);
	//irqEnableTC(true);
}

int UsartDevice::write(const char* data, size_t size) {
	int ret = 0;
	if (size == 0) return 0;

	auto save = global_irq_save();

	if (cansend() && m_txring.empty()) {
		ret += sendbyte(*data++);
		irqEnableTX(true);
	};

	ret += m_txring.write(data, size - ret);

	//if (!txwq.empty() && txring.room() > txring.size() / 2)
	//	txwq.wakeUp();

	global_irq_restore(save);

	return ret;
}

int UsartDevice::read(char* data, size_t size) {
	return m_rxring.read(data, size);
}

int UsartDevice::getc() {
	return m_rxring.getc();
}


int UsartDevice::flush() {

}

int UsartDevice::avail() {
	return m_rxring.avail();
}

int UsartDevice::room() {
	return m_txring.avail();
}


void interruptHandler_UsartRX(UsartDevice* usart) {
	//panic("rxHandler");
	if (usart->m_rxring.putc(usart->recvbyte()) == 0) panic("USART OVERPUT");
	usart->rxevent.emit_one();
}

void interruptHandler_UsartTX(UsartDevice* usart) {
	if (usart->m_txring.empty()) {
		usart->irqEnableTX(false);
		return;
	}

	usart->sendbyte(usart->m_txring.getc());
}

void interruptHandler_UsartTC(UsartDevice* usart) {
	panic("tcHandler");
}

#endif