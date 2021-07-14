/**
@file
    Структура таймера для использования совместно с менеджером таймеров.
*/

#ifndef GENOS_KTIMER_H
#define GENOS_KTIMER_H

#include <igris/compiler.h>
#include <igris/osinter/ctrobj.h>
#include <igris/systime.h> // for types

struct ktimer_head;
typedef void (*ktimer_callback_t)(void *arg, struct ktimer_head *tim);

struct ktimer_head
{
    struct ctrobj ctr;

    systime_t start;
    systime_difference_t interval;

    ktimer_callback_t callback;
    void *privdata;
};
typedef struct ktimer_head ktimer_t;

__BEGIN_DECLS

// Инициализировать таймер
void ktimer_init(struct ktimer_head *timer, ktimer_callback_t callback,
                 void *privdata, systime_t start, systime_difference_t interval);

void ktimer_deinit(struct ktimer_head *timer);

// Инициализировать и активировать таймер
void ktimer_plan(struct ktimer_head *timer);

// Сменить точку старта и запланировать
void ktimer_restart(struct ktimer_head *timer, systime_t start);

// Проверить, сработал ли таймер
int ktimer_check(struct ktimer_head *timer, systime_t curtime);

// Сместить start на значение interval, чтобы отмерить следующий
// квант времени
void ktimer_swift(struct ktimer_head *timer);

// Расчитать точку завершения.
systime_t ktimer_finish(struct ktimer_head *timer);

void ktimer_manager_init();
void ktimer_manager_step(systime_t curtime);
unsigned int ktimer_manager_planed_count();

__END_DECLS

#endif
