#ifndef GENOS_DLIST_H
#define GENOS_DLIST_H

#include "genos/datastruct/dlist_head.h"
#include "utilxx/member.h" 
#include "gxx/string.h"

template<typename type, dlist_head type::* member>
class dlist 
{
public:
	dlist_head list;

	dlist()
	{
		dlist_init_list(&list);
	}

	~dlist()
	{
		dlist_del(&list);
	}

	bool is_empty()
	{
		return dlist_is_empty(&list);
	};

	void push_front(type& obj)
	{
		dlist_move_next(&(obj.*member), &list);
	};
	
	void push_back(type& obj)
	{
		dlist_move_prev(&(obj.*member), &list);
	};
	
	void push_next(type& obj, type& head)
	{
		assert(is_linked(head));
		dlist_move_next(&(obj.*member), &(head.*member));
	};
	
	void push_prev(type& obj, type& head)
	{
		assert(is_linked(head));
		dlist_move_prev(&(obj.*member), &(head.*member));
	};

	void pop(type& obj)
	{
		assert(is_linked(obj));
		dlist_del(&(obj.*member));
	};

	void pop_if_linked(type& obj)
	{
		if (!is_linked(obj)) return;
		dlist_del(&(obj.*member));
	};

	void pop_front()
	{
		dlist_del(&((*begin()).*member));
	};

	void pop_back()
	{
		dlist_del(&((*rbegin()).*member));
	};

	void round_left()
	{
		push_back(*begin());
	};

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
				
		type& operator*() {return *member_container<type, dlist_head, member>(current);};
		type* operator->() {return member_container<type, dlist_head, member>(current);};
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
				
		type& operator*() {return *member_container<type, dlist_head, member>(current);};
		type* operator->() {return member_container<type, dlist_head, member>(current);};
	};

	iterator begin() {return iterator(list.next);};
	iterator end() {return iterator(&list);};
	reverse_iterator rbegin() {return reverse_iterator(list.prev);};
	reverse_iterator rend() {return reverse_iterator(&list);};

	gxx::string to_info() const
	{
		gxx::string str;
		str.reserve(128);
		int i = 0;
		dlist_head* ptr;
		dlist_for_each(ptr, &list)
		{
			i++;
		};
		str << "count: " << i;
		return str;
	};

	gxx::string to_str() const
	{
		gxx::string str;
		str.reserve(128);
		str << "[";
		for (auto r : *this)
		{
			str << to_str(r) << gxx::string(",");
		};
		str << "]";
		return str;
	};
};

template<typename type, dlist_head type::* member>
dlist<type, member>* dlist_head_to_class(dlist_head* head)
{
	using dlist_t = dlist<type, member>;
	return reinterpret_cast<dlist_t*>(reinterpret_cast<char*>(head) - member_offset<dlist_t, dlist_head, &dlist_t::list>());
};

//template<typename type, dlist_head type::* member>
//dlist<type, member>* dlist_get_topic(dlist_head* head)
//{
//	using dlist_t = dlist<type, member>;
//	dlist_t* list = dlist_head_to_class<type,member>(parrent);
//	return list;
//};

//#define dlist_topic(obj,lst) 						\
//({													\
//	using type = decltype(obj);						\
//	dlist_get_topic<type, &type::lst>(&(obj).lst); 	\
//})

#endif