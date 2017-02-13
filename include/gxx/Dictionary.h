#ifndef GENOS_DICTIONARY_H
#define GENOS_DICTIONARY_H

#include "genos_configure.h"
#include "gxx/HashTable.h"

namespace gxx {

	template <typename K, typename V>
	struct DictNode {
		hlist_node hnode;

		K& key = kv.first;
		V& value = kv.second;
	
		DictNode(const K& _key, const V& _value) 
		: kv(_key, _value) {};

	public:
		gxx::pair<K,V> kv;

		//DictNode(const K& _key) : kv(_key, V()) {};

		size_t hash(uint32_t seed) const {
			return gxx::hash(key, seed);
		}

		const K& getkey() const {
			return key;
		}
	};
	#define __dictnode_helper DictNode<K,V> 

	template < typename K, typename V > 
	class Dictionary : public HashTable<DictNode<K,V>, &DictNode<K,V>::hnode> {
		
		using Node = DictNode<K,V>;
		using Parent = HashTable<DictNode<K,V>, &DictNode<K,V>::hnode>;

	public:
		Dictionary() : Parent() {
			BasicHashTable::setStrategy(gxx::hash_memstrat70);
		}

		Node* insert(const K& key, const V& value) {
			Node* node = BasicHashTable::m_alloc->constructObject<Node>(key, value);
			this->put(*node);
			return node;
		}

		bool contains(const K& key) const {
			Node* ignore;
			return Parent::get(key,ignore);
		}

		V& operator[] (const K& key) {
			Node* node;
			auto ret = Parent::get(key,node);
			if (ret == false) node = insert(key, V());
			return node->value;
		}

		bool get(const K& key, V& ref) {
			Node* node;
			auto ret = Parent::get(key,node);
			if (ret == false) return false;
			ref = node->value;
			return true;	
		} 

		/*class iterator : public Parent::iterator {
		public:
			iterator(gxx::objbuf<typename Parent::hlist>& _table, typename gxx::objbuf<typename Parent::hlist>::iterator _ittbl, typename Parent::hlist::iterator _itcell) 
				: Parent::iterator(_table, _ittbl, _itcell) {}		
			
			iterator(gxx::objbuf<typename Parent::hlist>& _table) : Parent::iterator(_table) {}

			gxx::pair<K,V>& operator*() { 
			//	return Parent::iterator::operator*().kv;
			}
			gxx::pair<K,V>* operator->() = delete;
		};

		iterator begin() { 
			return iterator(Parent::m_htable); 
		}

		iterator end() { 
			//return iterator(Parent::m_htable, Parent::m_htable.end(), nullptr); 
		}*/
	};
};

#endif