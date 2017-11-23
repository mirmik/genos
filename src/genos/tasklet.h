#ifndef GXX_TASKLET_MANAGER_H
#define GXX_TASKLET_MANAGER_H

#include <gxx/dlist.h>
#include <gxx/event/delegate.h>

namespace genos {
	class tasklet {
	public:
		dlist_head lnk;
		gxx::action dlg;

		tasklet(gxx::action dlg) : dlg(dlg) {}
		
		void plan();
		gxx::action make_plan_delegate() { return gxx::action(&tasklet::plan, this); }
	};

	class tasklet_manager_class {
		gxx::dlist<tasklet, &tasklet::lnk> planed_list;

	public:
		void plan(tasklet& tsklt);
		void execute();
	};

	extern genos::tasklet_manager_class tasklet_manager;
}


#endif