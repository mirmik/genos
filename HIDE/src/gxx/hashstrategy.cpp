#include <gxx/container/hashstrategy.h>

namespace gxx {

	size_t hash_memstrat70(BasicHashTable* ht) {	
		if (ht->m_htable.size() == 0) {
			return 4;
		};
		
		if (ht->m_total < 4) return 0;
		
		if (ht->m_total >= ht->m_htable.size()) // > 100%
		{
			return ht->m_htable.size() + (ht->m_htable.size() >> 1); //150%
		};
		
		if (ht->m_total <= (ht->m_htable.size() >> 1)) // < 50%
		{
			return ht->m_htable.size() >> 1; //50%
		};
		return 0;
	};

};