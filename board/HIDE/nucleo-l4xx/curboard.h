#ifndef NUCLEO_BOARDS
#define NUCLEO_BOARDS

#include <sys/cdefs.h>

#include <drivers/gpio/pin.h>
#include <drivers/serial/stm32l4_usart.h>

#include <periph/map.h>

#define HSE_FREQ 8000000

#if defined(BOARD_NUCLEO_L412RB)
#	define BOARD_LED_GPIO GPIOB
#	define BOARD_LED_PIN 13
#	define DEBUG_USART LPUART1
#	define DEBUG_USART_IRQ LPUART1_IRQn
#	define DEBUG_USART_RX_GPIO GPIOA
#	define DEBUG_USART_TX_GPIO GPIOA
#	define DEBUG_USART_RX_PIN 2
#	define DEBUG_USART_TX_PIN 3
#	define DEBUG_USART_TX_AF 8
#	define DEBUG_USART_RX_AF 8
#elif defined(BOARD_NUCLEO_L432KC)
#	define BOARD_LED_GPIO GPIOB
#	define BOARD_LED_PIN 3
#	define DEBUG_USART LPUART1
#	define DEBUG_USART_IRQ LPUART1_IRQn
#	define DEBUG_USART_RX_GPIO GPIOA
#	define DEBUG_USART_TX_GPIO GPIOA
#	define DEBUG_USART_RX_PIN 15
#	define DEBUG_USART_TX_PIN 2
#	define DEBUG_USART_TX_AF 8
#	define DEBUG_USART_RX_AF 8
#else
#	error "unregistred board"
#endif

extern struct gpio_pin board_led;

namespace board 
{
	static gpio_pin sysled = board_led;
	extern stm32l4_usart_device sysuart;
	
#ifdef BOARD_NUCLEO_L432KC
	extern stm32l4_usart_device usart1;
#endif
	//static stm32l4_usart_device & sysuart = sysuart;
}

__BEGIN_DECLS

void board_init(int freqmode = 2);

__END_DECLS

#endif