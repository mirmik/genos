#ifndef GENOS_SYSTIME_H
#define GENOS_SYSTIME_H

#include <compiler.h>
#include <inttypes.h>

__BEGIN_DECLS

uint32_t seconds();
uint32_t millis();
uint32_t micros();

void delay(uint32_t ms);
void delayMicroseconds(uint16_t us);

__END_DECLS

#endif