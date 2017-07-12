#ifndef AVR_DIAG_H
#define AVR_DIAG_H

#include <genos/compiler.h>

__BEGIN_DECLS

int usart0_diag_init();
int usart0_diag_putchar(int c);

__END_DECLS

#endif