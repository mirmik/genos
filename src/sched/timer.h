#ifndef GENOS_TIMER_TASKLET_H
#define GENOS_TIMER_TASKLET_H

#include <igris/datastruct/dlist.h>
#include <igris/sync/syslock.h>
#include <systime/systime.h>

struct ktimer;

typedef void(* ktimer_callback_t)(void* arg, struct ktimer * tim);

struct ktimer
{
	struct dlist_head lnk;
	ktimer_callback_t act;
	void * arg;
	time_t start;
	time_t interval;
};

#define KTIMER_DECLARE(name, act, arg, interval) \
struct ktimer name = { DLIST_HEAD_INIT(name.lnk), act, arg, 0, interval }

__BEGIN_DECLS

void timer_manager_step();

struct ktimer * ktimer_create(ktimer_callback_t act, void* arg,
                              time_t start, time_t interval);

struct ktimer * ktimer_create_for(ktimer_callback_t act, void* arg,
                                  time_t interval);

struct ktimer * ktimer_create_for_milliseconds(ktimer_callback_t act, void* arg,
        uint32_t ms);

void ktimer_dealloc(struct ktimer * ptr);

void ktimer_plan(struct ktimer * t);

// Сместить временную отметку начала отсчета с тем, чтобы обеспечить
// срабатывание таймера через равные периоды времени.
void ktimer_swift(struct ktimer * t);

// Сместить временную отметку и перепланировать таймер.
static inline
void ktimer_replan(struct ktimer * t)
{
	ktimer_swift(t);
	ktimer_plan(t);
}

__END_DECLS

#endif