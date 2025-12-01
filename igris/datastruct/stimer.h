/**
@file
    Таймер для работы в рамках флагового автомата.
    Не имеет управляющих структур, не зависит от какого-либо
    диспетчера
*/

#ifndef IGRIS_DATASTRUCT_STIMER_H
#define IGRIS_DATASTRUCT_STIMER_H

#include <igris/compiler.h>

struct stimer_head
{
    long start;
    long interval;
    int planed;
};
typedef struct stimer_head stimer_t;

__BEGIN_DECLS

// Инициализировать таймер
void stimer_init(struct stimer_head *timer, long start, long interval);

// Инициализировать и активировать таймер
void stimer_plan(struct stimer_head *timer, long start, long interval);

// Сменить точку старта и запланировать
void stimer_start(struct stimer_head *timer, long start);

// Проверить, сработал ли таймер
int stimer_check(struct stimer_head *timer, long curtime);

// Сместить start на значение interval, чтобы отмерить следующий
// квант времени
void stimer_swift(struct stimer_head *timer);

// Расчитать точку завершения кванта.
unsigned long stimer_finish(struct stimer_head *timer);

__END_DECLS

// Переодически выполняемое действие.
#define STIMER_PERIODIC(tim, curtime)                                          \
    if (stimer_check(tim, curtime) && (stimer_swift(tim), 1))

#endif
