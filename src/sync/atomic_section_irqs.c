#include <kernel/csection.h>
#include <genos/irq.h>
#include <assert.h>

static irqstate_t __atomic_save;
static int8_t __atomic_counter;

void atomic_section_enter() {
	//dprln("atomic_section_enable");
	if (__atomic_counter == 0) __atomic_save = global_irqs_save();
	__atomic_counter++;
}

void atomic_section_leave() {
	__atomic_counter--;
	assert(__atomic_counter >= 0);
	//dprln("atomic_section_leave");
	if (__atomic_counter == 0) global_irqs_restore(__atomic_save);
}