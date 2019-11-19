#ifndef NUCLEO_BOARDS
#define NUCLEO_BOARDS

#include <sys/cdefs.h>
#include <drivers/gpio/pin.h>
#include <drivers/serial/stm32_usart.h>

#define HSE_FREQ 8000000

#if defined(BOARD_NUCLEO_L432)
#	define BOARD_LED_GPIO GPIOB
#	define BOARD_LED_PIN 3
#	define DEBUG_USART USART1
#	define DEBUG_USART_IRQ STM32_IRQ_USART1
#	define DEBUG_USART_RX_GPIO GPIOA
#	define DEBUG_USART_TX_GPIO GPIOA
#	define DEBUG_USART_RX_PIN 9
#	define DEBUG_USART_TX_PIN 10
#	define DEBUG_USART_AF GPIO_AF_USART1
#elif defined(BOARD_NUCLEO_F401RE)
#	define BOARD_LED_GPIO GPIOA
#	define BOARD_LED_PIN 5
#	define DEBUG_USART USART2
#	define DEBUG_USART_IRQ STM32_IRQ_USART2
#	define DEBUG_USART_RX_GPIO GPIOA
#	define DEBUG_USART_TX_GPIO GPIOA
#	define DEBUG_USART_RX_PIN 2
#	define DEBUG_USART_TX_PIN 3
#	define DEBUG_USART_AF GPIO_AF_USART2
/*#elif defined(BOARD_NUCLEO_F412RB)
#	define BOARD_LED_GPIO GPIOB
#	define BOARD_LED_PIN 13
#	define DEBUG_USART USART2
#	define DEBUG_USART_IRQ STM32_IRQ_USART2
#	define DEBUG_USART_RX_GPIO GPIOA
#	define DEBUG_USART_TX_GPIO GPIOA
#	define DEBUG_USART_RX_PIN 2
#	define DEBUG_USART_TX_PIN 3
#	define DEBUG_USART_AF GPIO_AF_USART2*/
#else
#	error "unregistred board"
#endif


extern struct gpio_pin board_led;

namespace board 
{
	static gpio_pin sysled = board_led;
	extern stm32_usart_device sysusart;
}

__BEGIN_DECLS

void board_init(int freqmode = 2);

__END_DECLS

#endif