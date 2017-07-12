#ifndef GENOS_TERM_LIST_H
#define GENOS_TERM_LIST_H

#include "gxx/container/hashtable.h"
#include "genos/sigslot/delegate.h"
#include "mem/allocator.h"
#include "crypto/crc.h"

template<typename R, typename ... Args>
struct termlist_elem
{
	hlist_node link;
	const char* name;
	delegate<R, Args ...> func;
};

template<typename R, typename ... Args>
class termlist
{
	using elem_t = termlist_elem<R,Args...>;
	using deleg_t = delegate<R,Args...>;

private:
	hashtable<
	elem_t, 
	&elem_t::link, 
	const char*, 
	&elem_t::name
	> ht;

public:
	termlist()
	{
		ht.init(strcrc8,strcmp);
		ht.memstrategy(&malloc_ops, hash_memstrat70);
	}

	void add(const char* name, const deleg_t& d)
	{
		elem_t* t = (elem_t*) malloc(sizeof(elem_t));
		t->name = name;
		t->func = d;
		ht.put(*t);
	}

	void add(const char* name, R(*func)(Args...))
	{
		elem_t* t = (elem_t*) malloc(sizeof(elem_t));
		t->name = name;
		t->func = deleg_t(func);
		ht.put(*t);
	}

	int find(const char* name, deleg_t& d)
	{
		elem_t* t = ht.get(name);
		if (t == 0) return -1;
		d = t->func;
		return 0; 
	};
};

#endif