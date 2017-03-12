#ifndef KERNEL_SUBST_SCHEDEE_H
#define KERNEL_SUBST_SCHEDEE_H

#include <kernel/sched/schedee.h>
#include <genos/subst/subst.h>
#include <genos/sigslot/delegate.h>

class subst_schedee : public schedee {
public:
	context cntxt;
	
	union {
		struct { 
			gstack** 	ppstack;
			qid_t* 		retqid;
		};
		struct { 
			void* startarg;
		};
	};
	delegate<void, void*> startpoint;
	void* stackptr;

	subst_schedee(delegate<void, void*> dlg, void* arg);
};

#endif