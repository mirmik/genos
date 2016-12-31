#ifndef GENOS_AVR_MAP_ATMEGA2560_H
#define GENOS_AVR_MAP_ATMEGA2560_H

#define USART_BASE_0 0xC0
#define USART_BASE_1 0xC8
#define USART_BASE_2 0xD0

#define USART0 ((volatile struct usart_regs *)(USART_BASE_0))
#define USART1 ((volatile struct usart_regs *)(USART_BASE_1))
#define USART2 ((volatile struct usart_regs *)(USART_BASE_2))

#endif