#include "util/bits.h"
#include <drivers/serial/avr/UsartStream.h>

namespace Genos {

void interruptHandler_UsartRX(AvrUsartStream* usart);
void interruptHandler_UsartTX(AvrUsartStream* usart);
void interruptHandler_UsartTC(AvrUsartStream* usart);

AvrUsartStream::AvrUsartStream(const usart_data& udata, gxx::buffer txbuf, gxx::buffer rxbuf)
 : AvrUsartDriver(udata.regs, udata.irqbase), m_txring(txbuf), m_rxring(rxbuf) {}


int AvrUsartStream::begin(int32_t baud, 
		Uart::Parity parity, 
		Uart::StopBits stopBits, 
		Uart::DataBits dataBits) {
	enable(true);	
	
	setup(baud, parity, stopBits, dataBits);

	setIrqHandlers(
		(IRQHandler) interruptHandler_UsartRX, this,
		(IRQHandler) interruptHandler_UsartTX, this,
		(IRQHandler) interruptHandler_UsartTC, this
	);

	enableRX(true);
	enableTX(true);
	
	irqEnableRX(true);
}

int AvrUsartStream::write(const char* data, size_t size) {
	if (size == 0) return 0;

	int ret = 0;
	auto save = global_irq_save();

	if (cansend() && m_txring.empty()) {
		ret += sendbyte(*data++);
		irqEnableTX(true);
	};
	ret += m_txring.write(data, size - ret);

	global_irq_restore(save);

	return ret;
}

int AvrUsartStream::read(char* data, size_t size) {
	auto ret = m_rxring.read(data, size);
	if (m_rxring.avail() == 0) haveDataFlag.reset();
	return ret;
}

int AvrUsartStream::getc() {
	auto ret = m_rxring.getc();
	if (m_rxring.avail() == 0) haveDataFlag.reset();
	return ret;
}

int AvrUsartStream::avail() {
	return m_rxring.avail();
}

int AvrUsartStream::room() {
	return m_txring.avail();
}

/*void AvrUsartStream::waitReceivedData(Tasklet& tasklet) {
	auto save = global_irq_save();
	haveDataFlag.wait(tasklet);
	global_irq_restore(save);
}*/

void interruptHandler_UsartRX(AvrUsartStream* usart) {
	if (usart->m_rxring.putc(usart->recvbyte()) == 0) panic("USART OVERPUT");
	usart->haveDataFlag.set();
}

void interruptHandler_UsartTX(AvrUsartStream* usart) {
	if (usart->m_txring.empty()) {
		usart->irqEnableTX(false);
		return;
	}

	usart->sendbyte(usart->m_txring.getc());
}

void interruptHandler_UsartTC(AvrUsartStream* usart) {
	panic("tcHandler");
}

}