#ifndef IGRIS_INCLUDE_STRING_VIEW_H
#define IGRIS_INCLUDE_STRING_VIEW_H

#ifdef __has_include
#if __has_include(<string_view>) && (__cplusplus >= 201703L)
#include <string_view>
#define IGRIS_HAS_STRING_VIEW 1
#else
#define IGRIS_HAS_STRING_VIEW 0
#endif
#if __has_include(<string>)
#include <string>
#define IGRIS_HAS_STRING 1
#else
#define IGRIS_HAS_STRING 0
#endif
#else
#include <string>
#include <string_view>
#define IGRIS_HAS_STRING_VIEW 1
#define IGRIS_HAS_STRING 1
#endif

#endif
