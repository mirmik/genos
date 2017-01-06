#ifndef GENOS_SMACH_H
#define GENOS_SMACH_H

#include <stdlib.h>
#include <datastruct/dlist_head.h>

#define SMACH_OK 0
#define SMACH_ERROR_LENGTH 1
#define SMACH_ERROR_DATA 2
#define SMACH_ERROR_ALLOCATE 3
#define SMACH_ERROR_EMPTY 4

struct smobj_s;

struct smach_s {
	struct dlist_head list;
	size_t deep;

	void*(*allocate)(size_t size);
	void(*deallocate)(void*);
};

struct smobj_s {
	struct dlist_head lnk;
	void* data;
	size_t size;
	void(*destruct)(struct smobj_s* self, struct smach_s* smach);
};

void smach_init(struct smach_s * smach, 
	void*(*allocate)(size_t size),
	void(*deallocate)(void*));

struct smobj_s * smach_constructAllocated	(struct smach_s*, void* data, size_t size);
struct smobj_s * smach_constructPlaced		(struct smach_s*, void* data, size_t size);

int smach_pushFront(struct smach_s * smach, struct smobj_s * add);
int smach_pushBack(struct smach_s * smach, struct smobj_s * add);

int smach_popFront(struct smach_s * smach);
int smach_popBack(struct smach_s * smach);

struct smobj_s * smach_getObject(struct smach_s * smach, int index);

int smach_popAll(struct smach_s * smach);

#endif