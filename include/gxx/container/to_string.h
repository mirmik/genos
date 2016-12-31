#ifndef GENOS_TO_STRING_H
#define GENOS_TO_STRING_H

#include "gxx/ByteArray.h"

namespace gxx {

	template<typename type>
	gxx::string to_str(const type& value)
	{
		return value.to_str();
	};
	
	gxx::string to_str(const int8_t& value);
	gxx::string to_str(const int16_t& value);
	gxx::string to_str(const int32_t& value);
	gxx::string to_str(const int64_t& value);
	
	gxx::string to_str(const uint8_t& value);
	gxx::string to_str(const uint16_t& value);
	gxx::string to_str(const uint32_t& value);
	gxx::string to_str(const uint64_t& value);

};

#endif