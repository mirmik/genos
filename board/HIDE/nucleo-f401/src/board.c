#include <drivers/gpio.h>

#include <genos/hal/board.h>
#include <genos/hal/arch.h>
#include <genos/hal/irq.h>

#include <gxx/debug/delay.h>
#include <gxx/debug/dprint.h>

#include <asm/rcc.h>

int32_t settings_generator_frequency;

static int board_diag_init(uint32_t brr) {	
	//RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	gpio_settings(GPIOA, (1 << 2), GPIO_MODE_ALTERNATE);
	gpio_settings(GPIOA, (1 << 3), GPIO_MODE_ALTERNATE);
	gpio_settings_alternate(GPIOA, (1 << 2), 0x7);
	gpio_settings_alternate(GPIOA, (1 << 3), 0x7);
	//GPIOA->AFR[0] |= (0x7 << (4 * 2)); 

	USART2->BRR = brr; 
	USART2->CR1 = USART_CR1_UE | USART_CR1_TE;  
}

void board_init() {
	arch_init();
	settings_generator_frequency = arch_external_generator_init(8, 168, 2, 7, 8000000); //84Mhz
	
	rcc_enable_gpio(GPIOA);
	rcc_enable_usart(USART2);

	board_diag_init(729); //USART2, 115200 baud

	pin_settings(USER_LED, GPIO_MODE_OUTPUT);
	pin_set_level(USER_LED, 1);
}

void board_shutdown(arch_shutdown_mode_t mode) {
	emergency_stop();
	switch(mode)
	{
		case ARCH_SHUTDOWN_MODE_HALT:
		break;
		case ARCH_SHUTDOWN_MODE_REBOOT:
		break;
		case ARCH_SHUTDOWN_MODE_ABORT:
			global_irqs_disable();
			pin_settings(USER_LED, GPIO_MODE_OUTPUT);
			debug_print("arch_shutdown"); dln();
			while(1) {
				pin_tgl_level(USER_LED);
				debug_delay(100);
			}
		break;
	};
	arch_shutdown(mode);	
}

__attribute__((weak)) void emergency_stop() {}