#ifndef GENOS_SCHPOOL_H
#define GENOS_SCHPOOL_H

#define SCHPOOL_SIZE 6

extern pool_head schpool;

namespace genos 
{
	union 
	{
		autom_schedee autom;
		coop_schedee  coop;
	};

	schedee_union * get_schedee();
	void put_schedee(schedee_union * ptr);
}

#endif