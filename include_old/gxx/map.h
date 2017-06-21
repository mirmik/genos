#ifndef GXX_MAP_H
#define GXX_MAP_H

#include <utilxx/setget.h>

#include <gxx/container/objsequence.h>
#include <gxx/allocator.h>

namespace gxx {
	/*template<
		typename K, 
		typename V, 
		class Compare = gxx::less<K>, 
		class Allocator = gxx::allocator<gxx::pair<K,V>>
	>
	class map : public gxx::objsequence<gxx::pair<K,V>, Allocator> {
		//types
		using Node = gxx::pair<const K, V>;
		using Parent = gxx::objsequence<gxx::pair<K,V>, Allocator>;

		using iterator = typename Parent::iterator;
		
		//resources
		//node_type* m_data = nullptr;
		//size_t m_size = 0;
		//size_t m_capacity = 0;

		//generated
		//objsequence : Allocator m_alloc
		Compare 	m_comp;		

	public:
		CONSTREF_GETTER(data, Parent::m_data)
		CONSTREF_GETTER(size, Parent::m_size)
		CONSTREF_GETTER(capacity, Parent::m_capacity)

		VALUE_GETTER(empty, Parent::m_size == 0);

		//constructors
		map() : map( Compare() ) {}
		explicit map( const Compare& comp, const Allocator& alloc = Allocator() ) 
			: Parent(alloc), m_comp(comp) {};
	};*/
}

#endif 