#include <periph/devices.h>
#include <asm/irq.h>

struct usart_device usart0 = {USART0, {.tx = ATMEGA_IRQ_U0TX, .rx = ATMEGA_IRQ_U0RX, .udre = ATMEGA_IRQ_U0DRE}};
struct usart_device usart1 = {USART1, {.tx = ATMEGA_IRQ_U1TX, .rx = ATMEGA_IRQ_U1RX, .udre = ATMEGA_IRQ_U1DRE}};
struct usart_device usart2 = {USART2, {.tx = ATMEGA_IRQ_U2TX, .rx = ATMEGA_IRQ_U2RX, .udre = ATMEGA_IRQ_U2DRE}};
