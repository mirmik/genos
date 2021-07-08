/**
@file
    Структура таймера для использования совместно с менеджером таймеров.
*/

#ifndef GENOS_KTIMER_H
#define GENOS_KTIMER_H

#include <igris/compiler.h>
#include <igris/osinter/ctrobj.h>

struct ktimer_head;
typedef void (*ktimer_callback_t)(void *arg, struct ktimer_head *tim);

struct ktimer_head
{
    struct ctrobj ctr;

    uint64_t start;
    int64_t interval;

    ktimer_callback_t callback;
    void *privdata;
};
typedef struct ktimer_head ktimer_t;

__BEGIN_DECLS

// Инициализировать таймер
void ktimer_init(struct ktimer_head *timer, ktimer_callback_t callback,
                 void *privdata, uint64_t start, int64_t interval);

// Инициализировать и активировать таймер
void ktimer_plan(struct ktimer_head *timer);

// Сменить точку старта и запланировать
void ktimer_restart(struct ktimer_head *timer, uint64_t start);

// Проверить, сработал ли таймер
int ktimer_check(struct ktimer_head *timer, uint64_t curtime);

// Сместить start на значение interval, чтобы отмерить следующий
// квант времени
void ktimer_swift(struct ktimer_head *timer);

// Расчитать точку завершения.
uint64_t ktimer_finish(struct ktimer_head *timer);

void ktimer_manager_step(uint64_t curtime);

__END_DECLS

#endif
