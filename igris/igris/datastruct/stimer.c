#include <igris/datastruct/stimer.h>

int stimer_check(struct stimer_head *timer, long curtime)
{
    return timer->planed && (curtime - timer->start >= timer->interval);
}

void stimer_init(struct stimer_head *timer, long start, long interval)
{
    timer->start = start;
    timer->interval = interval;
    timer->planed = 0;
}

void stimer_swift(struct stimer_head *timer)
{
    timer->start += timer->interval;
}

unsigned long stimer_finish(struct stimer_head *timer)
{
    return timer->start + timer->interval;
}

void stimer_plan(struct stimer_head *timer, long start, long interval)
{
    stimer_init(timer, start, interval);
    timer->planed = 1;
}

void stimer_start(struct stimer_head *timer, long start)
{
    timer->start = start;
    timer->planed = 1;
}
