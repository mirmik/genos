#ifndef GENOS_HASHTABLE_HEAD
#define GENOS_HASHTABLE_HEAD

#include <gxx/Allocator.h>
#include <datastruct/hlist_head.h>
//#include <gxx/container/hashstrategy.h>

namespace gxx {

	class BasicHashTable : public BasicAllocated {
	public:
		gxx::slice<hlist_head> m_htable;
		size_t m_total;
		size_t (*m_strategy) (BasicHashTable*);

	public:
		BasicHashTable() : m_htable(nullptr, 0), m_total(0), m_strategy(nullptr) {}	

		~BasicHashTable() {
			if (m_htable.data()) m_alloc->deallocate(m_htable.data());
		}

		void reserve(size_t sz) {
			hlist_head* newtbl = (hlist_head*) m_alloc->constructArray<hlist_head>(sz);
			if (!is_valid()) {
				m_htable.data(newtbl);
				m_htable.size(sz);
				m_total = 0;
				return;
			}
			relocate(newtbl, sz);
			m_alloc->deallocate(m_htable.data());
			m_htable.data(newtbl);
			m_htable.size(sz);
		}

		void setStrategy(size_t (*func) (BasicHashTable*)) {
			m_strategy = func;
		};

		virtual void relocate(hlist_head* dst, size_t dstsize) = 0;

		bool is_valid() const {
			return m_htable.data();
		}
	};
	
};

#endif