#include <kernel/csection.h>
#include <kernel/irq.h>

static irqstate_t __atomic_save;
static uint8_t __atomic_counter;

void atomic_section_enter() {
	if (__atomic_counter == 0) __atomic_save = global_irq_save();
	__atomic_counter++;
}

void atomic_section_leave() {
	__atomic_counter--;
	if (__atomic_counter == 0) global_irq_restore(__atomic_save);
}