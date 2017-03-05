#ifndef KERNEL_SUBST_SCHEDEE_H
#define KERNEL_SUBST_SCHEDEE_H

#include <kernel/sched/schedee.h>
#include <genos/subst/subst.h>

class subst_schedee : public schedee {
public:
	context cntxt;
	
	gstack** 	ppstack;
	qid_t* 		retqid;

	subst_schedee(void(*func)(void*), void* arg);
	~subst_schedee() override;

private:
	virtual uint8_t execute() override;

protected:
	//virtual void invalidate() override {}
};

#endif