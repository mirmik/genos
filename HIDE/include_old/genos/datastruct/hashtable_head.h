#ifndef GENOS_HASHTABLE_HEAD
#define GENOS_HASHTABLE_HEAD

#include <genos/defines/size_t.h>
#include "genos/datastruct/hlist_head.h"

struct hashtable {
	struct hlist_head* m_table;
	size_t m_tableSize;
	size_t m_total;

	int (*m_strategy) (hashtable*); 

	hashtable() : table(nullptr), tableSize(0), total(0) {}	
};

static int hash_memstrat70(hashtable_head* ht) {	
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