#include <kernel/diag.h>
#include <drivers/stm32/rcc.h>
#include <drivers/haldrivers/usart.h>
#include <drivers/gpio.h>

#include <debug/delay.h>

int stm32_diag_putchar(char c) {
	while (!(USART2->SR & (1 << 7)));
	USART2->DR = c;
	while (!(USART2->SR & (1 << 7)));
}

struct diag_ops diag_operations;
