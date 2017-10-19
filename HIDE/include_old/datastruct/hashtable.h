#ifndef GENOS_DATASTRUCT_HASHTABLE_H
#define GENOS_DATASTRUCT_HASHTABLE_H

#include <sys/cdefs.h>
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

static inline void hashtable_init(struct hashtable * ht, void* buffer, size_t sz, ht_getkey_t getkey, ht_hash_t hash, ht_equal_t equal) {
	ht->table = (struct hlist_head *)buffer;
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

#define hashtable_get_entry(ht,key,type,lnk) \
	mcast_out_or_null(hashtable_get(ht,key),type,lnk)

/*#define __hashtable_for_each(pos,ht,head) array_for_each(head,(ht)->table,(ht)->tblsize) hlist_for_each(pos,head)
#define hashtable_for_each(pos,ht) __hashtable_for_each(pos,ht,MACRO_GUARD(head))

#define __hashtable_for_each_entry(pos,ht,lnk,head) array_for_each(head,(ht)->table,(ht)->tblsize) hlist_for_each_entry(pos,head,lnk)
#define hashtable_for_each_entry(pos,ht,lnk) __hashtable_for_each_entry(pos,ht,lnk,MACRO_GUARD(head))
*/

static inline struct hlist_head* ht_next_nempty_head(struct hlist_head* head, struct hlist_head* hend) {
	for(;head != (hend);head++) if (head->first != NULL) return head; 				
	return NULL;					
}

static inline struct hlist_head* __hashtable_for_each_initializer(struct hlist_head* head, struct hlist_head* hend, struct hlist_node** pos) {
	head = ht_next_nempty_head(head, hend);
	*pos = head == NULL ? NULL : (head)->first;
	return head;
}

#define __hashtable_for_each(pos,head,ht,hend) \
	for(struct hlist_head *hend = (ht)->table + (ht)->tblsize, *head = __hashtable_for_each_initializer((ht)->table, hend, &pos); \
		pos != NULL; \
		pos = pos->next == NULL ? ({head = ht_next_nempty_head(head + 1, hend); head == NULL ? NULL : (head)->first;}) : (pos)->next)

#define hashtable_for_each(pos,ht) __hashtable_for_each(pos,MACRO_GUARD(head),ht,MACRO_GUARD(hend))


#define ____hashtable_for_each_entry_initializer(table,hend,pos,hd,lnk) ({	\
	struct hlist_head* hd = ht_next_nempty_head(table,hend);					\
	pos = hlist_first_entry(hd, decltypeof(*pos), lnk);	\
	hd;	\
})
#define __hashtable_for_each_entry_initializer(table,hend,pos,lnk) \
	____hashtable_for_each_entry_initializer(table,hend,pos,MACRO_GUARD(head),lnk)

#define __hashtable_for_each_entry(pos,head,ht,hend,lnk) \
	for(struct hlist_head *hend = (ht)->table + (ht)->tblsize, *head = __hashtable_for_each_entry_initializer((ht)->table,hend,pos,lnk); \
		pos != NULL; \
		pos = &hlist_next_entry(pos,lnk)->lnk == NULL ? ({head = ht_next_nempty_head(head + 1, hend); head == NULL ? NULL : hlist_first_entry(head,decltypeof(*pos),lnk);}) : hlist_next_entry(pos,lnk))

#define hashtable_for_each_entry(pos,ht,lnk) __hashtable_for_each_entry(pos,MACRO_GUARD(head),ht,MACRO_GUARD(hend),lnk)

#endif