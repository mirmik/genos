#ifndef GENOS_AVR_HARDWARE_USART_H
#define GENOS_AVR_HARDWARE_USART_H

#include "util/bits.h"
#include <gxx/debug/dprint.h>
#include <drivers/serial/avr/UsartDriver.h>

namespace Genos {

AvrUsartDriver::AvrUsartDriver(usart_regs* regs, int irqbase) : m_regs(regs), m_irqbase(irqbase) {}

int AvrUsartDriver::sendbyte(char c) {
	m_regs->udr = c;
	return 1;
}

int AvrUsartDriver::recvbyte() {
	return m_regs->udr;
}

int AvrUsartDriver::canrecv() {
	return bits_mask(m_regs->ucsr_a, (1 << RXC0));
}

int AvrUsartDriver::cansend() {
	return bits_mask(m_regs->ucsr_a, (1 << UDRE0));
}

int AvrUsartDriver::enable(bool en) {}

int AvrUsartDriver::enableRX(bool en) {
	bits_lvl(m_regs->ucsr_b, (1 << RXEN0), en);
}

int AvrUsartDriver::enableTX(bool en) {
	bits_lvl(m_regs->ucsr_b, (1 << TXEN0), en);
}

int AvrUsartDriver::irqEnableRX(bool en) {
	bits_lvl(m_regs->ucsr_b, (1 << RXCIE0), en);
}

int AvrUsartDriver::irqEnableTX(bool en) {
	bits_lvl(m_regs->ucsr_b, (1 << UDRIE0), en);
}

int AvrUsartDriver::irqEnableTC(bool en) {
	bits_lvl(m_regs->ucsr_b, (1 << TXCIE0), en);
}
	
int AvrUsartDriver::setup(int32_t baud, Uart::Parity parity, Uart::StopBits stopBits, Uart::DataBits dataBits) {
	m_regs->ucsr_a |= 1 << U2X0;
	uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
  
	m_regs->ubrr_h = baud_setting >> 8;
	m_regs->ubrr_l = baud_setting;

// PARITY
//	bits_mask_assign_bias(m_regs->ucsr_c, mode, 0b11, 4);

// STOP BITS
//	bits_mask_assign_bias(m_regs->ucsr_c, mode, 0b1, 3);

// DATA BITS
//	bits_mask_assign_bias(m_regs->ucsr_c, mode, 0b011, 1);
//	bits_mask_assign(m_regs->ucsr_b, mode, 0b100);
}

void AvrUsartDriver::setIrqHandlers(
	IRQHandler irqHandlerRX, void* argRX,
	IRQHandler irqHandlerTX, void* argTX, 
	IRQHandler irqHandlerTC, void* argTC
	) {
	setIRQHandler(m_irqbase + 0, irqHandlerRX, argRX);
	setIRQHandler(m_irqbase + 1, irqHandlerTX, argTX);
	setIRQHandler(m_irqbase + 2, irqHandlerTC, argTC);
}

}

#endif