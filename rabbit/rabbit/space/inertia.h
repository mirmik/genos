#ifndef RABBIT_SPACE_INERTIA_H
#define RABBIT_SPACE_INERTIA_H

#include <rabbit/third/linalg.h>

namespace rabbit 
{
	// Инерция тела обязательно задаётся в главных осях.
	template<typename T>
	struct hinertia3 
	{
		linalg::vec<T,3> moment;
		T mass;

		inertia3(T moment, T mass) 
			: moment(linalg::vec<T,3>(linalg::identity) * moment), 
			mass(mass) {}	
	};
}

#endif