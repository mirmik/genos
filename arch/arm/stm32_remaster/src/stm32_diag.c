#include <diag/diag.h>
#include <periph/map.h>

#include <asm/stm32_usart.h>

static struct diag_ops diag_operations; 

int stm32_diag_putchar(void* priv_usart, char c) 
{
	USART_TypeDef * usart = (USART_TypeDef *) priv_usart;
	
	//while (!(usart->SR & (1 << 7)));
	stm32_usart_wait_cantx(usart);
	stm32_usart_putc(usart, c);
	stm32_usart_wait_cantx(usart);
	//while (!(usart->SR & (1 << 7)));

	return 1;
}

void stm32_diag_init(USART_TypeDef * regs) 
{
	diag_operations.putc = stm32_diag_putchar;
	diag_operations.write = diag_write_common;
	diag_setup(&diag_operations, regs);
}
