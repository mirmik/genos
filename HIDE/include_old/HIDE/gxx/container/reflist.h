#ifndef GENOS_REFERENCE_LIST
#define GENOS_REFERENCE_LIST

#include <gxx/traits.h>
#include <genos/datastruct/dlist_head.h>
#include <gxx/allocator.h>

namespace gxx {

template <typename T>
struct reflistRecord : public dlist_head {
	T* ptr;
};

template <typename T, typename Allocator = gxx::system_allocator<reflistRecord<T>>>
class reflist {
public:
	using traits_type = gxx::elem_traits<T>;
	
	using pointer = 		typename traits_type::ptr_type;
	using const_pointer = 	typename traits_type::const_ptr_type;
	using reference = 		typename traits_type::reference;
	using const_reference = typename traits_type::const_reference;
	using size_type = 		typename traits_type::size_type;
	using difference_type = typename traits_type::ptrdiff_t;
	using value_type = 		typename traits_type::value_type;

	using record_t = reflistRecord<T>;

private:
	dlist_head list;
	Allocator alloc;

public:
	reflist() {
		dlist_init_list(&list);
	}

	~reflist() {
		dlist_del(&list);
	}	

	void link_front(pointer ptr) {
		record_t* rec = alloc.allocate(1);
		rec->ptr = ptr;
		dlist_add_front(rec, &list);
	};

	void link_back(pointer ptr) {
		record_t* rec = alloc.allocate(1);
		rec->ptr = ptr;
		dlist_add_back(rec, &list);
	};

	pointer pop_back() {
		record_t* rec = list.prev;
		pointer ptr = rec->ptr;
		dlist_del(rec);
		alloc.deallocate(rec);
		return ptr;
	};

	pointer pop_front() {
		record_t* rec = list.next;
		pointer ptr = rec->ptr;
		dlist_del(rec);
		alloc.deallocate(rec);
		return ptr;		
	};

	class iterator {
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
				
		value_type & operator*() {return *reinterpret_cast<record_t*>(current)->ptr;};
		value_type * operator->() {return reinterpret_cast<record_t*>(current)->ptr;};
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
					
		reference 	operator*() {return *reinterpret_cast<record_t*>(current)->ptr;};
		pointer 	operator->() {return reinterpret_cast<record_t*>(current)->ptr;};
	};

	iterator begin() 	{ return iterator(list.next); };
	iterator end() 		{ return iterator(&list); };
	iterator rbegin() 	{ return reverse_iterator(list.prev); };
	iterator rend() 	{ return reverse_iterator(&list); };
};

};

#endif