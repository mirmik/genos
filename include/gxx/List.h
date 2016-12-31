#ifndef GXX_LIST_H
#define GXX_LIST_H

#include <gxx/utility.h>
#include <gxx/algorithm.h>
#include <gxx/Allocator.h>

namespace gxx {

	template <typename T>
	class ListImpl : public BasicAllocated {
	public:
		T** m_data;
		size_t m_size;
		size_t m_capacity;
		uint8_t refs;

		void init() {
			m_data = nullptr;
			m_size = 0;
			m_capacity = 0;
		}

		void doCopyTo(ListImpl<T>& target) {
			target.m_data = target.m_alloc->allocate(sizeof(T*) * m_size);
			memcpy(target.m_data, m_data, sizeof(T*) * m_size);
			target.m_size = target.m_capacity = m_size; 
		} 

		ListImpl(Allocator* alloc) : BasicAllocated(alloc) {}
	};

	template <typename T>
	class List {
	public:
		ListImpl<T>* r;

		List(Allocator* alloc = gxx::defaultAllocator) {
			r = alloc->constructObject<ListImpl<T>>(alloc);
			r->init();
		}

		List(const List& other) {
			r = other.r;
			if (r == nullptr) return;
			++r->refs;
		}

		List(List&& other) {
			r = other.r;
			other.r = nullptr; 
		}

		~List() {
			if (r == nullptr) return;
			--r->refs;
			if (r->refs == 0) {
				dprln("OVERALDESTRUCTOR");
				r->m_alloc->destructObject<ListImpl<T>>(r);
			}
		}

		int doCopy() {
			ListImpl<T>* newr = r->m_alloc->constructObject<ListImpl<T>>(r->m_alloc);
			r->doCopyTo(*newr);
			newr->refs = 1;
			r = newr;
		}

		int reserve(size_t sz) {
			if (!r) return changeBuffer(sz);
			if (sz <= r->m_capacity) return 0;

			if (r->refs != 1) doCopy();

			changeBuffer(sz);
		}

		int changeBuffer(size_t sz) {
			r->m_data = r->m_alloc->reallocate(r->m_data, sizeof(T*) * r->m_size);
			return sz;
		}

		void push_back(const T& obj) {
			T* robj = r->m_alloc->constructorObject(obj);
			reserve(r->m_size + 1);
			r->m_data[r->m_size] = robj;
			r->m_size++;
		}
	};
}

#endif