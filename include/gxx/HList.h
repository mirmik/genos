#ifndef GENOS_HLIST_H
#define GENOS_HLIST_H

#include "utilxx/member.h" 
#include "datastruct/hlist_head.h"
#include "gxx/container/to_string.h"

template<typename type, hlist_node type::* member>
class HList : public hlist_head
{
public:
	HList() : hlist_head() {}

	~HList(){}

	void init() {
		hlist_head_init(this);
	}

	void push_front(type& obj) {
		hlist_add_next(&(obj.*member), &(first));
	}
		
	void pop(type& obj) {
		hlist_del(&(obj.*member));
	}

	bool empty() {
		return first == nullptr;
	}

	int total()	{
		int n = 0;
		hlist_node* pos;
		hlist_for_each(pos,this) n++;
		return n;
	};

	class iterator {
	public:
		hlist_node* current;
	public:
		iterator(hlist_node* head) : current(head) {};	
		iterator(const iterator& b) : current(b.current) {};		
				
		iterator operator++(int) { iterator i = *this; current=current->next; return i; }
		iterator operator++() { current=current->next; return *this; }
		bool operator!= (const iterator& b) {return current != b.current;};
		bool operator== (const iterator& b) {return current == b.current;};
		iterator& operator= (const iterator& b) { current = b.current; return *this; }
		iterator next() { return iterator(current->next); };
				
		type& operator*() {return *member_container(current, member);};
		type* operator->() {return member_container(current, member);};
	};

	iterator begin() {return iterator(first);};
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
		return gxx::move(str.shrink_to_print());
	};

	gxx::string to_str()
	{
		gxx::string str;
		str.reserve(128);
		str << "[";
		for (auto r : *this)
		{
			str << gxx::to_str(r) << gxx::string(",");
		};
		str << "]";
		return gxx::move(str.shrink_to_print());
	};
};



#endif