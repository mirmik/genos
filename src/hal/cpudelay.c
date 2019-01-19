#include <hal/cpudelay.h>

void cpu_delay(uint64_t ticks) {
	volatile uint64_t count = ticks;
	while(count--);
}