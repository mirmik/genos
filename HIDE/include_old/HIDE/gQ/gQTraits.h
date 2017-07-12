#ifndef GQ_TRAITS_H
#define GQ_TRAITS_H

#include <genos/defines/size_t.h>
#include <genos/defines/ssize_t.h>

template <typename T> struct gQTraits {
	using val_t = T;
	using ptr_t = T*;
	using ref_t = T&;
	using rref_t = T&&;
	using size_t = ::size_t;
	using diff_t = ssize_t;
};

#endif