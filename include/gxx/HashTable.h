#ifndef GENOS_HASH_TABLE
#define GENOS_HASH_TABLE

#include <gxx/Allocator.h>
#include <gxx/container/hashstrategy.h>
#include <gxx/container/hashtable.h>
#include <gxx/container/hash.h>
#include <gxx/container/compare.h>
#include <gxx/algorithm.h>
#include <gxx/HList.h>

#include <assert.h>

namespace gxx {

	template <typename T, hlist_node T::* link, typename K, K T::*keyfield>	
	class HashTable : public BasicHashTable {
		using hlist = HList<T,link>;

	private:
		void put_to_cell(hlist_head* h, size_t sz, T& obj) {
			size_t cellnum = gxx::hash(obj.*keyfield, gxx::defaultSeed) % sz;
			hlist& cell = *(hlist*)(h + cellnum);
			cell.push_front(obj);						
		}

	public:
		HashTable() : BasicHashTable() {};
		
		~HashTable() {}

		void relocate(hlist_head* pdst, size_t dstsize) {
			for (int i = 0; i < m_tableSize; i++) {
				hlist& cell = *(hlist*)(m_table + i);
				typename hlist::iterator it = cell.begin(); 
				while(it != cell.end()) {
					T& r = *it;
					++it;
					put_to_cell(pdst, dstsize, r);	
				}
			}
		};

		void put(T& obj) {
			m_total++;
			assert((m_table && m_tableSize != 0) || m_strategy);
			check_memstrategy();
			put_to_cell(m_table, m_tableSize, obj);
		}

		bool get(const K& key, T*& ptr) const {
			if (!is_valid()) return false;
			size_t cellnum = gxx::hash(key, gxx::defaultSeed) % m_tableSize;
			hlist& cell = *(hlist*)(m_table + cellnum);
			auto it = gxx::find_if(cell.begin(), cell.end(), [key](const T& ref) -> bool {
				return gxx::compare(ref.*keyfield, key);
			});
			if (it == cell.end()) return false;
			ptr = &*it; 
			return true;
		}

		void check_memstrategy() {
			if (m_strategy) {
				size_t newsize = m_strategy(this);
				if (newsize == 0) return;
				reserve(newsize);
			}
		};

		gxx::string to_info() const
		{
			gxx::string str;
			str.reserve(128);
			str << "[";
			for(int i = 0; i < m_tableSize; i++)
				str << ((hlist*)(m_table + i))->total() << ',';
			str << "]";
			return gxx::move(str.shrink_to_print());
		}
	
	};
};

#define GXX_HASHTABLE(type,link,key) \
gxx::HashTable<type,&type::link,decltype(type::key),&type::key>
	/*template < typename T, hlist_node type::* link, typename K, K type::* keyfield>
	class HashTable
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
	
	};
*/


	
#endif