#include "util/bits.h"
#include <drivers/serial/avr/UsartBottomHalf.h>

void interruptHandler_UsartRX(AVRUsartBottomHalf* usart);
void interruptHandler_UsartTX(AVRUsartBottomHalf* usart);
void interruptHandler_UsartTC(AVRUsartBottomHalf* usart);

AVRUsartBottomHalf::AVRUsartBottomHalf(const usart_data& udata, gxx::buffer txbuf)
 : AVRUsartDriver(udata.regs, udata.irqbase), m_txring(txbuf) {}


int AVRUsartBottomHalf::begin(int32_t baud, 
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

int AVRUsartBottomHalf::write(const char* data, size_t size) {
	if (size == 0) return 0;

	int ret = 0;
	auto save = global_irqs_save();

	if (cansend() && m_txring.empty()) {
		ret += sendbyte(*data++);
		irqEnableTX(true);
	};
	ret += m_txring.write(data, size - ret);
	global_irqs_restore(save);

	return ret;
}

int AVRUsartBottomHalf::room() {
	return m_txring.avail();
}

void interruptHandler_UsartRX(AVRUsartBottomHalf* usart) {
	//if (usart->m_rxring.putc(usart->recvbyte()) == 0) panic("USART OVERPUT");
	//usart->rxevent.emit_one(usart);
	char c = usart->recvbyte();
	usart->received_char_handler(c);
}

void interruptHandler_UsartTX(AVRUsartBottomHalf* usart) {
	if (usart->m_txring.empty()) {
		usart->irqEnableTX(false);
		return;
	}

	usart->sendbyte(usart->m_txring.getc());
}

void interruptHandler_UsartTC(AVRUsartBottomHalf* usart) {
	panic("tcHandler");
}