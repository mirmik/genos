#ifndef COMPILER_H_
#define COMPILER_H_

#undef putc
#undef getc

#define _NORETURN __attribute__((noreturn))
#define _PRINTF_FORMAT(format_i, arg_start_i) \
 	__attribute__ ((format (printf, format_i, arg_start_i)))

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

#define __extmem
#define __packed __attribute__((packed))

#undef EXTERN_C
#ifdef __cplusplus
# define EXTERN_C extern "C"
#else
# define EXTERN_C extern
#endif

#undef __BEGIN_DECLS
#ifdef __cplusplus
#define __BEGIN_DECLS extern "C" {
#else
#define __BEGIN_DECLS 
#endif

#undef __END_DECLS
#ifdef __cplusplus
#define __END_DECLS }
#else
#define __END_DECLS
#endif

#ifdef __cplusplus
#define __if_cplusplus(...) __VA_ARGS__
#define __if_not_cplusplus(...)
#define __if_C_if_CXX(c, cxx) cxx
#else
#define __if_cplusplus(...)
#define __if_not_c_plusplus(...) __VA_ARGS__
#define __if_C_if_CXX(c, cxx) c 
#endif

//#define types_compatible(type1,type2) __builtin_types_compatible_p(type1, type2)

#endif /* COMPILER_H_ */
