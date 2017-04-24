#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <stdint.h>
#include <datastruct/dlist_head.h>
#include <datastruct/hlist_head.h>
#include <kernel/sched/AbstractSchedeeManager.h>

#include <kernel/id/id.h>
#include <kernel/resources/ManagedObject.h>
#include <gxx/vector.h>
//#include <gxx/ByteArray.h>

#include <utilxx/setget.h>

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

	class Schedee;

	namespace Glue {
		pid_t registerSchedee(Schedee*);
	}

	class Schedee : public ManagedObject {
	public:
		struct dlist_head schlnk;	//линк к спискам планировщика
		struct hlist_node hlnk;		//линк в хештаблицу процессов

		uint8_t prio;
		Genos::pid_t pid; 
		
		struct {
			uint8_t state : 4;
		};

		gxx::vector<ManagedObject*> resources;

		
	public:
		Schedee() : prio(PRIORITY_TOTAL - 1), state(SCHEDEE_STATE_INIT) {
			dlist_init(&schlnk);
			//dprln("Schedee()");
			//pid = Genos::Glue::registry(this);
		};

		Schedee(uint8_t prio) : prio(prio), state(SCHEDEE_STATE_INIT) {
			dlist_init(&schlnk);
			//dprln("Schedee(uint8_t prio)");
			//pid = Genos::Glue::registry(this);
		};

		void run() {
			Glue::systemSchedeeManager().runSchedee(*this);
		}

		void stop() {
			Glue::systemSchedeeManager().stopSchedee(*this);
		}

		void wait() {
			Glue::systemSchedeeManager().waitSchedee(*this, SCHEDEE_STATE_WAIT);
		}

		void wait(uint8_t state) {
			Glue::systemSchedeeManager().waitSchedee(*this, state);
		}

		void unwait() {
			Glue::systemSchedeeManager().unwaitSchedee(*this, SCHEDEE_STATE_WAIT);
		}

		void unwait(uint8_t state) {
			Glue::systemSchedeeManager().unwaitSchedee(*this, state);
		}

		void setPrio(uint8_t prio) {
			assert(prio < PRIORITY_TOTAL);
			this->prio = prio;
		}

		void finalizeResources() {
			auto beg = resources.begin();
			auto end = resources.end();
			for(auto it = beg; it != end; it++) {
				if (*it != nullptr)	(*it)->release();
			}
		}

		virtual void execute() = 0;
		virtual void displace() = 0;
		virtual void finalize() = 0;
	
		const char* m_name = nullptr;
		void assignName(const char* name) {
			m_name = name;
		}

		//ManagedObject support
		void release() {
			Glue::systemSchedeeManager().finalSchedee(*this);
		}

		static pid_t& getkey(Schedee& sch) { return sch.pid; }
	};

	using SchedeeList = gxx::dlist<Schedee, &Schedee::schlnk>;

	Schedee* currentSchedee();
	void currentSchedee(Schedee* sch);

	void finalizeSchedee(Schedee* sch);
}

#endif