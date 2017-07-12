#ifndef GXX_ITERATOR_BASE_TYPES_H
#define GXX_ITERATOR_BASE_TYPES_H

#include <stddef.h>

namespace gxx {
	///  Marking input iterators.
	struct input_iterator_tag { };

	///  Marking output iterators.
	struct output_iterator_tag { };

	/// Forward iterators support a superset of input iterator operations.
	struct forward_iterator_tag : public input_iterator_tag { };

	/// Bidirectional iterators support a superset of forward iterator
	/// operations.
	struct bidirectional_iterator_tag : public forward_iterator_tag { };

	/// Random-access iterators support a superset of bidirectional
	/// iterator operations.
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };
	
	/**
	 *  @brief  Common %iterator class.
	 *
	 *  This class does nothing but define nested typedefs.  %Iterator classes
	 *  can inherit from this class to save some work.  The typedefs are then
	 *  used in specializations and overloading.
	 *
	 *  In particular, there are no default implementations of requirements
	 *  such as @c operator++ and the like.  (How could there be?)
	*/
	template<typename Category, typename Tp, typename Distance = ptrdiff_t,
			 typename Pointer = Tp*, typename Reference = Tp&>
	struct iterator
	{
		/// One of the @link iterator_tags tag types@endlink.
		typedef Category  iterator_category;
		/// The type "pointed to" by the iterator.
		typedef Tp        value_type;
		/// Distance between iterators is represented as this type.
		typedef Distance  difference_type;
		/// This type represents a pointer-to-value_type.
		typedef Pointer   pointer;
		/// This type represents a reference-to-value_type.
		typedef Reference reference;
	};

	/**
	 *  @brief  Traits class for iterators.
	 *
	 *  This class does nothing but define nested typedefs.  The general
	 *  version simply @a forwards the nested typedefs from the Iterator
	 *  argument.  Specialized versions for pointers and pointers-to-const
	 *  provide tighter, more correct semantics.
	*/
	template<typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::difference_type   difference_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
	};

	/*

	template<typename _Iterator, typename = __void_t<>>
	struct __iterator_traits { };

	template<typename _Iterator>
	struct __iterator_traits<_Iterator, 
			   __void_t<typename _Iterator::iterator_category,
						typename _Iterator::value_type,
						typename _Iterator::difference_type,
						typename _Iterator::pointer,
						typename _Iterator::reference>>
	{
		typedef typename _Iterator::iterator_category iterator_category;
		typedef typename _Iterator::value_type        value_type;
		typedef typename _Iterator::difference_type   difference_type;
		typedef typename _Iterator::pointer           pointer;
		typedef typename _Iterator::reference         reference;
	};

	template<typename _Iterator> struct iterator_traits : public __iterator_traits<_Iterator> { };

	*/

	/// Partial specialization for pointer types.
	template<typename Tp>
	struct iterator_traits<Tp*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef Tp                         value_type;
		typedef ptrdiff_t                   difference_type;
		typedef Tp*                        pointer;
		typedef Tp&                        reference;
	};

	/// Partial specialization for const pointer types.
	template<typename Tp>
	struct iterator_traits<const Tp*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef Tp                         value_type;
		typedef ptrdiff_t                  difference_type;
		typedef const Tp*                  pointer;
		typedef const Tp&                  reference;
	};

	/**
	 *  This function is not a part of the C++ standard but is syntactic
	 *  sugar for internal library use only.
	*/
	template<typename Iter>
	inline typename iterator_traits<Iter>::iterator_category __iterator_category(const Iter&) { 
		return typename iterator_traits<Iter>::iterator_category(); 
	}

	// If Iterator has a base returns it otherwise _Iterator is returned
	// untouched
	/*template<typename Iterator, bool HasBase>
	struct _Iter_base
	{
		typedef Iterator iterator_type;
		static iterator_type _S_base(Iterator __it) 	{ return __it; }
	};

	template<typename Iterator>
	struct _Iter_base<Iterator, true>
	{
		typedef typename Iterator::iterator_type iterator_type;
		static iterator_type _S_base(Iterator __it)
		{ return __it.base(); }
	};*/
} // namespace

#endif

