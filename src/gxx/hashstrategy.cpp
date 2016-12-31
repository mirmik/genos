#include <gxx/container/hashstrategy.h>

namespace gxx {

	size_t hash_memstrat70(BasicHashTable* ht) {	
		if (ht->m_tableSize == 0) {
			return 4;
		};
		
		if (ht->m_total < 4) return 0;
		
		if (ht->m_total >= ht->m_tableSize) // > 100%
		{
			return ht->m_tableSize + (ht->m_tableSize >> 1); //150%
		};
		
		if (ht->m_total <= (ht->m_tableSize >> 1)) // < 50%
		{
			return ht->m_tableSize >> 1; //50%
		};
		return 0;
	};

};