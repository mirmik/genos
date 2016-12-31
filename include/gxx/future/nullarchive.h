#ifndef GXX_NULL_ARCHIVE_H
#define GXX_NULL_ARCHIVE_H

#include <gxx/future/archive.h>

namespace gxx {

	class NullArchive {
	public:

		int pull(uint8_t& ref);
		int pull(uint16_t& ref);
		int pull(uint32_t& ref);
		int pull(uint64_t& ref);
		int pull(int8_t& ref);
		int pull(int16_t& ref);
		int pull(int32_t& ref);
		int pull(int64_t& ref);

		int push(uint8_t& ref);
		int push(uint16_t& ref);
		int push(uint32_t& ref);
		int push(uint64_t& ref);
		int push(int8_t& ref);
		int push(int16_t& ref);
		int push(int32_t& ref);
		int push(int64_t& ref);
	
	public:
		NullArchive() {}
	};

}


#endif 