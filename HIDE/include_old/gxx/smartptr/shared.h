#ifndef GXX_SHARED_PTR_H
#define GXX_SHARED_PTR_H

#include <gxx/Allocator.h>
#include <genos_configure.h>

namespace gxx {

	template<typename T> class shared_ptr;

	template<typename T>
	class shared_ptr_node {	
		T* 			m_ptr;
		uint16_t	m_refs;
		gxx::Allocator* 	m_alloc;

	public:
		shared_ptr_node(gxx::Allocator* alloc) : m_alloc(alloc) {};

		friend class shared_ptr<T>;
	};

	template<typename T>
	class shared_ptr {
	private:
		shared_ptr_node<T>* node;
		
	public:

		//shared_ptr(gxx::Allocator* alloc = GXX_DEFAULT_ALLOCATOR) {
		//	node = alloc->constructObject<shared_ptr_node<T>, gxx::Allocator*>(alloc);
		//	node->m_refs = 1;
		//}

		shared_ptr(T* ptr, gxx::Allocator* alloc = GXX_DEFAULT_ALLOCATOR) {
			node = alloc->constructObject<shared_ptr_node<T>, gxx::Allocator*>(alloc);
			node->m_ptr = ptr;
			node->m_refs = 1;
		}

		shared_ptr(const shared_ptr & other) {
			node = other.node;
			node->m_refs++;
		}

		~shared_ptr() {
			node->m_refs--;
			if (node->m_refs == 0) node->m_alloc->destructObject(node->m_ptr); 
		}

		T& operator*() {
			return *node->m_ptr;
		}

		T* operator->() {
			return node->m_ptr;
		}
	};
}

#endif