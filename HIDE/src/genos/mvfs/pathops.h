/**
	@file Операции над строковыми путями
 */

#ifndef MVFS_PATHOPS_H
#define MVFS_PATHOPS_H

#include <igris/compiler.h>

__BEGIN_DECLS

///Истина, если следующий элемент пути - двойная точка.
extern int path_is_double_dot(const char *path);

///Истина, если следующий элемент пути - точка.
extern int path_is_single_dot(const char *path);

/**
 * @brief Get stripped from leading '/' path and lenght of first node's name
 * For example,
 * for '/fooo/bar/' returns 'fooo/bar/' and 4 on plen
 * for '/bar/' ('fooo/bar/' + 4) returns 'bar/', 3
 * for '/' ('bar/' + 3) return NULL
 *
 * @param path Path to process
 * @param p_len Pointer to store first node name's length
 *
 * @return Path stripped from leading '/' if any
 * 	NULL if empty path supplied or none left
 */
extern const char *path_next(const char *path, unsigned int *p_len);

extern int path_is_simple(const char *path);

extern int path_is_abs(const char *path);

__END_DECLS

#endif