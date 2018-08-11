#ifndef GENOS_SCHED_H
#define GENOS_SCHED_H

#include <gxx/container/dlist.h>
#include <gxx/debug/delay.h>
//#include <genos/atomic.h>
#include <gxx/syslock.h>
#include <assert.h>

#define PRIORITY_TOTAL 3

namespace genos {
	static constexpr uint8_t SCHEDEE_STATE_RUN = 0;
	static constexpr uint8_t SCHEDEE_STATE_WAIT = 2;
	static constexpr uint8_t SCHEDEE_STATE_STOP = 3;
	static constexpr uint8_t SCHEDEE_STATE_FINAL = 4;
	static constexpr uint8_t SCHEDEE_STATE_ZOMBIE = 5;

	class schedee {
	public:
		dlist_head lnk;
		uint8_t prio = 0;
		uint8_t state = SCHEDEE_STATE_STOP;
		bool detached;

		virtual void finalize() = 0;
		virtual void execute() = 0;
		virtual void displace() = 0;

		void unwait(schedee& sch, uint8_t type);
		void unwait_wstate();
	};

	class schedee_ptr {		
	public:
		schedee_ptr(const schedee_ptr&) = delete;
		schedee_ptr(schedee_ptr&&) = default;
		schedee_ptr(schedee* sch) : sch(sch) {}
		~schedee_ptr() { if (sch) sch->detached = true; }
		void detach() { sch->detached = true; sch = nullptr; }
		schedee* operator->() { return sch; }

	private:
		schedee* sch;
	};

	class schedee_manager_class {
	public:
		using schedee_list = gxx::dlist<genos::schedee, &genos::schedee::lnk>;

		schedee_list runlist[PRIORITY_TOTAL];
		schedee_list waitlist;
		schedee_list finallist;
		schedee_list zombielist;

	public:
		schedee* current_schedee = nullptr;

	public:
		schedee& current() {
			return* current_schedee;
		}

		size_t total_runned() {
			size_t total = 0;
			for (auto& r: runlist) {
				total += r.size();
			}
			return total;
		}

		void final(schedee& sch) 
		{
			if (sch.detached) 
			{
				sch.state = SCHEDEE_STATE_FINAL;
				finallist.move_back(sch);
			}
			else 
			{
				sch.state = SCHEDEE_STATE_ZOMBIE;
				zombielist.move_back(sch);
			}
		}

		void zombie(schedee& sch) {
			assert(sch.state == SCHEDEE_STATE_FINAL);
			sch.state = SCHEDEE_STATE_ZOMBIE;
			zombielist.move_back(sch);
		}

		void run(schedee& sch) {
			assert(sch.state != SCHEDEE_STATE_WAIT);
			sch.state = SCHEDEE_STATE_RUN;
			runlist[sch.prio].move_back(sch);
			dprln(runlist[sch.prio].size());
		}

		void stop(schedee& sch) {
			assert(sch.state != SCHEDEE_STATE_WAIT);
			sch.state = SCHEDEE_STATE_STOP;
			schedee_list::unbind(sch);
		}

		void wait(schedee& sch, uint8_t type) {
			assert(sch.state == SCHEDEE_STATE_RUN);
			sch.state = type;
			waitlist.move_back(sch);
		}

		void unwait(schedee& sch, uint8_t type) {
			//dprln("unwait");
			if (sch.state == SCHEDEE_STATE_FINAL ||
				sch.state == SCHEDEE_STATE_ZOMBIE) return;
			assert(sch.state == type);
			sch.state = SCHEDEE_STATE_RUN;
			runlist[sch.prio].move_back(sch);
		}


		void execute() {
			while (!finallist.empty()) {
				schedee& fsch = *finallist.begin();
				
				//данная функция инициализирует процедуру удаления
				//процесса из системы. Удаление может быть достаточно
				//длительным, но в любом случае это не проблемы планировщика.
				schedee_list::unbind(fsch);
				fsch.finalize();
			}
		
			//atomic_section_enter();
			gxx::system_lock();

			schedee* sch;
			for (int priolvl = 0; priolvl < PRIORITY_TOTAL; priolvl++) {
				if (!runlist[priolvl].empty()) {
					
					sch = &*runlist[priolvl].begin();
					runlist[priolvl].move_back(*sch);
					current_schedee = sch;

					gxx::system_unlock();
					sch->execute();
					return;
				}
			}

			gxx::system_unlock();

			//Nobody to run
			return;
		}
		void exec() { execute(); }
	};

	extern genos::schedee_manager_class schedee_manager;
}

__BEGIN_DECLS

static inline void genos_schedee_unwait(void* arg) {
	dprln("HERE");
	genos::schedee_manager.unwait(* (genos::schedee*) arg, genos::SCHEDEE_STATE_WAIT);
}

__END_DECLS

#endif 