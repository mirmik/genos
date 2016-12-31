#ifndef ATMEGA2560_DEVICES_H
#define ATMEGA2560_DEVICES_H

#include <periph/map.h>
#include <periph/dev/usart.h>
#include <compiler.h>

__BEGIN_DECLS

extern struct usart_device usart0;
extern struct usart_device usart1;
extern struct usart_device usart2;

__END_DECLS

#endif