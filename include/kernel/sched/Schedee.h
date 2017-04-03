#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <stdint.h>
#include <datastruct/dlist_head.h>
#include <kernel/sched/AbstractSchedeeManager.h>

#ifndef PRIORITY_TOTAL
#	define PRIORITY_TOTAL 4
#endif

//#define SCHEDULE_RESUME 1
//#define SCHEDULE_REPEAT 0

static const uint8_t SCHEDEE_STATE_INIT = 0x00;
static const uint8_t SCHEDEE_STATE_RUN = 0x01;
static const uint8_t SCHEDEE_STATE_STOP = 0x07;

static const uint8_t SCHEDEE_STATE_WAIT = 0x02;
static const uint8_t SCHEDEE_BLOCKED_SEND = 0x03;
static const uint8_t SCHEDEE_BLOCKED_RECEIVE = 0x04;
static const uint8_t SCHEDEE_STATE_FINAL = 0x05;
static const uint8_t SCHEDEE_STATE_ZOMBIE = 0x06;

static const uint8_t SCHEDEE_STATE_MASK = 0x0F;

//static const uint8_t SCHEDEE_FINAL_DEALLOCATE = 0x80;
//static const uint8_t SCHEDEE_FINAL_RELEASE = 0x40;

namespace Genos {

	class Schedee {
	public:
		struct dlist_head schlnk;
		uint8_t prio;
		struct {
			uint8_t state : 4;
		};

	public:
		Schedee() : prio(PRIORITY_TOTAL - 1), state(SCHEDEE_STATE_INIT) {};
		Schedee(uint8_t prio) : prio(prio), state(SCHEDEE_STATE_INIT) {};

		void run() {
			Glue::systemSchedeeManager().runSchedee(*this);
		}

		void stop() {
			Glue::systemSchedeeManager().stopSchedee(*this);
		}

		void setPrio(uint8_t prio) {
			assert(prio < PRIORITY_TOTAL);
			this->prio = prio;
		}

		virtual void execute() = 0;
		virtual void displace() = 0;
		virtual void finalize() = 0;

	};

	using SchedeeList = gxx::dlist<Schedee, &Schedee::schlnk>;

	Schedee* currentSchedee();
	void currentSchedee(Schedee* sch);

	void finalizeSchedee(Schedee* sch);
}

#endif