#include <stream/strqueue.h>
#include "assert.h"
#include <string.h>
#include <util/math.h>

strqueue::record* strqueue::create(const void* buf, int len) {
	record* rec = (record*) sysalloc(sizeof(record));
	dlist_init_list(&rec->lnk);
	rec->buf = sysalloc(len);
	rec->len = len;
	memcpy(rec->buf, buf, len);
	total++;
	return rec;
}

int strqueue::push_back(const void* buf, int len) {
	assert(buf);
	record* rec = create(buf,len);
	list.push_back(*rec);
};

int strqueue::push_front(const void* buf, int len) {
	assert(buf);
	record* rec = create(buf,len);
	list.push_front(*rec);
};
	
int strqueue::pop(strqueue::record* rec, void* buf, int len) {
	int ret = min(rec->len, len);
	memcpy(buf, rec->buf, ret);
	dlist_del(&rec->lnk);
	sysfree(rec->buf);
	sysfree(rec);
	total--;
	return ret;
};

int strqueue::pop_back(void* buf, int len) {
	assert(total != 0);
	record* rec = &*list.rbegin();
	int ret = pop(rec, buf, len);
	return ret;
};

int strqueue::pop_front(void* buf, int len) {
	assert(total != 0);
	record* rec = &*list.begin();
	int ret = pop(rec, buf, len);
	return ret;
};