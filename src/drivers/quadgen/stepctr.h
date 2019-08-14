#ifndef GENOS_DRIVERS_QUADGEN_STEPCTR_H
#define GENOS_DRIVERS_QUADGEN_STEPCTR_H

#include "quadgen.h"
#include <ralgo/planning/stepctr.h>

namespace genos 
{
	class quadgen_stepctr : public ralgo::stepctr
	{
	public:
		genos::quadgen_differential q;

	public:
		quadgen_stepctr(const genos::quadgen_differential& q) : q(q) {}

		void inc() override 
		{
			q.inc();
		}

		void dec() override 
		{
			q.inc();
		}

		void set();
	};
}

#endif