#ifndef NUCLEO_BOARDS
#define NUCLEO_BOARDS

#include <sys/cdefs.h>
#include <drivers/gpio/pin.h>
#include <drivers/serial/stm32_usart.h>

#define HSE_FREQ 24000000

#define SYSLED_GPIO GPIOA
#define SYSLED_MASK (1<<5)

#if defined(BOARD_NUCLEO_G474RE)
#	define SYSUART LPUART1
#	define SYSUART_IRQ LPUART1_IRQn
#	define SYSUART_RX_GPIO GPIOA
#	define SYSUART_TX_GPIO GPIOA
#	define SYSUART_RX_PIN 2
#	define SYSUART_TX_PIN 3
#	define SYSUART_TX_AF 12
#	define SYSUART_RX_AF 12
#else
#	error "unregistred board"
#endif

namespace board 
{
	extern gpio_pin sysled;
	extern genos::stm32_usart sysuart;
}

__BEGIN_DECLS

void board_init(int freqmode=0);

__END_DECLS

#endif