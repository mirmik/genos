#ifndef GXX_AVR_irqs_lock_H
#define GXX_AVR_irqs_lock_H

#include <genos/hal/irq.h>
#include <inttypes.h>

namespace gxx {
	class irqs_lock {

	static uint8_t __irqs_lock_counter;
	static irqstate_t __irqs_lock_save;

	public:
		void lock();
		void unlock();
	};
}

#endif