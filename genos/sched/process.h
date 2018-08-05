#ifndef GENOS_PROCESS_SCHEDEE_H
#define GENOS_PROCESS_SCHEDEE_H

#include <genos/sched/schedee.h>
#include <gxx/event/delegate.h>
#include <gxx/buffer.h>

namespace genos {
	genos::schedee_ptr create_process(void(*func)(void*), void*, gxx::buffer);
}

#endif