#include <kernel/diag.h>
#include <drivers/stm32/rcc.h>
#include <drivers/haldrivers/usart.h>
#include <drivers/gpio.h>

#include <gxx/debug/delay.h>

int stm32_diag_init() {	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	gpio_settings(GPIOA, (1 << 2), GPIO_MODE_ALTERNATE);
	gpio_settings_alternate(GPIOA, (1 << 2), 0x7);
	//GPIOA->AFR[0] |= (0x7 << (4 * 2)); 

	USART2->BRR = 1458; 
	USART2->CR1 = USART_CR1_UE | USART_CR1_TE;  
}

int stm32_diag_putchar(char c) {
	while (!usart_room(USART2));
	usart_putc(USART2, c);
	while (!usart_room(USART2));
}

struct diag_ops diag_operations;
