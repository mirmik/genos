#include <diag/diag.h>
#include <periph/map.h>

static struct diag_ops diag_operations; 

int stm32l4_diag_putchar(void* priv_usart, char c) 
{
	usart_regs_t* usart = (usart_regs_t*) priv_usart;
	
	while (!(usart->ISR & USART_ISR_TXE));
	usart->TDR = c;
	while (!(usart->ISR & USART_ISR_TXE));

	return 1;
}

void stm32l4_diag_init(usart_regs_t* regs) 
{
	diag_operations.putc = stm32l4_diag_putchar;
	diag_operations.write = diag_write_common;
	diag_setup(&diag_operations, regs);
}
