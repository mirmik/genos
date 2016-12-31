#ifndef DATASTRUCT_CVECTOR_H
#define DATASTRUCT_CVECTOR_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <assert.h>

#include <datastruct/dynarray.h>

struct cvector_t {
	dynarray_t da;
	size_t elsz;
	size_t sz;
};

typedef struct cvector_t cvector_t;

static inline void cvector_invalidate(struct cvector_t * cv) {
	dynarray_invalidate(&cv->da);
}

static inline void cvector_init(struct cvector_t * cv, size_t elsz) {
	dynarray_init(&cv->da);
	cv->sz = 0;
	cv->elsz = elsz;
}

static inline void* cvector_paste(struct cvector_t * cv, size_t ppos, size_t psz) {
	void* ret = dynarray_paste(&cv->da, cv->elsz * cv->sz, cv->elsz * ppos, cv->elsz * psz);
	cv->sz++;
	return (char*)cv->da.data + cv->elsz * ppos;
}

static inline void cvector_remove(struct cvector_t * cv, size_t ppos, size_t psz) {
	assert(cv->sz >= ppos + psz);

	dynarray_remove(&cv->da, cv->sz * cv->elsz, ppos * cv->elsz, psz * cv->elsz);	
	cv->sz -= psz;
}

static inline unsigned char cvector_shrink(struct cvector_t * cv) {
	dynarray_changeBuffer(&cv->da, cv->sz * cv->elsz); 
}

static inline unsigned char cvector_reserve(struct cvector_t * cv, size_t cap) {
	return cv->da.cap >= cap ? 1 : dynarray_changeBufferCopy(&cv->da, cap * cv->elsz, cv->sz * cv->elsz) ? 1 : 0; 
}

static inline void* cvector_cell(struct cvector_t * cv, size_t cell) {
	return (char*)cv->da.data + cv->elsz * cell; 
}

static inline unsigned char cvector_setSize(struct cvector_t * cv, size_t sz) {
	unsigned char ret = cvector_reserve(cv, sz);
	if (ret) cv->sz = sz;
	return ret;
}

static inline void* cvector_engage_back(struct cvector_t * cv) {
	unsigned char ret = cvector_reserve(cv, cv->sz + 1);
	if (ret == 0) return NULL;
	return (char*)cv->da.data + cv->elsz * cv->sz++;	
}

#define cvector_ref(cv,type,num) \
	(*(type*)cvector_cell(cv,num))

#define cvector_ptr(cv,type,num) \
	((type*)cvector_cell(cv,num))

#define cvector_array(cv,type) \
	((type*)(cv)->data)

#define cvector_pushback(cv,type,nelem) \
	(*(type*)cvector_engage_back(cv) = nelem)

#define cvector_pushfront(cv,type,nelem) \
	(*(type*)cvector_paste(cv,0,1) = nelem)

#define cvector_insert(cv,type,pos,nelem) \
	(*(type*)cvector_paste(cv,pos,1) = nelem)
	
#define cvector_popback(cv) \
	((cv)->sz--)

#define cvector_popfront(cv) \
	(cvector_remove(cv,0,1))

#define cvector_foreach(it,cv) \
	for (it = (typeof(it))((cv)->da.data); it != ((typeof(it))((cv)->da.data) + (cv)->sz); it++)

#endif