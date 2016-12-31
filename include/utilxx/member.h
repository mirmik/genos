#ifndef GENOS_UTILXX_MEMBER_H
#define GENOS_UTILXX_MEMBER_H

#include "inttypes.h"

template<typename Type, typename MemberType, MemberType Type::*member>
inline uintptr_t member_offset()
{
	constexpr uintptr_t ret = (uintptr_t)&(((Type*)0)->*member);
	return ret;
};

template<typename Type, typename MemberType, MemberType Type::*member>
struct member_helper
{
	constexpr static uintptr_t offset = (uintptr_t)&(((Type*)0)->*member);
};

template<typename Type, typename MemberType, MemberType Type::*member>
inline Type* member_container(MemberType* ptr)
{
	Type* ret = (Type*)((char*)ptr - member_helper<Type,MemberType,member>::offset);
	return ret;
};

#endif