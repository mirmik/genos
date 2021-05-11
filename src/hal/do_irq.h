#ifndef GXX_HAL_DO_IRQ_H
#define GXX_HAL_DO_IRQ_H

#include <igris/compiler.h>
#include <inttypes.h>

__BEGIN_DECLS
void do_irq(uint8_t irq) __attribute__((section(".handlers")));
unsigned char is_interrupt_context();
__END_DECLS

#endif