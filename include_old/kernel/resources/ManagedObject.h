#ifndef GENOS_MANAGED_OBJECT_H
#define GENOS_MANAGED_OBJECT_H

#include <gxx/utility.h>

namespace Genos {
	class ManagedObject {
	public:
		virtual void release() = 0;
	};

	namespace Glue {
		template<typename T> T* getKernelObject();
		template<typename T> void putKernelObject(T*);
	}

	template <typename T>
	class KernelObject {
	public:
		template<typename ... Args>
		static T* engage(Args ... args) {
			T* ptr = Genos::Glue::getKernelObject<T>();
			gxx::constructor(ptr, gxx::forward(args) ...);
			return ptr;
		}

		static void release(T* ptr) {
			gxx::destructor(ptr);
			Genos::Glue::putKernelObject<T>(ptr);
		}

		void release() {
			T::release((T*)this);
		}
	};
}

#endif