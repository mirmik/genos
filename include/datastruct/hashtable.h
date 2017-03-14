#ifndef GENOS_DATASTRUCT_HASHTABLE_H
#define GENOS_DATASTRUCT_HASHTABLE_H

#include <compiler.h>
#include <stdlib.h>

#include <datastruct/hlist_head.h>
#include <datastruct/array.h>

typedef int 	(*ht_equal_t) 		(void* lkey, void* rkey);
typedef size_t 	(*ht_hash_t) 		(void* key);
typedef void* 	(*ht_getkey_t) 		(struct hlist_node* lnk);

struct hashtable {
	struct hlist_head* 	table;
	size_t 				tblsize;

	ht_getkey_t 	getkey;
	ht_equal_t 		equal;
	ht_hash_t 		hash;
};

__BEGIN_DECLS

void hashtable_init(struct hashtable * ht, void* buffer, size_t sz, ht_getkey_t getkey, ht_hash_t hash, ht_equal_t equal) {
	ht->table = buffer;
	ht->tblsize = sz;
	ht->equal = equal;
	ht->getkey = getkey;
	ht->hash = hash;

	array_for_each(it, ht->table, ht->tblsize) {
		hlist_head_init(it);
	}
}

static inline void __hashtable_put_to_cell(struct hlist_head* tbl, struct hlist_node* node, size_t cell) {
	struct hlist_head* hcell = tbl + cell;
	hlist_add_next(node, &hcell->first);
}

void hashtable_put(struct  hashtable* ht, struct hlist_node* node) {
	void* key = ht->getkey(node);
	size_t hash = ht->hash(key);
	__hashtable_put_to_cell(ht->table, node, hash % ht->tblsize);
}

struct hlist_node* hashtable_get(struct  hashtable* ht, void* key) {
	size_t hash = ht->hash(key);	
	struct hlist_node* pos;

	hlist_for_each(pos, ht->table + hash % ht->tblsize) {
		if (ht->equal(ht->getkey(pos), key)) return pos;
	}
	return NULL;
}

void debug_print_hashtable(struct hashtable* ht) {
	for (int i = 0; i < ht->tblsize; i++) {
		int n = 0;
		struct hlist_node * curnode;
		hlist_for_each(curnode, (ht->table + i)) {
			n++;
		}
		debug_printdec_int32(n); 
		debug_print(" ");
	}
}

__END_DECLS

#define hashtable_get_typed(ht,key,type,lnk) \
	mcast_out_or_null(hashtable_get(ht,key),type,lnk)

#define __hashtable_for_each(pos,ht,head) array_for_each(head,(ht)->table,(ht)->tblsize) hlist_for_each(pos,head)
#define hashtable_for_each(pos,ht) __hashtable_for_each(pos,ht,MACRO_GUARD(head))

#define __hashtable_for_each_entry(pos,ht,lnk,head) array_for_each(head,(ht)->table,(ht)->tblsize) hlist_for_each_entry(pos,head,lnk)
#define hashtable_for_each_entry(pos,ht,lnk) __hashtable_for_each_entry(pos,ht,lnk,MACRO_GUARD(head))

#endif