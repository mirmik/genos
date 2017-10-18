#ifndef GXX_TASKLET_MANAGER_H
#define GXX_TASKLET_MANAGER_H

#include <gxx/dlist.h>

namespace genos {
	class tasklet {
	public:
		dlist_head lnk;
		gxx::delegate<void> dlg;
		
		tasklet(gxx::delegate<void> dlg) : dlg(dlg) {}
	};

	class tasklet_manager {
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
}

#endif