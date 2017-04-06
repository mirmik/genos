#ifndef GXX_READLINEDYNAMIC_H
#define GXX_READLINEDYNAMIC_H

#include <gxx/Allocator.h>
#include <gxx/ByteArray.h>
#include <gxx/smartptr/shared.h>

#include <genos_configure.h>

namespace gxx {

	class ReadLineDynamic {
		gxx:: * data;
		Allocator* m_alloc;
	
	public:
		ReadLineDynamic(Allocator* alloc = GXX_DEFAULT_ALLOCATOR) : m_alloc(alloc) {}
	
		void reserve(size_t sz) {
			data->reserve(sz);
			capacity = sz;
		}
	
		int concat(const char* str, size_t len) {
			return data->concat(str, len);
		}

		int concat(const char chr) {
			return data->concat(chr);
		}

		gxx::shared_ptr
	};
}

#endif