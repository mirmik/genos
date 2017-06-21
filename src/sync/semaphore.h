#ifndef GENOS_SEMAPHORE_H
#define GENOS_SEMAPHORE_H

#include <sync/wait_queue.h>

namespace genos {
	class semaphore {
		uint8_t count;
		genos::wait_queue wq;

	public:
		semaphore(int count) : count(count) {}

		int down_interrutible() {
			schedee_wait_for(wq);
		}

		int up() {
			count++;
		}

	};
}

#endif