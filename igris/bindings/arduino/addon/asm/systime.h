#ifndef ARDUINO_ASM_SYSTIME_H
#define ARDUINO_ASM_SYSTIME_H

#include <Arduino.h>

typedef unsigned long systime_t;
typedef long systime_difference_t;

__BEGIN_DECLS

static inline systime_t system_time()
{
    return millis();
} // TODO Всего на 50 дней. Расширить
static inline uint64_t systime_millis()
{
    return millis();
} // TODO Всего на 50 дней. Расширить
static inline float systime_frequency() { return 1000; }

__END_DECLS

#endif
