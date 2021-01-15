#include <curboard.h>

#include <asm/stm32_rcc.h>
#include <asm/stm32_gpio.h>

#include <hal/arch.h>
#include <drivers/gpio/pin.h>
#include <drivers/serial/stm32_usart.h>

#include <util/cpu_delay.h>

gpio_pin board_led = {GPIOI, (1<<1)};

stm32_usart_device board_usart(USART1, USART1_IRQn);

void board_init() 
{
	arch_init();

	stm32_clockbus_freq[CLOCKBUS_HSI] = 16000000;
	stm32_clockbus_freq[CLOCKBUS_HSE] = 16000000;
	stm32_clockbus_freq[CLOCKBUS_MSI] = 16000000;
	stm32_clockbus_freq[CLOCKBUS_PLL] = 16000000;
	stm32_clockbus_freq[CLOCKBUS_APB1] = 16000000;
	stm32_clockbus_freq[CLOCKBUS_APB2] = 16000000;

	stm32_rcc_enable_gpio(GPIOA);
	stm32_rcc_enable_gpio(GPIOI);

	board_led.mode(GPIO_MODE_OUTPUT);
	board_led.set(1);


	gpio_settings(GPIOA, (1<<9), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_alternate(GPIOA, (1<<9), 7);

	board_usart.setup(115200, 'n', 8, 1);

	while(1) {
		board_usart.sendbyte('A');
		cpu_delay(1000000);
		board_led.set(1);
		board_usart.sendbyte('B');
		cpu_delay(1000000);
		board_led.set(0);
	}
}