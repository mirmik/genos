#ifndef STM32_USART_H
#define STM32_USART_H

#include <sys/cdefs.h>
#include <igris/util/bits.h>
#include <igris/util/bug.h>

#include <asm/stm32_rcc.h>
#include <periph/map.h>

#include <defs/uartdefs.h>

__BEGIN_DECLS

void stm32_usart_enable(USART_TypeDef * regs, int en);

uint8_t stm32_usart_clockbus(USART_TypeDef * regs);

void stm32_usart_set_baudrate(USART_TypeDef * regs, uint32_t baud);

void stm32_usart_set_parity(USART_TypeDef * regs, char parity);

// Note: If parity bit is using you should set one more databit.
// F.E. nine databits for eight information bits and one for parity.
void stm32_usart_set_databits(USART_TypeDef * regs, uint8_t databits);

void stm32_usart_set_stopbits(USART_TypeDef * regs,
                              enum uart_stopbits_e stopbits);

// Активировать USART в базовом режиме с передаваемыми параметрами.
// Также проводит включение тактирования, если то выключено.
int stm32_usart_setup( USART_TypeDef * regs, uint32_t baud,
                       char parity, uint8_t databits, uint8_t stopbits);

void stm32_usart_txirq_enable(USART_TypeDef * regs, int en);
void stm32_usart_tcirq_enable(USART_TypeDef * regs, int en);
void stm32_usart_rxirq_enable(USART_TypeDef * regs, int en);

void stm32_drop_overrun_flag(USART_TypeDef * regs);

int stm32_usart_putc(USART_TypeDef * usart, char c);
int stm32_usart_getc(USART_TypeDef * usart);
int stm32_usart_room(USART_TypeDef * usart);
int stm32_usart_avail(USART_TypeDef * usart);

int stm32_rxirq_status(USART_TypeDef * usart);
int stm32_txirq_status(USART_TypeDef * usart);
int stm32_tcirq_status(USART_TypeDef * usart);
int stm32_overrun_irq_status(USART_TypeDef * usart);

void stm32_usart_debug_print(USART_TypeDef * usart);
void stm32_usart_wait_cantx(USART_TypeDef * usart);
void stm32_usart_wait_canrx(USART_TypeDef * usart);

void stm32_debug_print_usart_interrupt_status_register(USART_TypeDef * regs);

__END_DECLS

#endif