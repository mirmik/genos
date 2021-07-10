#ifndef RL_SYNCRO_V2_BOARD
#define RL_SYNCRO_V2_BOARD

#include <sys/cdefs.h>
#include <drivers/serial/stm32_usart.h>
//#include <drivers/serial/stm32_usart.h>
#include <drivers/gpio/pin.h>

#define HSE_FREQ 8000000
#define GPIO_AF_USART2 0x07
#define GPIO_AF_USART6 0x08

extern stm32_usart_device usart2;
extern stm32_usart_device usart6;

extern struct gpio_pin board_led;
extern struct gpio_pin board_led2;
extern struct gpio_pin extpin[4];

namespace board 
{
	extern stm32_usart_device sysuart;
	extern genos::gpio_pin sysled;
}

__BEGIN_DECLS

void board_init(int mode = 0);

__END_DECLS

#endif