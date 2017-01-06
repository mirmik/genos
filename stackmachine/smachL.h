#ifndef GENOS_SMACH_LIB_H
#define GENOS_SMACH_LIB_H

#include <smach.h>

void smachL_init(struct smach_s * smach);
void smachL_pushString(struct smach_s * smach, const char * str);
void smachL_pushInt32(struct smach_s * smach, int32_t integer);
void smachL_debugInfo(struct smach_s * smach);

const char * smachL_toString(struct smach_s * smach, int index, size_t* size);
int32_t smachL_toInt32(struct smach_s * smach, int index, uint8_t* isnum);

/*
int smachL_push_string(struct smach_s * smach, const char * str);
int smachL_pop_string(struct smach_s * smach, char** str, size_t* s);
*/
#endif