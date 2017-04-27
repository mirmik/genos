#include <compiler.h>
#include <asm/irq.h>

#include <mutex>

#include <debug/dprint.h>

static irqstate_t current_state = 0;
static std::mutex lock;

void global_irq_enable(void) {
	//dprln("global_irq_enable");
	current_state = 0;
	lock.unlock();
}

irqstate_t global_irq_save(void) {
	//dprln("global_irq_save");
	auto ret = current_state;
	global_irq_disable();
	return ret;
}

void global_irq_restore(irqstate_t state) {
	//dprln("global_irq_restore");
	if (state == 1) global_irq_disable();
	else global_irq_enable();
}

void global_irq_disable(void) {
	//dprln("global_irq_disable");
	lock.lock();
	current_state = 1;
}