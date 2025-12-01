/**@file*/

#ifndef IGRIS_PATHOPS_H
#define IGRIS_PATHOPS_H

#include <cstring>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <igris/util/bug.h>

static inline int path_is_single_dot(const char *path)
{
    char nc = *(path + 1);

    return *path == '.' && (nc == '/' || nc == '\0');
}

// Промотать указатель до следующего элемента пути.
// @param p_len - длина промотанного контента.
static inline const char *path_next(const char *path, unsigned int *p_len)
{

    if (!path)
    {
        return NULL;
    }

    // Skip leading slashes.
    // XXX Skip single dots too
    while (*path == '/' || path_is_single_dot(path))
    {
        ++path;
    }

    if (!*path)
    {
        return NULL;
    }

    if (p_len)
    {
        const char *end = path;

        while (*end && *end != '/')
        {
            ++end;
        }

        *p_len = (unsigned int)(end - path);
    }

    return path;
}

static inline const char *path_skip_slashes_and_single_dots(const char *path)
{
    while (*path == '/' || path_is_single_dot(path))
    {
        ++path;
    }

    return path;
}

/// Перемотать нод до следующего.
/// Слеш щитается нодом, на пустую строку отдаётся NULL.
static inline const char *path_iterate(const char *path)
{
    if (!path || *path == '\0')
    {
        return NULL;
    }

    if (*path == '/')
    {
        path = path_skip_slashes_and_single_dots(path);
        return path;
    }

    while (*path && *path != '/')
        ++path;
    while (*path == '/' || path_is_single_dot(path))
        ++path;

    return path;
}

static inline int path_is_double_dot(const char *path)
{
    return *path == '.' && *(path + 1) == '.' &&
           (*(path + 2) == '/' || *(path + 2) == '\0');
}

static inline int path_is_abs(const char *path)
{
    return path[0] == '/';
}

static inline int path_is_simple(const char *path)
{
    char c;

    // for(c = *path++; c; c = *path++)
    while ((c = *path++))
        if (c == '/')
            return false;

    return true;
}

static inline const char *path_last_node(const char *path)
{
    const char *it = path + strlen(path);

    do
    {
        --it;
    } while (*it != '\\' && it != path);

    if (*it == '\\')
        it++;

    return it;
}

/// Сравнивает ноды, на которые сейчас указывают указатели, лексикографически
static inline int path_compare_node(const char *a, const char *b)
{
    while (*a != '\0' && *a != '/' && *b != '\0' && *b != '/')
    {
        if (*a == *b)
        {
            ++a;
            ++b;
            continue;
        }

        return *a < *b ? -1 : 1;
    }

    if (*a == '\0' || *a == '/')
    {
        if (*b == '\0' || *b == '/')
        {
            return 0;
        }

        return -1;
    }

    return 1;
}

static inline const char *path_remove_prefix(const char *path,
                                             const char *prefix)
{
    while (*prefix != 0 || *path != 0)
    {
        int cmp = path_compare_node(path, prefix);

        if (cmp == 0)
        {
            path = path_iterate(path);
            prefix = path_iterate(prefix);
        }

        else
        {
            break;
        }
    }

    return path;
}

#endif
