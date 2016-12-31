#ifndef GENOS_HASH_TABLE
#define GENOS_HASH_TABLE

#include "assert.h"
#include "inttypes.h"
#include "genos/datastruct/hashtable_head.h"
#include "gxx/container/hlist.h"
#include "gxx/algorithm.h"
#include "mem/allocator.h"
#include "util/retype.h"
//#include "gxx/allocator.h"

template <
typename type, 
hlist_node type::* link, 
typename keytype, 
keytype type::* keyfield
>
class hashtable
{
	using hash_fn_t = uint32_t(*)(keytype);
	using cmp_fn_t = int32_t(*)(keytype,keytype);
	using cell_t = hlist<type,link>;

private:
	hashtable_head ht;

	hash_fn_t hash;
	cmp_fn_t  cmp;

	alloc_ops* alloc;
	int (*strategy) (hashtable_head*); 

	inline cell_t* eval_cell(const keytype& key) {
		return reinterpret_cast<cell_t*>
			(ht.table + (hash(key) % ht.table_size));
	}

	inline cell_t* to_cell(struct hlist_head* ptr) const {
		return reinterpret_cast<cell_t*>(ptr);
	}

	inline void relocate(hashtable_head* dst)
	{
		for (int i = 0; i < ht.table_size; i++)
		{
			RETYPE(cell_t*, srccell, ht.table + i);
			
			auto it = srccell->begin();
			while(it != srccell->end())
			{
				auto next = it.next(); 
				RETYPE(cell_t*, dstcell, dst->table + 
						(hash(*it.*keyfield) % dst->table_size));
				dstcell->push_front(*it);
				it = next;
			};
		};
	};

	inline void check_memstrategy()
	{
		int newsz;
		if (strategy) newsz = strategy(&ht);
		if (newsz == 0) return;

		void* buf = alloc->allocate(newsz * sizeof(cell_t));
 	
		if (!ht.table) 
		{
			hashtable_locate(&ht,buf,newsz * sizeof(cell_t));
			return;
		}

		hashtable_head nht;
		hashtable_locate(&nht,buf,newsz * sizeof(cell_t));

		relocate(&nht);
		hlist_head* oldbuf = ht.table; 
		ht.table = nht.table;
		alloc->deallocate(oldbuf);
		ht.table_size = nht.table_size;
		return;
 	}

public:
	hashtable() : cmp(nullptr), hash(nullptr), strategy(nullptr) 
	{
		hashtable_head_init(&ht);
	};

	void init(void* buf, int len, hash_fn_t _hash, cmp_fn_t _cmp)
	{
		hashtable_locate(&ht, buf, len);
		hash = _hash;
		cmp = _cmp;
		strategy = nullptr;
		alloc = nullptr;
	};

	void init(hash_fn_t _hash, cmp_fn_t _cmp)
	{
		hash = _hash;
		cmp = _cmp;
	};

	void memstrategy(alloc_ops* _alloc, int (*_strategy) (hashtable_head*))
	{
		alloc = _alloc;
		strategy = _strategy;
	}

	void put(type& item)
	{
		assert(hash);
		assert(ht.table || strategy);
		if (strategy) check_memstrategy();
		ht.total++;
		cell_t* cell = eval_cell(item.*keyfield);
		cell->push_front(item);
	};

	type* get(const keytype& key)
	{
		assert(hash);
		assert(cmp);
		assert(ht.table);
		
		cell_t* cell = eval_cell(key);
		auto end = cell->end();
		auto begin = cell->begin();
		auto res = gxx::find_if(begin,end,[=](type& item)
		{
			return cmp(item.*keyfield, key) == 0;
		});
		return res == end ? (type*)0 : &*res;												
	};

	type* del(const keytype& key)
	{
		assert(hash);
		assert(cmp);
		assert(ht.table);
		
		cell_t* cell = eval_cell(key);
		auto end = cell->end();
		auto begin = cell->begin();
		auto res = gxx::find_if(begin,end,[=](type& item)
		{
			return cmp(item.*keyfield, key) == 0;
		});
		if (res == end) return 0;
		cell->pop(*res);
		ht.total--;
		if (strategy) check_memstrategy();
		return &*res;
	};

	gxx::string to_info() const
	{
		gxx::string str;
		str.reserve(128);
		str << "[";
		for(int i = 0; i < ht.table_size; i++)
			str << to_cell(ht.table + i)->total() << gxx::string(",");
		str << "]";
		return str;
	}
};

static int hash_memstrat70(hashtable_head* ht)
{
//	assert(ht);

	if (ht->table == nullptr)
	{
		return 4;
	};

	if (ht->total < 4) return 0;

	if (ht->total >= ht->table_size) // > 100%
	{
		return ht->table_size + (ht->table_size >> 1); //150%
	};

	if (ht->total <= (ht->table_size >> 1)) // < 50%
	{
		return ht->table_size >> 1; //50%
	};

	return 0;
};

#endif