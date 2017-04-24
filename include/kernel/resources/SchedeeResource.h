#ifndef GENOS_SCHEDEE_RESOURCE_H
#define GENOS_SCHEDEE_RESOURCE_H

#include <kernel/sched/Schedee.h>
#include <kernel/resources/ManagedObject.h>

namespace Genos {
	class SchedeeResource : public ManagedObject {
	protected:
		Schedee* parent;

	public:
		SchedeeResource(Schedee* sch) : parent(sch) {}
	};
}

#endif