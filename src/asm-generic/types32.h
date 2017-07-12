#ifndef ASM_GENERIC_TYPES32_H_
#define ASM_GENERIC_TYPES32_H_

#define __WORDSIZE         32

#ifndef __ASSEMBLER__

//#define __SWORD_TYPE int

typedef signed char        __s8;
typedef unsigned char      __u8;
typedef signed short       __s16;
typedef unsigned short     __u16;
typedef signed int         __s32;
typedef unsigned int       __u32;
typedef long long          __s64;
typedef unsigned long long __u64;

typedef unsigned int      __size_t;
typedef signed int        __ssize_t;
typedef signed int        __ptrdiff_t;

typedef unsigned int __uintptr_t;
typedef int          __intptr_t;

typedef __s32 __s_fast;
typedef __u32 __u_fast;

typedef __s32 __atomic_t;

#endif /* __ASSEMBLER__ */

#endif /* ASM_GENERIC_TYPES32_H_ */
