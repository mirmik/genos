#ifndef IGRIS_CPU_DELAY_H
#define IGRIS_CPU_DELAY_H

static inline void cpu_delay(unsigned long long ticks)
{
    volatile unsigned long long t = ticks;
    while (t) t=t-1;
}

#endif
