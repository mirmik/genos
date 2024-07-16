#ifndef RABBIT_SPACE_FORCE_H
#define RABBIT_SPACE_FORCE_H

#include <rabbit/third/linalg.h>

namespace rabbit 
{
	template<class T>
	struct force3 
	{
		linalg::vec<T,3> angfrc;
		linalg::vec<T,3> linfrc;		
	};
}

#endif