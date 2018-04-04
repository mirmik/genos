#include <drivers/serial/stm32/UartDriver.h>
#include <communication/ttycls//SerialDriver.h>
#include <drivers/haldrivers/usart.h>
#include <gxx/util/bits.h>

#include <assert.h>

#include <genos/hal/irq.h>
#include <util/stub.h>

STM32HardwareUsart::STM32HardwareUsart(usart_regs* regs, uint8_t irqno) : m_regs(regs), m_irqno(irqno) {
}

int STM32HardwareUsart::putc(char c) {
	return usart_putc(m_regs,c);
}

int STM32HardwareUsart::getc() {
	return usart_getc(m_regs);
}

int STM32HardwareUsart::avail() {
	return usart_avail(m_regs);
}

int STM32HardwareUsart::room() {
	return usart_room(m_regs);
}

int STM32HardwareUsart::enable(bool en) {
	bits_lvl_mask(m_regs->CR1, USART_CR1_UE, en);
	irq_enable(m_irqno);
}

int STM32HardwareUsart::enableRX(bool en) {
	bits_lvl_mask(m_regs->CR1, USART_CR1_RE, en);
}

int STM32HardwareUsart::enableTX(bool en) {
	bits_lvl_mask(m_regs->CR1, USART_CR1_TE, en);
}

int STM32HardwareUsart::irqEnableRX(bool en) {
	assert(driver);
	bits_lvl_mask(m_regs->CR1, USART_CR1_RXNEIE, en);
}

int STM32HardwareUsart::irqEnableTX(bool en) {
	assert(driver);
	bits_lvl_mask(m_regs->CR1, USART_CR1_TXEIE, en);
}

int STM32HardwareUsart::irqEnableTC(bool en) {
	assert(driver);
	bits_lvl_mask(m_regs->CR1, USART_CR1_TCIE, en);
}
	
int STM32HardwareUsart::setup(UartParams *params) {
	setIRQHandler(m_irqno, (IRQHandler) STM32HardwareUsart_interruptHandler, this);
}

void STM32HardwareUsart_interruptHandler(STM32HardwareUsart* usart) {
	//Rx Handler
	if (usart->m_regs->SR & USART_SR_RXNE) {
		usart->driver->rxHandler(usart->m_regs->DR);
		return;
	}

	//Tx Handler
	if (usart->m_regs->SR & USART_SR_TXE) {
		usart->driver->txHandler();
		return;
	}

	//TC Handler
	if (usart->m_regs->SR & USART_SR_TC) {
		usart->driver->tcHandler();
		return;
	}

	panic("UNRESOLVED_STM32USART_INTERRUPT");
}