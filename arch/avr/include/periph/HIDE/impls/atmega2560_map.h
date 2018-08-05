#ifndef GENOS_AVR_MAP_ATMEGA2560_H
#define GENOS_AVR_MAP_ATMEGA2560_H

#include <periph/regs/usart.h>
#include <periph/regs/gpio.h>
#include <periph/regs/twi.h>
#include <periph/irqdefs.h>

#define GPIOA ((struct gpio_regs *)(0x20))
#define GPIOB ((struct gpio_regs *)(0x23))
#define GPIOC ((struct gpio_regs *)(0x26))
#define GPIOD ((struct gpio_regs *)(0x29))
#define GPIOE ((struct gpio_regs *)(0x2C))
#define GPIOF ((struct gpio_regs *)(0x2F))
#define GPIOG ((struct gpio_regs *)(0x32))
#define GPIOH ((struct gpio_regs *)(0x100))
#define GPIOJ ((struct gpio_regs *)(0x103))
#define GPIOK ((struct gpio_regs *)(0x106))
#define GPIOL ((struct gpio_regs *)(0x109))

#define TWI ((struct twi_regs*)(0xBD))

#define USART0 ((struct usart_regs *)(0xC0))
#define USART1 ((struct usart_regs *)(0xC8))
#define USART2 ((struct usart_regs *)(0xD0))

/*struct usart_data {
	struct usart_regs* regs;
	int8_t irqbase;
};

static const struct usart_data usart0_data = { USART0, ATMEGA_IRQ_U0RX };
static const struct usart_data usart1_data = { USART1, ATMEGA_IRQ_U1RX };
static const struct usart_data usart2_data = { USART2, ATMEGA_IRQ_U2RX };
*/

#endif