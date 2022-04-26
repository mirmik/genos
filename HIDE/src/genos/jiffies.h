#ifndef IGRIS_JIFFIES_TIME_H
#define IGRIS_JIFFIES_TIME_H

#include <stdint.h>
#include <igris/compiler.h>

#if __has_include(<asm/systime.h>)
#include <asm/systime.h>
#else
#define systime_lopart() (0)
#define systime_lomax() (1)
#endif

typedef int64_t jiffies_t;

extern volatile jiffies_t __jiffies;

typedef struct jiffies_pair_s
{
	jiffies_t hi;
	jiffies_t lo;
} jiffies_pair_t;

__BEGIN_DECLS

static inline void system_tick() { ++ __jiffies; }
jiffies_t jiffies();

__END_DECLS

#endif