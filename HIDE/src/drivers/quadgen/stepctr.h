#ifndef GENOS_DRIVERS_QUADGEN_STEPCTR_H
#define GENOS_DRIVERS_QUADGEN_STEPCTR_H

#include "quadgen.h"
#include "quadgen_differential.h"
#include <ralgo/heimer/stepctr.h>

namespace genos 
{
	class quadgen_differential_stepctr : 
		public heimer::stepctr<float, int64_t, float>
	{
	public:
		genos::quadgen_differential q;

	public:
		quadgen_differential_stepctr(const char * name) : 
			heimer::stepctr<float, int64_t, float>(name)
		{}

		void init(genos::quadgen_differential q) 
		{
			this->q = q;
		}

		void inc() override 
		{
			q.inc();
		}

		void dec() override 
		{
			q.dec();
		}
	};


	class quadgen_stepctr : 
		public heimer::stepctr<float, int64_t, float>
	{
	public:
		genos::quadgen q;

	public:
		quadgen_stepctr(const char * name) : 
			heimer::stepctr<float, int64_t, float>(name)
		{

		}

		void init(genos::quadgen q) 
		{
			this->q = q;
		}

		void inc() override 
		{
			q.inc();
		}

		void dec() override 
		{
			q.dec();
		}
	};
}

#endif