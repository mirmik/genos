#ifndef GXX_AVR_ATOMIC_H
#define GXX_AVR_ATOMIC_H

#include <hal/irq.h>
#include <inttypes.h>

namespace gxx {
	class atomic {

	static uint8_t __atomic_counter;
	static irqstate_t __atomic_save;

	public:
		void lock();
		void unlock();
	};
}

#endif