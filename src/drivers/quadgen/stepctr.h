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
		bool en = false;

	public:
		quadgen_stepctr(const genos::quadgen_differential& q) : q(q) {}

		void inc() override 
		{
			q.inc();
		}

		void dec() override 
		{
			q.dec();
		}

		void set();

		void enable_power(bool en) override 
		{
			this->en = en;
			//pass
		}

		/*int64_t target_position() override 
		{
			return target_impulse_position / 4;
		}*/
	};
}

#endif