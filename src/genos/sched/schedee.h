#ifndef GENOS_SCHED_H
#define GENOS_SCHED_H

#include <gxx/dlist.h>
#include <genos/atomic.h>

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
		uint8_t state;

		virtual void finalize() = 0;
		virtual void execute() = 0;
		virtual void displace() = 0;
	};

	class schedee_manager_class {
		using schedee_list = gxx::dlist<genos::schedee, &genos::schedee::lnk>;

		schedee_list runlist[PRIORITY_TOTAL];
		schedee_list waitlist;
		schedee_list finallist;
		schedee_list zombielist;

		schedee* current_schedee = nullptr;

	public:
		void final(schedee& sch) {
			assert(sch.state != SCHEDEE_STATE_FINAL);
			sch.state = SCHEDEE_STATE_FINAL;
			finallist.move_back(sch);
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
		
			atomic_section_enter();

			schedee* sch;
			for (int priolvl = 0; priolvl < PRIORITY_TOTAL; priolvl++) {
				if (!runlist[priolvl].empty()) {
					
					sch = &*runlist[priolvl].begin();
					runlist[priolvl].move_back(*sch);
					current_schedee = sch;

					atomic_section_leave();
					sch->execute();
					return;
				
				}
			}

			atomic_section_leave();

			//Nobody to run
			return;
		}
	};

	extern genos::schedee_manager_class schedee_manager;
}

#endif 