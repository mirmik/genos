#ifndef GENOS_WAIT_QUEUE_H
#define GENOS_WAIT_QUEUE_H

#include <gxx/dlist.h>
#include <err/errno.h>
#include <sched/Scheduler.h>

namespace genos {
	class wait_head {
		public:
		genos::pid_t pid;
		dlist_head lnk;
	};

	class wait_queue {
	public:
		gxx::dlist<wait_head, &wait_head::lnk> list;

		void unwait_one() {
			dprln("onewait one");
		}

		void push(wait_head& head) {
			list.move_back(head);
		}
	};

	static int schedee_wait_for(wait_queue& wq) {
		if (!genos::current_schedee()->can_subst()) return EAGAIN;

		wait_head head { genos::current_schedee()->pid } ;
		
		genos::current_schedee()->wait();
		wq.push(head);
		
		genos::sched::displace(genos::sched::substtype);
	}
}

#endif