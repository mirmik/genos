#include <hal/board.h>

#include <asm/irq.h>
#include <asm/avr_gpio.h>

#include <igris/util/cpu_delay.h>
#include <defs/gpiodefs.h>

void board_init() 
{
	avr_gpio_mode(SYSLED_GPIO, SYSLED_MASK, GPIO_MODE_OUTPUT);
	avr_gpio_set(SYSLED_GPIO, SYSLED_MASK, 0);

	avr_usart_setup(SYSUSART, 115200, 'n', 8, 1);
	avr_usart_enable_tx(SYSUSART, 1);
}

#define CPU_DELAY_ARGUMENT 50000
void board_test() 
{
	irqs_disable();
	while(1) 
	{
		avr_gpio_set(SYSLED_GPIO, SYSLED_MASK, 1);
		cpu_delay(CPU_DELAY_ARGUMENT);
		avr_gpio_set(SYSLED_GPIO, SYSLED_MASK, 0);
		cpu_delay(CPU_DELAY_ARGUMENT);

		avr_usart_sendbyte(SYSUSART, 'A');
	}
}