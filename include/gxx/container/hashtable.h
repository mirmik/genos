#ifndef GENOS_HASHTABLE_HEAD
#define GENOS_HASHTABLE_HEAD

#include <gxx/Allocator.h>
#include <datastruct/hlist_head.h>
//#include <gxx/container/hashstrategy.h>

namespace gxx {

	struct BasicHashTable : public BasicAllocated {
		struct hlist_head* m_table;
		size_t m_tableSize;
		size_t m_total;
		size_t (*m_strategy) (BasicHashTable*);

	public:
		BasicHashTable() : m_table(nullptr), m_tableSize(0)
			, m_total(0), m_strategy(nullptr) {}	

		~BasicHashTable() {
			if (m_table) m_alloc->deallocate(m_table);
		}

		void reserve(size_t sz) {
			hlist_head* newtbl = (hlist_head*) 
				m_alloc->constructArray<hlist_head>(sz);
			if (!is_valid()) {
				m_table = newtbl;
				m_tableSize = sz;
				m_total = 0;
				return;
			}
			relocate(newtbl, sz);
			m_alloc->deallocate(m_table);
			m_table = newtbl;
			m_tableSize = sz;
		}

		void setStrategy(size_t (*func) (BasicHashTable*)) {
			m_strategy = func;
		};

		virtual void relocate(hlist_head* dst, size_t dstsize) = 0;

		bool is_valid() const {
			return m_table;
		}
	};
	
};

#endif