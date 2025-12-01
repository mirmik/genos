#ifndef GENOS_UTILXX_MEMBER_H
#define GENOS_UTILXX_MEMBER_H

//Аналоги offsetof и container_of для с++.

#include "inttypes.h"

template <typename Type, typename MemberType>
static inline uintptr_t member_offset(MemberType Type::*member)
{
    return (uintptr_t) & (((Type *)0)->*member);
}

template <typename Type, typename MemberType>
static inline Type *member_container(MemberType *ptr, MemberType Type::*member)
{
    Type *ret = reinterpret_cast<Type *>((char *)ptr - member_offset(member));

    return ret;
}

#endif
