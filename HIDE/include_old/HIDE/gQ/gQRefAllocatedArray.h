#ifndef GENOS_GQ_REF_BYTE_ALLOCATED_H
#define GENOS_GQ_REF_BYTE_ALLOCATED_H

#include <genos/defines/ssize_t.h>
#include <gQ/gQByteAllocated.h>
#include <gQ/util/gQMemoryStrategy.h>
#include <gxx/utility.h>
#include <gxx/algorithm.h>

#include <assert.h>

template <typename T>
class gQRefAllocatedArray : public gQBasicAllocated {
	class Record {
	public:
		T* data;
		size_t size;
		size_t capacity;
		uint16_t refs;
	};

	Record* record;

public:
	T* getBuffer() {
		return record->data; 
	}

public:
	gQRefAllocatedArray()  {
		record = m_alloc->constructObject<Record>();
		init(record);
		++record->refs;
	}

	gQRefAllocatedArray(const gQRefAllocatedArray& other) 
	: gQBasicAllocated(other) {
		record = other.record;
		++other.record->refs;
	}

	gQRefAllocatedArray(gQRefAllocatedArray&& other) 
	: gQBasicAllocated(gxx::forward<gQRefAllocatedArray>(other)) {
		record = other.record;
	}

	~gQRefAllocatedArray() {
		record->refs--;
		if (record->refs == 0 && record->data) {
			m_alloc->destructArray<T>(record->data, record->size);
			m_alloc->destructObject(record);
		}
	}

	void do_copy() {
		Record* newrecord = m_alloc->constructObject<Record>();
		init(newrecord, record);
		newrecord->data = allocateCopyBuffer(record->size);
		--record->refs;
		++newrecord->refs;
		record = newrecord;
	}	

	void shrink() {
		changeBuffer(record->size);
	}

	void init(Record* rec) {
		rec->data = nullptr;
		rec->size = rec->capacity = rec->refs = 0;
	}

	void init(Record* rec, Record* src) {
		rec->data = nullptr;
		rec->size = src->size;
		rec->capacity = src->capacity;
		rec->refs = 0;
	}

	T* data() {
		return record->data;
	}

	size_t size() {
		return record->size;
	}

	size_t capacity() {
		return record->capacity;
	}

	uint8_t invalidate() {
		if (record->data) { 
			m_alloc->destructArray<T>(record->data, record->size);
		};
		record->size = record->capacity = 0;
	};

	uint8_t resize(size_t newsz) {
		reserve(newsz);
		if (newsz > record->size)
			gxx::constructor(record->data + record->size, record->data + newsz);
		if (newsz < record->size)
			gxx::destructor(record->data + newsz, record->data + record->size);
		record->size = newsz;
		return 1; 
	};

	uint8_t reserve(size_t sz) {
		if (record->refs > 1) do_copy();
		if (sz <= record->capacity) return 1;
		return changeBuffer(sz); 
	};

	uint8_t changeBuffer(size_t sz) {
		assert( sz >= record->size );
		if (!record->data) {
			record->data = allocateEmptyBuffer(sz);
			record->capacity = sz;
			return 1;
		}

		auto olddata = record->data;
		record->data = allocateMoveBuffer(sz);
		record->capacity = sz;
		m_alloc->deallocateArray<T>(olddata);
		return 1;
	}

	T* allocateEmptyBuffer(size_t sz) {
		T* newrray = m_alloc->allocateArray<T>(sz);
		return newrray;
	};

	T* allocateMoveBuffer(size_t newsize) {
		T* newrray = m_alloc->allocateArray<T>(newsize);
		T* src = begin();
		T* dst = newrray;
		size_t movelen = gxx::min(newsize, record->size);
		ssize_t emptylen = newsize - movelen;
		for(int i = 0; i < movelen; i++)
			gxx::moveConstructor(dst++, gxx::move(*src++));
		return newrray;
	};

	T* allocateCopyBuffer(size_t newsize) {
		T* newrray = m_alloc->allocateArray<T>(newsize);
		T* src = begin();
		T* dst = newrray;
		size_t movelen = gxx::min(newsize, record->size);
		ssize_t emptylen = newsize - movelen;
		for(int i = 0; i < movelen; i++)
			gxx::copyConstructor(dst++, *src++);
		return newrray;
	};

	T& at(size_t n) {
		if (n >= record->size) resize(n + 1);
		return *(record->data + n);
	}

	T* begin() {
		return record->data;
	};

	T* end() {
		return record->data + record->size;
	};
};

#endif