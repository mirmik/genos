#ifndef STM32_USART_H
#define STM32_USART_H

#include <sys/cdefs.h>
#include <periph/regs/usart.h>

__BEGIN_DECLS

uint32_t stm32_usart_declared_freq(struct usart_regs* regs);

static inline
void stm32_usart_set_baudrate(struct usart_regs* regs, uint32_t baud, uint32_t busfreq)
{
	regs->BRR = busfreq / baud;
}

static inline 
int stm32_usart_setup(
	struct usart_regs* regs, 
	uint32_t baud, 
	char parity, 
	uint8_t databits, 
	uint8_t stopbits) 
{
	uint32_t freq = stm32_usart_declared_freq(regs);
	stm32_usart_set_baudrate(regs, baud, freq); 
}

static inline
void stm32_usart_rxirq_enable(struct usart_regs* regs, int en) 
{

}

__END_DECLS

#endif