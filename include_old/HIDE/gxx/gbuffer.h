#ifndef GENOS_GBUFFER_H
#define GENOS_GBUFFER_H

namespace gxx {

class gbuffer {
public:
	void* _data;
	size_t _size;

public:
	buffer(void* __data, size_t __size)
		: _data(__data), _size(__size) {};
	
	void* data() const {
		return _data;
	}

	size_t size()  const {
		return _size;
	}

	void invalidate() {
		_data = nullptr;
		_size = 0;
	}

	template <typename Allocator>
	void allocate(size_t sz) {
		Allocator alloc;
		_data = alloc.allocate(sz);
	}
	
	template <typename Allocator>
	void deallocate() {
		Allocator alloc;
		_data = alloc.deallocate(_data);
	}
	
	template <typename Allocator>
	void reallocate(size_t sz) {
		Allocator alloc;
		_data = alloc.reallocate(_data, sz);
	}

};

}

#endif