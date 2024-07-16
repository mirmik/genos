#include <sys/cdefs.h>

__BEGIN_DECLS

int stm32_diag_putchar(void* priv_usart, char c);
void stm32_diag_init(USART_TypeDef * regs);

__END_DECLS