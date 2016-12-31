#ifndef GENOS_HLIST_H
#define GENOS_HLIST_H

#include "genos/datastruct/hlist_head.h"
#include "utilxx/member.h" 
#include "gxx/to_string.h"

template<typename type, hlist_node type::* member>
class hlist
{
private:
	hlist_head list;
public:

	hlist()
	{
		hlist_head_init(&list);
	}

	~hlist(){}

	void init()
	{
		hlist_head_init(&list);
	}

	void push_front(type& obj)
	{
		hlist_add_next(&(obj.*member), &(list.first));
	}
		
	//void push_next(type& obj, type& head)
	//{
	//	hlist_add_next(&(obj.*member), &(head.*member));
	//}

	void pop(type& obj)
	{
		hlist_del(&(obj.*member));
	}

	bool is_empty()
	{
		return list.first == nullptr;
	}

	int total()
	{
		int n = 0;
		hlist_node* pos;
		hlist_for_each(pos,&list) n++;
		return n;
	};

	class iterator{
		public:
		hlist_node* current;
		public:
		iterator(hlist_node* head) : current(head) {};		
				
		iterator operator++(int) { iterator i = *this; current=current->next; return i; }
		iterator operator++() { current=current->next; return *this; }
		bool operator!= (const iterator& b) {return current != b.current;};
		bool operator== (const iterator& b) {return current == b.current;};
		iterator next() { return iterator(current->next); };
				
		type& operator*() {return *member_container<type, hlist_node, member>(current);};
		type* operator->() {return member_container<type, hlist_node, member>(current);};
	};

	iterator begin() {return iterator(list.first);};
	iterator end() {return iterator(0);};

	gxx::string to_info()
	{
		gxx::string str;
		str.reserve(128);
		for (auto r : *this)
		{
			str << "next: " << gxx::string((uintptr_t)(r.*member).next,16) << "\r\n";
			str << "pprev: " << gxx::string((uintptr_t)(r.*member).pprev,16) << "\r\n";
		};
		return str;
	};

	gxx::string to_str()
	{
		gxx::string str;
		str.reserve(128);
		str << "[";
		for (auto r : *this)
		{
			str << ::to_str(r) << gxx::string(",");
		};
		str << "]";
		return str;
	};
};



#endif