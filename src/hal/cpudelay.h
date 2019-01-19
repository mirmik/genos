#ifndef HAL_CPUDELAY_H
#define HAL_CPUDELAY_H

#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

extern void cpu_delay(uint64_t count);

__END_DECLS

#endif