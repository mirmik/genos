#ifndef GENOS_DRIVERS_USART_H
#define GENOS_DRIVERS_USART_H

#include <sys/cdefs.h>
#include "inttypes.h"
#include "periph/regs/usart.h"

__BEGIN_DECLS

int usart_putc(struct usart_regs * usart, char c);
int usart_getc(struct usart_regs * usart);

int usart_room(struct usart_regs * usart);
int usart_avail(struct usart_regs * usart);


__END_DECLS

#endif