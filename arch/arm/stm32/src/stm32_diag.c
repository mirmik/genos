#include <diag/diag.h>
#include <periph/map.h>

static struct diag_ops diag_operations; 

int stm32_diag_putchar(void* priv_usart, char c) 
{
	struct usart_regs* usart = (struct usart_regs*) priv_usart;
	
	while (!(USART2->SR & (1 << 7)));
	USART2->DR = c;
	while (!(USART2->SR & (1 << 7)));
}

void stm32_diag_init(void) 
{
	diag_operations.putc = stm32_diag_putchar;
	diag_operations.write = diag_write_common;
	diag_setup(&diag_operations, USART2);
}
