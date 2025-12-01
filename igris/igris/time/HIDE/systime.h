/**
@file
Обобщённые системные часы. Работают с системным таймером, вызывающим в
прерывании функцию system_tick,

Старшая часть отметки времени берётся по количиству тиков, jiffies. Младшая
часть, если это требуется, запрашивается у функции systime_lopart(),
определённой аппаратно зависимо.
*/

#ifndef IGRIS_TIME_TIME_H
#define IGRIS_TIME_TIME_H

#include <igris/compiler.h>
#include <igris/time/jiffies.h>
#include <inttypes.h>

#if __has_include(<asm/delay.h>)
#include <asm/delay.h>
#endif

// Тип знаковый. Снижает вероятность ошибок при сравнении двух значений.
typedef int64_t jiffies_t;

typedef struct jiffies_pair_s
{
    jiffies_t hi;
    jiffies_t lo;
} jiffies_pair_t;

extern volatile jiffies_t __jiffies;

__BEGIN_DECLS

int64_t millis();
int64_t micros();
int64_t nanos();

void delay(double d);

// Частота прерывания таймер
void systime_set_frequency(uint32_t freq);
uint32_t systime_frequency();

void system_tick();

jiffies_t jiffies();
jiffies_t millis();
jiffies_t micros();

jiffies_pair_t jiffies_pair();
int jiffies_pair_compare(jiffies_pair_t a, jiffies_pair_t b);
jiffies_t jiffies_pair_to_micros(jiffies_pair_t pair);

jiffies_t ms2jiffies(uint32_t ms);

__END_DECLS

#endif

#endif
