#include <kernel/csection.h>
#include <kernel/irq.h>

//ipl_t enter_critical_section() {
//	if (!is_interrupt_context()) {
//		return ipl_save();
//	}
//}

//void leave_critical_section(ipl_t save) {
//	if (!is_interrupt_context()) {
//		ipl_restore(save);
//		return;
//	}
//}

static ipl_t __atomic_save;
static uint8_t __atomic_counter;

void atomic_section_enter() {
	if (__atomic_counter == 0) __atomic_save = ipl_save();
	__atomic_counter++;
}

void atomic_section_leave(ipl_t save) {
	__atomic_counter--;
	if (__atomic_counter == 0) ipl_restore(__atomic_save);
}