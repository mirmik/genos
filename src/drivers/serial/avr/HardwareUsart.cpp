#ifndef GENOS_AVR_HARDWARE_USART_H
#define GENOS_AVR_HARDWARE_USART_H

#include "util/bits.h"
#include <drivers/serial/avr/UartDriver.h>


void AVRHardwareUsart_interruptHandlerTC(AVRHardwareUsart* usart);
void AVRHardwareUsart_interruptHandlerTX(AVRHardwareUsart* usart);
void AVRHardwareUsart_interruptHandlerRX(AVRHardwareUsart* usart);

AVRHardwareUsart::AVRHardwareUsart(usart_regs* regs, int irqbase) : m_regs(regs), m_irqbase(irqbase) {}

int AVRHardwareUsart::putc(char c) {
	m_regs->udr = c;
	return 1;
}

int AVRHardwareUsart::getc() {
	return m_regs->udr;
}

int AVRHardwareUsart::avail() {
	return bits_mask(m_regs->ucsr_a, (1 << RXC0));
}

int AVRHardwareUsart::room() {
	return bits_mask(m_regs->ucsr_a, (1 << UDRE0));
}

int AVRHardwareUsart::enable(bool en) {}

int AVRHardwareUsart::enableRX(bool en) {
	bits_lvl(m_regs->ucsr_b, (1 << RXEN0), en);
}

int AVRHardwareUsart::enableTX(bool en) {
	bits_lvl(m_regs->ucsr_b, (1 << TXEN0), en);
}

int AVRHardwareUsart::irqEnableRX(bool en) {
	bits_lvl(m_regs->ucsr_b, (1 << RXCIE0), en);
}

int AVRHardwareUsart::irqEnableTX(bool en) {
	bits_lvl(m_regs->ucsr_b, (1 << UDRIE0), en);
}

int AVRHardwareUsart::irqEnableTC(bool en) {
	bits_lvl(m_regs->ucsr_b, (1 << TXCIE0), en);
}
	
int AVRHardwareUsart::setup(UartParams *params) {
	auto baud = params->baudRate;

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

	setIRQHandler(m_irqbase + 0, (IRQHandler) AVRHardwareUsart_interruptHandlerRX, this);
	setIRQHandler(m_irqbase + 1, (IRQHandler) AVRHardwareUsart_interruptHandlerTX, this);
	setIRQHandler(m_irqbase + 2, (IRQHandler) AVRHardwareUsart_interruptHandlerTC, this);
}

void AVRHardwareUsart_interruptHandlerRX(AVRHardwareUsart* usart) {
	usart->driver->rxHandler(usart->m_regs->udr);
}

void AVRHardwareUsart_interruptHandlerTX(AVRHardwareUsart* usart) {
	usart->driver->txHandler();
}

void AVRHardwareUsart_interruptHandlerTC(AVRHardwareUsart* usart) {
	usart->driver->tcHandler();
}

#endif