#ifndef STM32_USART_H
#define STM32_USART_H

#include <sys/cdefs.h>
#include <igris/util/bits.h>
#include <igris/util/bug.h>

#include <asm/rcc.h>
#include <periph/regs/usart.h>

#include <defs/uartdefs.h>

__BEGIN_DECLS

void stm32_usart_enable(struct usart_regs* regs, int en);

uint8_t stm32_usart_clockbus(struct usart_regs* regs);

void stm32_usart_set_baudrate(struct usart_regs* regs, uint32_t baud);

void stm32_usart_set_parity(struct usart_regs* regs, char parity);

// Note: If parity bit is using you should set one more databit.
// F.E. nine databits for eight information bits and one for parity.
void stm32_usart_set_databits(struct usart_regs* regs, uint8_t databits);

void stm32_usart_set_stopbits(struct usart_regs* regs,
                              enum uart_stopbits_e stopbits);

// Активировать USART в базовом режиме с передаваемыми параметрами.
// Также проводит включение тактирования, если то выключено.
int stm32_usart_setup( struct usart_regs* regs, uint32_t baud,
                       char parity, uint8_t databits, uint8_t stopbits);

static inline
void stm32_usart_rxirq_enable(struct usart_regs* regs, int en)
{

}

__END_DECLS

#endif