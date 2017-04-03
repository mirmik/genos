#ifndef GENOS_SCHEDEE_MANAGER_H
#define GENOS_SCHEDEE_MANAGER_H

#include <kernel/sched/AbstractSchedeeManager.h>
#include <kernel/sched/Schedee.h>
#include <kernel/csection.h>

namespace Genos {
	class SchedeeManager : public AbstractSchedeeManager {
	private:
		SchedeeList runlist[PRIORITY_TOTAL];
		SchedeeList waitlist;
		SchedeeList finallist;

	public:
		void runSchedee(Schedee& sch) override {
			assert(sch.state != SCHEDEE_STATE_WAIT);
			sch.state = SCHEDEE_STATE_RUN;
			runlist[sch.prio].move_back(sch);
		}

		void stopSchedee(Schedee& sch) override {
			assert(sch.state != SCHEDEE_STATE_WAIT);
			sch.state = SCHEDEE_STATE_STOP;
			dlist_del_init(&sch.schlnk);
		}

		void waitSchedee(Schedee& sch, uint8_t type) override {
			panic("TODO");
		}

		void unwaitSchedee(Schedee& sch, uint8_t type) override {
			panic("TODO");
		}

		void execute() override {
			while (!finallist.empty()) {
				Schedee* fsch = &*finallist.begin();
				fsch->finalize();
				delete fsch;
			}
		
			critical_section_enter();

			Schedee* sch;
			for (int priolvl = 0; priolvl < PRIORITY_TOTAL; priolvl++) {
				if (!runlist[priolvl].empty()) {
					
					sch = &*runlist[priolvl].begin();
					runlist[priolvl].move_back(*sch);
					currentSchedee(sch);

					critical_section_leave();
					sch->execute();
					return;
				
				}
			}

			critical_section_leave();

			//Nobody to run
			return;
		}
	};
}

#endif