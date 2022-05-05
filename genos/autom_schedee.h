#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <genos/schedee.h>
#include <igris/compiler.h>

namespace genos
{
	typedef void (* autom_schedee_func_t)(void* priv, int* state);

	class autom_schedee : public schedee
	{
		autom_schedee_func_t func;
		void *               priv;
		int state;

	public:
		autom_schedee() = default;
		autom_schedee(autom_schedee_func_t foo, void * privdata);
		void init(autom_schedee_func_t foo, void * privdata);

		void execute() override;
		void finalize() override;
		int displace() override;
	};
}

#endif