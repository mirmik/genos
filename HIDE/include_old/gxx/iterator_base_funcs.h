#ifndef GXX_ITERATOR_BASE_FUNCS_H
#define GXX_ITERATOR_BASE_FUNCS_H

#include <gxx/iterator_base_types.h>

namespace gxx
{
	template<typename _InputIterator>
	inline typename iterator_traits<_InputIterator>::difference_type __distance(_InputIterator __first, _InputIterator __last, input_iterator_tag) {
		typename iterator_traits<_InputIterator>::difference_type __n = 0;
		while (__first != __last) {
			++__first;
			++__n;
		}
		return __n;
	}

	template<typename _RandomAccessIterator>
	inline typename iterator_traits<_RandomAccessIterator>::difference_type __distance(_RandomAccessIterator __first, _RandomAccessIterator __last, random_access_iterator_tag) {
		return __last - __first;
	}

	/**
	 *  @brief A generalization of pointer arithmetic.
	 *  @param  __first  An input iterator.
	 *  @param  __last  An input iterator.
	 *  @return  The distance between them.
	 *
	 *  Returns @c n such that __first + n == __last.  This requires
	 *  that @p __last must be reachable from @p __first.  Note that @c
	 *  n may be negative.
	 *
	 *  For random access iterators, this uses their @c + and @c - operations
	 *  and are constant time.  For other %iterator classes they are linear time.
	*/
	template<typename _InputIterator>
	inline typename iterator_traits<_InputIterator>::difference_type distance(_InputIterator __first, _InputIterator __last) {
		return gxx::__distance(__first, __last, gxx::__iterator_category(__first));
	}

	template<typename _InputIterator, typename _Distance>
	inline void
	__advance(_InputIterator& __i, _Distance __n, input_iterator_tag)
	{
		assert(__n >= 0);
		while (__n--) ++__i;
	}

	template<typename _BidirectionalIterator, typename _Distance>
	inline void __advance(_BidirectionalIterator& __i, _Distance __n, bidirectional_iterator_tag) {
		if (__n > 0) while (__n--) ++__i;
		else while (__n++) --__i;
	}

	template<typename _RandomAccessIterator, typename _Distance> 
	inline void __advance(_RandomAccessIterator& __i, _Distance __n, random_access_iterator_tag) {
		__i += __n;
	}

	/**
	 *  @brief A generalization of pointer arithmetic.
	 *  @param  __i  An input iterator.
	 *  @param  __n  The @a delta by which to change @p __i.
	 *  @return  Nothing.
	 *
	 *  This increments @p i by @p n.  For bidirectional and random access
	 *  iterators, @p __n may be negative, in which case @p __i is decremented.
	 *
	 *  For random access iterators, this uses their @c + and @c - operations
	 *  and are constant time.  For other %iterator classes they are linear time.
	*/
	template<typename _InputIterator, typename _Distance>
	inline void advance(_InputIterator& __i, _Distance __n) {
		typename iterator_traits<_InputIterator>::difference_type __d = __n;
		gxx::__advance(__i, __d, gxx::__iterator_category(__i));
	}

	template<typename _ForwardIterator> 
	inline _ForwardIterator next(_ForwardIterator __x, typename iterator_traits<_ForwardIterator>::difference_type __n = 1) {
		gxx::advance(__x, __n);
		return __x;
	}

	template<typename _BidirectionalIterator>
	inline _BidirectionalIterator prev(_BidirectionalIterator __x, typename iterator_traits<_BidirectionalIterator>::difference_type __n = 1) {
		gxx::advance(__x, -__n);
		return __x;
	}

} // namespace

#endif
