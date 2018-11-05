#ifndef GENOS_EVENT_WAIT_HEAD_H
#define GENOS_EVENT_WAIT_HEAD_H

/*#include <gxx/dlist.h>
//#include <gxx/algorithm.h>

namespace genos {
	class wait_head;

	class waiter { 
		friend class wait_head;
		dlist_head lnk;

	protected:
		virtual void notify();
	};

	class wait_head {
		gxx::dlist<waiter, &waiter::lnk> head;

	public:
		void notify_one() {
			head.begin()->notify();
		}

		void add_waiter(waiter& node) {
			head.move_back(node);
		}
	};

	class w
}*/

#endif
