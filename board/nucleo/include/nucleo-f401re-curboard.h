#ifndef NUCLEO_BOARDS
#define NUCLEO_BOARDS

#include <sys/cdefs.h>
#include <drivers/gpio/pin.h>
#include <drivers/serial/stm32_usart.h>

#   define BOARD_HSE_FREQ 8000000
#	define SYSLED_GPIO GPIOA
#	define SYSLED_MASK (1<<5)
#	define SYSUART USART2
#	define SYSUART_IRQ USART2_IRQn
#	define SYSUART_RX_GPIO GPIOA
#	define SYSUART_TX_GPIO GPIOA
#	define SYSUART_RX_PIN 2
#	define SYSUART_TX_PIN 3
#	define SYSUART_TX_AF 7
#	define SYSUART_RX_AF 7

namespace board 
{
	extern gpio_pin sysled;
	extern genos::stm32_usart sysuart;
}

__BEGIN_DECLS

void board_init(int freqmode = 2);

__END_DECLS

#endif