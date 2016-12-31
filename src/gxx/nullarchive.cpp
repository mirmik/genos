#include <gxx/future/nullarchive.h>
#include <util/ascii_convert.h>

namespace gxx {

	void NullArchive::pull(uint8_t& ref) {}; 	
	void NullArchive::pull(uint16_t& ref) {};	
	void NullArchive::pull(uint32_t& ref) {};	
	void NullArchive::pull(uint64_t& ref) {};	
	void NullArchive::pull(int8_t& ref) {};	
	void NullArchive::pull(int16_t& ref) {};	
	void NullArchive::pull(int32_t& ref) {};
	void NullArchive::pull(int64_t& ref) {};

	void NullArchive::push(uint8_t& ref) {};
	void NullArchive::push(uint16_t& ref) {};	
	void NullArchive::push(uint32_t& ref) {};	
	void NullArchive::push(uint64_t& ref) {};	
	void NullArchive::push(int8_t& ref) {};	
	void NullArchive::push(int16_t& ref) {};	
	void NullArchive::push(int32_t& ref) {};	
	void NullArchive::push(int64_t& ref) {};
};