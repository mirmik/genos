#ifndef GENOS_MANAGED_OBJECT_H
#define GENOS_MANAGED_OBJECT_H

namespace Genos {
	class ManagedObject {
	public:
		virtual void release() = 0;
	};
}

#endif