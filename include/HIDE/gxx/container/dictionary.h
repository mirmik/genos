#ifndef GENOS_DICTIONARY_H
#define GENOS_DICTIONARY_H

#include "genos_configure.h"
#include "gxx/allocator.h"
#include "gxx/container/hashtable.h"
#include "gxx/container/traits.h"

template <typename keytype, typename valtype>
struct dictnode {
	hlist_node hnode;
	keytype key;
	valtype val;

	dictnode(const keytype& _key, const valtype& _val) 
	: key(_key), val(_val) {};
};

template <
	typename keytype, 
	typename valtype 	
	> 
class dictionary {
	
	using hashtable_t = hashtable<
	dictnode<keytype,valtype>, &dictnode<keytype,valtype>::hnode, 
	keytype, &dictnode<keytype,valtype>::key
	>;

private:
	hashtable_t ht;

	alloc_ops* alloc;

public:
	dictionary(alloc_ops* _alloc) {
		alloc = _alloc;
		ht.init(traits<keytype>::hash_fn,traits<keytype>::cmp_fn);
		ht.memstrategy(_alloc, hash_memstrat70);
	};

	dictionary() : dictionary(&malloc_ops) {};

	void insert(const keytype& key,const valtype& val) {
		dictnode<keytype,valtype>* dn = new dictnode<keytype,valtype>(key,val);
		ht.put(*dn);
	};

	//valtype operator[](const keytype& key) {
	//	return ht.get(key)->val;
	//};

	int get(valtype& tgt, const keytype& key) {
		auto ret = ht.get(key);
		if (ret == nullptr) return -1;
		tgt = ret->val;
		return 0;	
	};

	bool contains(const keytype& key) {
		auto ret = ht.get(key);
		if (ret == nullptr) return false;
		return true;	
	}

	valtype& operator[](const keytype& key) {
		auto ret = ht.get(key);
		if (ret == nullptr) dprln("AllBad");
		return ret->val;
	}

	const hashtable_t& get_table() {
		return ht;
	};
};

#endif