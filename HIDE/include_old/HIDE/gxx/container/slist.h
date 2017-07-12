#ifndef GENOS_SLIST_H
#define GENOS_SLIST_H

#include "genos/datastruct/slist_head.h"
#include "utilxx/member.h" 
#include "utilxx/string.h"

template<typename type, slist_head type::* member>
class slist 
{
public:
	slist_head list;

	slist()
	{
		slist_init(&list);
	}

	~slist(){}

	void init()
	{
		slist_init(&list);
	};

	bool is_empty()
	{
		return slist_is_empty(&list);
	};

	void push_front(type& obj)
	{
		slist_connect_after(&(obj.*member), &list);
	};
	
	void push_next(type& obj, type& head)
	{
		slist_connect_after(&(obj.*member), &(head.*member));
	};
	
	slist& operator= (slist_head* l)
	{
		list = l;
	};

	class iterator{
		public:
		slist_head* current;
		public:
		iterator(slist_head* head) : current(head) {};		
				
		iterator operator++(int) { iterator i = *this; current=current->next; return i; }
		iterator operator++() { current=current->next; return *this; }
		bool operator!= (const iterator& b) {return current != b.current;};
		bool operator== (const iterator& b) {return current == b.current;};
				
		type& operator*() {return *member_container<type, slist_head, member>(current);};
		type* operator->() {return member_container<type, slist_head, member>(current);};
	};

	iterator begin() {return iterator(list.next);};
	iterator end() {return iterator(&list);};

	gxx::string to_info()
	{
		gxx::string str;
		str.reserve(128);
		int i = 0;
		slist_head* ptr;
		slist_for_each(ptr, &list)
		{
			i++;
		};
		str << "count: " << i;
		return str;
	};

	gxx::string to_str()
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

template<typename type, slist_head type::* member>
slist<type, member>* slist_head_to_class(slist_head* head)
{
	using slist_t = slist<type, member>;
	return reinterpret_cast<slist_t*>(reinterpret_cast<char*>(head) - member_offset<slist_t, slist_head, &slist_t::list>());
};

#endif