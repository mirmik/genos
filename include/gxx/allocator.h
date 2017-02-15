#ifndef GXX_ALLOCATOR_H
#define GXX_ALLOCATOR_H

namespace gxx {
	template<typename T>
	class allocator {
	public:
		T* allocate(size_t sz) {
			return (T*)malloc(sz * sizeof(T));
		};

		T* reallocate(void* ptr, size_t sz) {
			return (T*)realloc(ptr, sz * sizeof(T));
		};

		void deallocate(void* ptr) {
			free(ptr);
		};
	};
}

#endif