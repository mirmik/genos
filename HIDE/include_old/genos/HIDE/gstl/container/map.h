#ifndef GENOS_GSTL_MAP
#define GENOS_GSTL_MAP
	
	
	#include <genos/gstl/utility.h>
	#include "genos/gstl/allocator.h"
	
	namespace gxx {
		
		template <
		class valtype, 
		class traits_type = genos::elem_traits<valtype>
		>
		class map {
			
			typedef typename traits_type::	ptr_type 			pointer;
			typedef typename traits_type::	const_ptr_type 		const_pointer;
			typedef typename traits_type::	reference 			reference;
			typedef typename traits_type::	const_reference 	const_reference;
			typedef 						valtype				value_type;
			typedef typename traits_type::	size_type 			size_type;
			typedef typename traits_type::	ptrdiff_t 			difference_type;
			
			gxx::allocator<valtype> alloc;
			
					
		};
		
	};
	
	
#endif