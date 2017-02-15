#ifndef GXX_ALLOCATOR_H
#define GXX_ALLOCATOR_H

namespace gxx {
	template<typename T>
	class allocator {
	public:
		void* allocate(size_t sz) {
			return malloc(sz * sizeof(T));
		};

		void* reallocate(void* ptr, size_t sz) {
			return realloc(ptr, sz * sizeof(T));
		};

		void deallocate(void* ptr) {
			free(ptr);
		};
	};
}

#endif