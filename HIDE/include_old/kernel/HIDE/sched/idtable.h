#ifndef GENOS_IDTABLE_H
#define GENOS_IDTABLE_H

#include "inttypes.h"
#include "virtee.h"
#include "gxx/container/hashtable.h"
#include "defines/id_t.h"
#include "genos/panic.h"

static uint32_t idhash(id_t k) {
	return k;
}

static int32_t idcmp(id_t k0, id_t k1) {
	return k1 - k0;
}

template <
	typename type, 
	hlist_node type::* 
	fld_lnk, 
	id_t type::* fld_id,
	int ht_size
	>
class idtable {
private:
	char hashbuf[sizeof(hlist_head) * ht_size];
	hashtable<type, fld_lnk, id_t, fld_id> ht;
	id_t idcount;

public:
	id_t getnewid() {
		do { ++idcount; } 
			while(idcount == 0 || find(idcount) != 0);
		return idcount;
	}

	id_t registry(type* obj) {
		obj->*fld_id = getnewid(); 
		ht.put(*obj);
	}
	
	void unregistry(id_t) {
		panic("TODO unreg");
	}
	
	type* find(id_t id) {
		return &*ht.get(id);
	}

	idtable() {
		ht.init(hashbuf, sizeof(hlist_head) * ht_size, idhash, idcmp);
	};
};

#endif