#include <diag/diag.h>
#include <arch/rcc.h>
#include <periph/map.h>

int stm32_diag_putchar(char c) {
	while (!(USART2->SR & (1 << 7)));
	USART2->DR = c;
	while (!(USART2->SR & (1 << 7)));
}

struct diag_ops diag_operations;
