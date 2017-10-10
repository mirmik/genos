#ifndef GENOS_HAL_IRQS_H
#define GENOS_HAL_IRQS_H

#include <arch/hal/irqs.h>

namespace hal {
	namespace irqs {
		void enable();
		void disable();
		uint8_t save();
		void restore(uint8_t s);
	}
}

#endif