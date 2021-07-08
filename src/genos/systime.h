/**
@file
Обобщённые системные часы. Работают с системным таймером, вызывающим в прерывании
функцию system_tick,

Старшая часть отметки времени берётся по количиству тиков, jiffies. Младшая часть,
если это требуется, запрашивается у функции systime_lopart(), определённой аппаратно
зависимо.
*/

#ifndef GENOS_SYSTIME_H
#define GENOS_SYSTIME_H

#include <inttypes.h>
#include <igris/compiler.h>
#include <genos/jiffies.h>

#if __has_include(<asm/delay.h>)
#include <asm/delay.h>
#endif

extern volatile jiffies_t __jiffies;

__BEGIN_DECLS

int64_t millis();
int64_t micros();
int64_t nanos();

void delay(double d);

// Частота прерывания таймер
void systime_set_frequency(uint32_t freq);
uint32_t systime_frequency();

jiffies_t jiffies();
jiffies_t millis();
jiffies_t micros();

jiffies_pair_t jiffies_pair();
int jiffies_pair_compare(jiffies_pair_t a, jiffies_pair_t b);
jiffies_t jiffies_pair_to_micros(jiffies_pair_t pair);

jiffies_t ms2jiffies(uint32_t ms);

__END_DECLS

#endif
