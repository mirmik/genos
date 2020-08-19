#ifndef NUCLEO_BOARDS
#define NUCLEO_BOARDS

#include <sys/cdefs.h>
#include <drivers/gpio/pin.h>
#include <drivers/serial/stm32_usart.h>

#if defined(BOARD_NUCLEO_L432)
#	define SYSLED_GPIO GPIOB
#	define SYSLED_MASK (1<<3)
#	define DEBUG_USART USART1
#	define DEBUG_USART_IRQ USART1_IRQn
#	define DEBUG_USART_RX_GPIO GPIOA
#	define DEBUG_USART_TX_GPIO GPIOA
#	define DEBUG_USART_RX_PIN 9
#	define DEBUG_USART_TX_PIN 10
#	define DEBUG_USART_AF 7
#elif defined(BOARD_NUCLEO_F401RE)
#   define BOARD_HSE_FREQ 16000000
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

namespace board 
{
	extern gpio_pin sysled;
	extern genos::stm32_usart sysuart;
}

__BEGIN_DECLS

void board_init(int freqmode = 2);

__END_DECLS

#endif