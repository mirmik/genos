#ifndef GENOS_LIST_H
#define GENOS_LIST_H

#include <genos/datastruct/dlist_head.h>
#include <gxx/allocator.h>
#include <utilxx/new.h>
#include "gxx/to_string.h"

namespace gxx {

template <typename valtype>
struct listrecord {		
	dlist_head lnk;
	valtype value;

	template<typename ... Args>
	listrecord(Args&& ... args) : value(gxx::forward<Args...>(args ...)) {
		dlist_init_list(&lnk);
	}
};

template<typename T, typename Allocator = system_allocator<listrecord<T>>>
class list {
public:
	using traits_type = elem_traits<T>;
	
	using pointer = 		typename traits_type::ptr_type;
	using const_pointer = 	typename traits_type::const_ptr_type;
	using reference = 		typename traits_type::reference;
	using const_reference = typename traits_type::const_reference;
	using size_type = 		typename traits_type::size_type;
	using difference_type = typename traits_type::ptrdiff_t;
	using value_type = 		typename traits_type::value_type;

	using record_t = listrecord<T>;

private:
	dlist_head lst;
	Allocator alloc;

public:
	list() {
		dlist_init_list(&lst);
	}

	~list() {
		dlist_del(&lst);
	}

	template<typename ... Args>
	void emplace_back(Args&& ... args) {
		record_t* ptr = alloc.allocate(1);
		new ((void*)ptr) record_t(gxx::forward<Args...>(args...));
		dlist_add_back(&ptr->lnk, &lst);
	}

	template<typename ... Args>
	void emplace_front(Args&& ... args) {
		record_t* ptr = alloc.allocate(1);
		new ((void*)ptr) record_t(gxx::forward<Args...>(args...));
		dlist_add_back(&ptr->lnk, &lst);
	}

	void pop_back() {
		assert(!empty());
		record_t* rec = container_of(lst.prev, record_t, lnk);
		destruct(rec);
	}

	void pop_front() {
		assert(!empty());
		record_t* rec = container_of(lst.next, record_t, lnk);
		destruct(rec);
	}

	void push_front( const T& value ) {
		record_t* ptr = alloc.allocate(1);
		new ((void*)ptr) record_t(value);
		dlist_add_front(&ptr->lnk, &lst);		
	};
		
	void push_front( T&& value ) {
		record_t* ptr = alloc.allocate(1);
		new ((void*)ptr) record_t(value);
		dlist_add_front(&ptr->lnk, &lst);		
	};

	void push_back( const T& value ) {
		record_t* ptr = alloc.allocate(1);
		new ((void*)ptr) record_t(value);
		dlist_add_back(&ptr->lnk, &lst);		
	};
		
	void push_back( T&& value ) {
		record_t* ptr = alloc.allocate(1);
		new ((void*)ptr) record_t(value);
		dlist_add_back(&ptr->lnk, &lst);		
	};

	T& back() {
		return container_of(lst.prev, record_t, lnk)->value;
	}

	T& front() {
		return container_of(lst.next, record_t, lnk)->value; 
	}

	bool empty() {
		return lst.next == &lst;
	}

	void destruct(record_t* rec) {
		dlist_del(&rec->lnk);
		rec->value.~T();
		alloc.deallocate(rec);
	}

	class iterator{
		public:
		dlist_head* current;
		public:
		iterator(dlist_head* head) : current(head) {};		
				
		iterator operator++(int) { iterator i = *this; current=current->next; return i; }
		iterator operator++() { current=current->next; return *this; }
		iterator operator--(int) { iterator i = *this; current=current->prev; return i; }
		iterator operator--() { current=current->prev; return *this; }
		bool operator!= (const iterator& b) {return current != b.current;};
		bool operator== (const iterator& b) {return current == b.current;};
				
		value_type & operator*() {return container_of(current, record_t, lnk)->value;};
		value_type * operator->() {return &container_of(current, record_t, lnk)->value;};
	};

	class reverse_iterator{
		public:
		dlist_head* current;
		public:
		reverse_iterator(dlist_head* head) : current(head) {};		
				
		reverse_iterator operator++(int) { reverse_iterator i = *this; current=current->prev; return i; }
		reverse_iterator operator++() { current=current->prev; return *this; }
		reverse_iterator operator--(int) { reverse_iterator i = *this; current=current->next; return i; }
		reverse_iterator operator--() { current=current->next; return *this; }
		bool operator!= (const reverse_iterator& b) {return current != b.current;};
		bool operator== (const reverse_iterator& b) {return current == b.current;};
					
		value_type & operator*() {return container_of(current, record_t, lnk)->value;};
		value_type * operator->() {return &container_of(current, record_t, lnk)->value;};
	};

	iterator begin() 	{ return iterator(lst.next); };
	iterator end() 		{ return iterator(&lst); };
	iterator rbegin() 	{ return reverse_iterator(lst.prev); };
	iterator rend() 	{ return reverse_iterator(&lst); };

	gxx::string to_str() const {
		gxx::string str;
		str.reserve(128);
		str << "[";
		for (auto r : *const_cast<list*>(this)) {
			str << ::to_str(r) << gxx::string(",");
		};
		str << "]";
		return str;
	};
};

};

#endif