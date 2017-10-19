#include <genos/atomic.h>
#include <arch/irqs.h>
#include <assert.h>

static uint8_t __atomic_save;
static uint8_t __atomic_counter;

void atomic_section_enter() {
	if (__atomic_counter == 0) __atomic_save = arch::irqs::save();
	__atomic_counter++;
}

void atomic_section_leave() {
	__atomic_counter--;
	assert(__atomic_counter >= 0);
	if (__atomic_counter == 0) arch::irqs::restore(__atomic_save);
}