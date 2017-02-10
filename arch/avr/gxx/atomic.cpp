#include <halgxx/irqs_lock.h>

uint8_t gxx::irqs_lock::__irqs_lock_counter = 0;
irqstate_t gxx::irqs_lock::__irqs_lock_save;

void gxx::irqs_lock::lock() {
	if (__irqs_lock_counter == 0) __irqs_lock_save = global_irq_save();
	__irqs_lock_counter++;
}

void gxx::irqs_lock::unlock() {
	__irqs_lock_counter--;
	if (__irqs_lock_counter == 0) global_irq_restore(__irqs_lock_save);
}