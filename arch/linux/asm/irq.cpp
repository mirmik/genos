#include <compiler.h>
#include <asm/irq.h>

#include <gxx/debug/dprint.h>

#include <mutex>

static irqstate_t current_state = 0;
static std::mutex lock;

void global_irqs_enable(void) {
	current_state = 0;
	lock.unlock();
}

irqstate_t global_irqs_save(void) {
	auto ret = current_state;
	global_irqs_disable();
	return ret;
}

void global_irqs_restore(irqstate_t state) {
	if (state == 1) global_irqs_disable();
	else global_irqs_enable();
}

void global_irqs_disable(void) {
	lock.lock();
	current_state = 1;
}