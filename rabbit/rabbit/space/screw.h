#ifndef RABBIT_SPACE_SCREW__H
#define RABBIT_SPACE_SCREW__H

#include <ralgo/space/screw.h>

namespace rabbit 
{
	template <class T, int N>
	using screw = ralgo::screw<T,N>;

	template <class T>
	using screw3 = ralgo::screw<T,3>;

	template <class T>
	using screw2 = ralgo::screw<T,2>;
}

#endif