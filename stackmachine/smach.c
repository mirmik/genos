#include "smach.h"
#include <string.h>

void smach_init(struct smach_s * smach, 
	void*(*allocate)(size_t size),
	void(*deallocate)(void*))
{
	smach->allocate = allocate;
	smach->deallocate = deallocate;
	smach->deep = 0;
	dlist_init(&smach->list);
}

static void smobj_destruct_allocated(struct smobj_s* self, struct smach_s* smach) {
	smach->deallocate(self->data);
	smach->deallocate(self);
}

static void smobj_destruct_placed(struct smobj_s* self, struct smach_s* smach) {
	smach->deallocate(self);	
}

struct smobj_s * smach_constructAllocated (struct smach_s* smach, void* data, size_t size) {
	struct smobj_s * obj = (struct smobj_s *) smach->allocate(sizeof(struct smobj_s));
	obj->data = smach->allocate(size);
	memcpy(obj->data, data, size);
	obj->size = size;
	obj->destruct = &smobj_destruct_allocated;
	return obj;
}

struct smobj_s * smach_constructPlaced (struct smach_s * smach, void* data, size_t size) {
	struct smobj_s * obj = (struct smobj_s *) smach->allocate(sizeof(struct smobj_s));
	obj->data = data;
	obj->size = size;
	obj->destruct = &smobj_destruct_placed;
	return obj;
}

int smach_pushFront(struct smach_s * smach, struct smobj_s * add) {
	dlist_add_front(&add->lnk, &smach->list);
	smach->deep++;
	return SMACH_OK;
}

int smach_pushBack(struct smach_s * smach, struct smobj_s * add) {
	dlist_add_back(&add->lnk, &smach->list);
	smach->deep++;
	return SMACH_OK;
}

int smach_popFront(struct smach_s * smach) {
	if (smach->deep == 0) return SMACH_ERROR_EMPTY;
	struct smobj_s * obj = dlist_first_entry(&smach->list, struct smobj_s, lnk);
	dlist_del(&obj->lnk);
	obj->destruct(obj, smach);
	smach->deep--;
	return SMACH_OK;
}

int smach_popBack(struct smach_s * smach) {
	if (smach->deep == 0) return SMACH_ERROR_EMPTY;
	struct smobj_s * obj = dlist_last_entry(&smach->list, struct smobj_s, lnk);
	dlist_del(&obj->lnk);
	obj->destruct(obj, smach);
	smach->deep--;
	return SMACH_OK;
}

struct smobj_s * smach_getObject(struct smach_s * smach, int index) {
	if (index >= 0) {
		if (smach->deep <= index) return NULL;
		struct dlist_head* it = smach->list.prev; 
		for(int i = 0; i < index; i++) {
			it = it->prev;
		}
		return dlist_entry(it, struct smobj_s, lnk);
	} else {
		if (smach->deep < -index) return NULL;
		struct dlist_head* it = &smach->list; 
		for(int i = 0; i < -index; i++) {
			it = it->next;
		}
		return dlist_entry(it, struct smobj_s, lnk);
	}
}

int smach_popAll(struct smach_s * smach) {
	while(smach->deep) smach_popBack(smach);
	return SMACH_OK;
}