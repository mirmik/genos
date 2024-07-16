#include "bincommon.h"

std::map<std::string, size_t> visitor_size =
{
	{"f64", 8}, 
	{"i64", 8}, 
	{"u64", 8},
	{"f32", 4}, 
	{"i32", 4},
	{"u32", 4},
	{"f16", 2}, 
	{"i16", 2}, 
	{"u16", 2},
	{"f8", 1}, 
	{"i8", 1}, 
	{"u8", 1}
};
