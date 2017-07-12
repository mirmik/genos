#ifndef UTIL_MEMBER_H_
#define UTIL_MEMBER_H_

#include <stddef.h>

#ifndef __cplusplus
#	define mcast_offsetof(type, member) \
		offsetof(type, member)
#else
#include <utilxx/member.h>
#	define mcast_offsetof(type, member) \
		member_offset(&type::member)
#endif

/** typeof(foo.bar);  @a type is (an expr of) a struct or a union */
#define member_typeof(type, member_nm) \
	typeof(((typeof(type) *) 0x0)->member_nm)

/** sizeof(foo.bar);  @a type is (an expr of) a struct or a union */
#define member_sizeof(type, member_nm) \
	sizeof(((typeof(type) *) 0x0)->member_nm)


/** &foo --> &foo.bar;  @a struct_ptr must not be null */
#define mcast_in(struct_ptr, member) \
	(&(struct_ptr)->member)

/** &foo --> &foo.bar; NULL --> NULL; */
#define mcast_in_or_null(struct_ptr, member)	 			\
	({ 														\
		typeof(struct_ptr) __member_expr__ = (struct_ptr); 	\
		__member_expr__                                    	\
			? &__member_expr__->member                 		\
			: (typeof(&__member_expr__->member)) NULL; 		\
	})

/** &foo.bar --> &foo;  @a member_ptr must not be null */
#define mcast_out(member_ptr, type, member) \
	((type *) ((char *) (member_ptr) - mcast_offsetof(type, member)))

/** &foo.bar --> &foo; NULL --> NULL; */
#define mcast_out_or_null(member_ptr, type, member) 			\
	({                                                          \
		char *__member_expr__ = (char *) (member_ptr);          \
		(type *) (__member_expr__ ?                             \
			__member_expr__ - mcast_offsetof(type, member) : NULL); 	\
	})

//SYNONIM
//#define container_of(member_ptr, type, member) \
//	mcast_out(member_ptr, type, member)

#endif /* UTIL_MEMBER_H_ */
