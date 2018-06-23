#ifndef GENOS_KERNEL_ATOMIC_SECTION_H
#define GENOS_KERNEL_ATOMIC_SECTION_H

namespace genos {
	void atomic_section_enter();
	void atomic_section_leave();
}

#endif