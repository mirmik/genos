#ifndef GENOS_DICTIONARY_H
#define GENOS_DICTIONARY_H

#include "genos_configure.h"
#include "gxx/HashTable.h"

namespace gxx {

	template <typename K, typename V>
	struct DictNode {
		hlist_node hnode;
		K key;
		V value;
	
		DictNode(const K& _key, const V& _value) 
		: key(_key), value(_value) {};

		DictNode(const K& _key): key(_key), value() {};
	};
	#define __dictnode_helper DictNode<K,V> 

	template < typename K, typename V > 
	class Dictionary : private GXX_HASHTABLE(__dictnode_helper,hnode,key) {
		
		using Node = DictNode<K,V>;
		using Parent = GXX_HASHTABLE(__dictnode_helper,hnode,key);

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
	};
};

#endif