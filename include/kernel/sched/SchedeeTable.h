#ifndef GENOS_SCHEDEE_TABLE_H
#define GENOS_SCHEDEE_TABLE_H

#include <kernel/sched/Schedee.h>
#include <gxx/hashtable.h>

#define SCHEDEE_HASHTABLE_SIZE 20
#define MAX_QID 1000

//static int& __service_getkey (Genos::Schedee& srvs) { return srvs.pid; }

namespace Genos {

	class SchedeeTable {

		pid_t pid_counter;
		gxx::static_hashtable<SCHEDEE_HASHTABLE_SIZE,
			Schedee, pid_t, &Schedee::hlnk> table;

	public:
		SchedeeTable() {}

	public:
		pid_t registry(Schedee& srvs) {
			srvs.pid = get_new_pid();
			table.put(srvs);
			return srvs.pid;
		}

		Schedee* get(pid_t pid) {
			return table.get(pid);
		}

		template<typename Function>
		void foreach(Function func) {
			table.foreach(func);
		}

		template<typename Function>
		Schedee* find(Function func) {
			return table.find(func);
		}

		Schedee* find() {

		}

	private: 
		pid_t get_new_pid() {
			while (table.get(++pid_counter))
				if (pid_counter > MAX_QID) pid_counter = 0; 
			return pid_counter;
		}
	};

	namespace Glue {
		Schedee* getSchedee(pid_t pid);
		SchedeeTable& systemSchedeeTable();
	}
}

#endif