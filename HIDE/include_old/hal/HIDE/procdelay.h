#ifndef GENOS_PROC_DELAY_H
#define GENOS_PROC_DELAY_H

#include <inttypes.h>
#include <genos/compiler.h>

__BEGIN_DECLS

void processor_delay_us(uint16_t);
void processor_delay_ms(uint16_t);
void processor_delay_s(uint16_t);

__END_DECLS

#endif