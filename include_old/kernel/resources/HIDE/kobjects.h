#ifndef GENOS_KOBJECTS_H
#define GENOS_KOBJECTS_H

#include <kernel/pool/Pool.h>
#include <kernel/event/WakeUpWaiter.h>

namespace Genos {
	extern Pool wwake_pool;

	void pools_init();


}

#endif