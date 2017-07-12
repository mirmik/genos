/**
 * @file
 *
 * @date Aug 29, 2013
 * @author: Anton Bondarev
 */

#ifndef SSIZE_T_DEF_H_
#define SSIZE_T_DEF_H_

#ifdef __SSIZE_TYPE__
 typedef __SSIZE_TYPE__ ssize_t;
#else
 #include <asm/types.h>
 typedef __ssize_t ssize_t;
#endif


#endif /* SSIZE_T_DEF_H_ */
