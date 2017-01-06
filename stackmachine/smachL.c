#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <smachL.h>
#include <datastruct/dlist_head.h>

void smachL_init(struct smach_s * smach) {
	smach_init(smach, malloc, free); 
}

void smachL_pushString(struct smach_s * smach, const char * str) {
	struct smobj_s * obj = smach_constructAllocated (smach, (void*)str, strlen(str));
	smach_pushBack(smach, obj);
}

void smachL_pushInt32(struct smach_s * smach, int32_t integer) {
	struct smobj_s * obj = smach_constructAllocated (smach, (void*)&integer, sizeof(int32_t));
	smach_pushBack(smach, obj);
}

const char * smachL_toString(struct smach_s * smach, int index, size_t* size) {
	struct smobj_s * obj = smach_getObject(smach, index);
	if (!obj) return NULL;
	if (size) *size = obj->size;
	return obj->data;
}


/*DEBUG*/
#include <stdio.h>
void smachL_debugInfo(struct smach_s * smach) {
	printf("stack deep %d\r\n", smach->deep);
	struct smobj_s* it;
	dlist_for_each_entry(it, &smach->list, lnk) {
		printf("%d\r\n", it->size);
	}
}