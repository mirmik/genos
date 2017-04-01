#ifndef GENOS_UTILXX_MEMBER_H
#define GENOS_UTILXX_MEMBER_H

#include "inttypes.h"

template<typename Type, typename MemberType>
static inline uintptr_t member_offset(MemberType Type::*member) {
	return (uintptr_t)&(((Type*)0)->*member);
};

template<typename Type, typename MemberType>
static inline Type* member_container(MemberType* ptr, MemberType Type::*member) {
	Type* ret = (Type*)(char*)ptr - member_offset(member);
	return ret;
}

#endif