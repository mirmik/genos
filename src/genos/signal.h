#ifndef GENOS_SIGNAL_H
#define GENOS_SIGNAL_H

#include <genos/sched.h>

namespace genos 
{
	static inline
	void raise_signal_group(int gid, int sig) 
	{
		genos::schedee * sch;
		dlist_for_each_entry(sch, &schedee_list, schedee_list_lnk) 
		{
			if (sch->gid == gid) 
			{
				sch->signal_handler(sig);
			}
		}
	}
}

#endif