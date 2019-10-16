#ifndef NUCLEO_BOARDS
#define NUCLEO_BOARDS

#include <sys/cdefs.h>
#include <drivers/gpio/pin.h>
#include <drivers/serial/stm32_usart.h>

#define HSE_FREQ 8000000

#if defined(BOARD_NUCLEO_L412RB)
#	define BOARD_LED_GPIO GPIOB
#	define BOARD_LED_PIN 13
#	define DEBUG_USART USART2
#	define DEBUG_USART_IRQ USART2_IRQn
#	define DEBUG_USART_RX_GPIO GPIOA
#	define DEBUG_USART_TX_GPIO GPIOA
#	define DEBUG_USART_RX_PIN 2
#	define DEBUG_USART_TX_PIN 3
#	define DEBUG_USART_AF GPIO_AF_USART2
#else
#	error "unregistred board"
#endif

extern struct gpio_pin board_led;
//extern struct stm32l4_usart_device usart2;

namespace board 
{
	static gpio_pin sysled = board_led;
	//static stm32_usart_device & sysusart = usart2;
}

__BEGIN_DECLS

void board_init();

__END_DECLS

#endif