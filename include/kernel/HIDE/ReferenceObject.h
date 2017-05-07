#ifndef GENOS_REFERENCE_OBJECT_H
#define GENOS_REFERENCE_OBJECT_H

#include <inttypes.h>

namespace Genos {

	class ReferenceObject {
		uint8_t refcount = 0;
	
	protected:
		void counter_open() {
			++refcount;
		}
	
		void counter_close() {
			--refcount;
			if (refcount == 0) destroy();
		}
	
	protected:
		virtual void destroy() = 0;
	};

}

#endif