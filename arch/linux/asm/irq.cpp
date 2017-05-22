#include <compiler.h>
#include <asm/irq.h>

#include <mutex>

#include <debug/dprint.h>

static irqstate_t current_state = 0;
static std::mutex lock;

void global_irqs_enable(void) {
	//dprln("global_irqs_enable");
	current_state = 0;
	lock.unlock();
}

irqstate_t global_irqs_save(void) {
	//dprln("global_irqs_save");
	auto ret = current_state;
	global_irqs_disable();
	return ret;
}

void global_irqs_restore(irqstate_t state) {
	//dprln("global_irqs_restore");
	if (state == 1) global_irqs_disable();
	else global_irqs_enable();
}

void global_irqs_disable(void) {
	//dprln("global_irqs_disable");
	lock.lock();
	current_state = 1;
}