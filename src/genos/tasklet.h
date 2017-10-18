#ifndef GXX_TASKLET_MANAGER_H
#define GXX_TASKLET_MANAGER_H

#include <gxx/dlist.h>

namespace genos {
	class tasklet {
	public:
		dlist_head lnk;
		gxx::delegate<void> dlg;
		
		void plan();
		tasklet(gxx::delegate<void> dlg) : dlg(dlg) {}
	};

	class tasklet_manager_singleton {
		gxx::dlist<tasklet, &tasklet::lnk> planed_list;

	public:
		void plan(tasklet& tsklt) {
			planed_list.move_back(tsklt);
		}

		void execute() {
			while(!planed_list.empty()) {
				tasklet& tsklt = *planed_list.begin();
				planed_list.pop(tsklt);
				tsklt.dlg();
			}
		}
	};

	extern genos::tasklet_manager_singleton tasklet_manager;
}

inline void genos::tasklet::plan() {
	genos::tasklet_manager.plan(*this);
}

#endif