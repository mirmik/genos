#ifndef GENOS_SCHEDEE_MANAGER_H
#define GENOS_SCHEDEE_MANAGER_H

#include <kernel/sched/AbstractSchedeeManager.h>
#include <kernel/sched/Schedee.h>
#include <kernel/csection.h>
#include <kernel/panic.h>

namespace Genos {
	class SchedeeManager : public AbstractSchedeeManager {
	private:
		SchedeeList runlist[PRIORITY_TOTAL];
		SchedeeList waitlist;
		SchedeeList finallist;

	public:
		void finalSchedee(Schedee& sch) override {
			assert(sch.state != SCHEDEE_STATE_FINAL);
			sch.state = SCHEDEE_STATE_FINAL;
			finallist.move_back(sch);
		}

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
			assert(sch.state == SCHEDEE_STATE_RUN);
			sch.state = type;
			waitlist.move_back(sch);
		}

		void unwaitSchedee(Schedee& sch, uint8_t type) override {
			assert(sch.state == type);
			sch.state = SCHEDEE_STATE_RUN;
			runlist[sch.prio].move_back(sch);
		}

		void execute() override {
			while (!finallist.empty()) {
				Schedee* fsch = &*finallist.begin();
				//dprln("finalize");
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