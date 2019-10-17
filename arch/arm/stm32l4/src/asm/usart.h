#ifndef STM32_USART_H
#define STM32_USART_H

#include <sys/cdefs.h>
#include <igris/util/bits.h>
#include <igris/util/bug.h>

#include <asm/rcc.h>
#include <periph/map.h>

#include <defs/uartdefs.h>

__BEGIN_DECLS

void stm32_usart_enable(usart_regs_t* regs, int en);

uint8_t stm32_usart_clockbus(usart_regs_t* regs);

void stm32_usart_set_baudrate(usart_regs_t* regs, uint32_t baud);

void stm32_usart_set_parity(usart_regs_t* regs, char parity);

// Note: If parity bit is using you should set one more databit.
// F.E. nine databits for eight information bits and one for parity.
void stm32_usart_set_databits(usart_regs_t* regs, uint8_t databits);

void stm32_usart_set_stopbits(usart_regs_t* regs,
                              enum uart_stopbits_e stopbits);

// Активировать USART в базовом режиме с передаваемыми параметрами.
// Также проводит включение тактирования, если то выключено.
int stm32l4_usart_setup( usart_regs_t* regs, uint32_t baud,
                       char parity, uint8_t databits, uint8_t stopbits);

void stm32_usart_txirq_enable(usart_regs_t* regs, int en);
void stm32_usart_tcirq_enable(usart_regs_t* regs, int en);
void stm32_usart_rxirq_enable(usart_regs_t* regs, int en);

static inline
int stm32_usart_putc(usart_regs_t * usart, char c)
{
	usart->TDR = c;
	return 1;
}

static inline
int stm32_usart_getc(usart_regs_t * usart)
{
	return usart->RDR;
}

static inline
int stm32_usart_room(usart_regs_t * usart)
{
	return usart->ISR & (1 << 7); //TXNE byte
}

static inline
int stm32_usart_avail(usart_regs_t * usart)
{
	return usart->ISR & (1 << 5); //RXNE byte
}

static inline
int stm32_rxirq_status(usart_regs_t * usart) 
{
	return usart->ISR & USART_ISR_RXNE;
}

static inline
int stm32_txirq_status(usart_regs_t * usart) 
{
	return usart->ISR & USART_ISR_TXE;
}

static inline
int stm32_tcirq_status(usart_regs_t * usart) 
{
	return usart->ISR & USART_ISR_TC;
}

void stm32_usart_debug_print(usart_regs_t * usart);

__END_DECLS

#endif