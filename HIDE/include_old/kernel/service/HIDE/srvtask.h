#ifndef GENOS_SERVICE_TASKLET_H
#define GENOS_SERVICE_TASKLET_H

#include <kernel/sched/schedee.h>
#include <kernel/service/service.h>

namespace genos {

	template<typename Parent>
	class service_tasklet : public schedee {
	public:
		Parent* parent;

	public:
		service_tasklet(Parent* parent, uint8_t prio) : parent(parent), 
			schedee(prio) {}
	
		virtual void routine() = 0;

		uint8_t execute() override {
			
			routine();
			return SCHEDULE_REPEAT;
		}

		uint8_t engage() override {
			routine();
		}

		uint8_t displace() override {}
		uint8_t lastexit() override {}
		uint8_t destructor() override {}

		Parent* get_service() { return parent; }
	};

}

#endif