#include <halgxx/atomic.h>

uint8_t gxx::atomic::__atomic_counter = 0;
irqstate_t gxx::atomic::__atomic_save;

void gxx::atomic::lock() {
	if (__atomic_counter == 0) __atomic_save = global_irq_save();
	__atomic_counter++;
}

void gxx::atomic::unlock() {
	__atomic_counter--;
	if (__atomic_counter == 0) global_irq_restore(__atomic_save);
}