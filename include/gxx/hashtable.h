#ifndef GENOS_HASHTABLE_H
#define GENOS_HASHTABLE_H

#include <datastruct/hlist_head.h>
#include <datastruct/array.h>
#include <gxx/container/hash.h>
#include <gxx/container/compare.h>

namespace gxx {

	template <size_t TableSize, typename T, typename K, hlist_node T::* lnk, 
		K&(*GetKey)(T&) = T::getkey> 
	class static_hashtable {
		hlist_head table[TableSize];

	public:
		static_hashtable() {
			array_for_each(it, &table[0], TableSize) {
				hlist_head_init(it);
			}
		}	

		void put(T& item) {
			hlist_node* node = &(item.*lnk);
			K& key = GetKey(item);
			size_t hash = gxx::hash(key);
			//dprln(hash);
			__hashtable_put_to_cell(table, node, hash % TableSize);
		}
		
		T* get(const K& key) {
			size_t hash = gxx::hash(key);	
			struct hlist_node* pos;
		
			//dprln(hash);

			hlist_for_each(pos, table + hash % TableSize) {
				if (gxx::equal(GetKey(*member_container(pos, lnk)), key)) 
					return member_container(pos, lnk);
			}
			return nullptr;
		}

		void dump() {
			for (int i = 0; i < TableSize; i++) {
				int n = 0;
				struct hlist_node * curnode;
				hlist_for_each(curnode, (table + i)) {
					n++;
				}
				debug_printdec_int32(n); 
				debug_print(" ");
			}
		}

		template <typename Function>
		void foreach(Function func) {
			array_for_each(hl, &table[0], TableSize) {
				struct hlist_node * pos;
				hlist_for_each(pos, (hl)) {
					func(*member_container(pos, lnk));
				}
			}
		}

		template <typename Function>
		T* find(Function func) {
			array_for_each(hl, &table[0], TableSize) {
				struct hlist_node * pos;
				hlist_for_each(pos, (hl)) {
					T& el = *member_container(pos, lnk);
					if (func(el)) return &el;
				}
			}
			return nullptr;
		}

	private:

		void __hashtable_put_to_cell(struct hlist_head* tbl, struct hlist_node* node, size_t cell) {
			struct hlist_head* hcell = tbl + cell;
			hlist_add_next(node, &hcell->first);
		}
	};
}

#endif